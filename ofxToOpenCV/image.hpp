#ifndef _TUTTLE_OFXTOOPENCV_IMAGE_HPP_
#define	_TUTTLE_OFXTOOPENCV_IMAGE_HPP_

#include <tuttle/plugin/image.hpp>

#include <ofxsImageEffect.h>

#include <opencv2/opencv.hpp>


namespace tuttle {
namespace plugin {

/*class MatView
{
public:
    cv::Mat img;
    cv::Point origin;
    cv::Size size;

    MatView(cv::Mat img = cv::Mat(), cv::Point origin = cv::Point(0,0), cv::Size size = cv::Size(-1,-1))
        :img(img), origin(origin)
    {
        if(size.width >= 0 && size.height >= 0)
            this->size = size;
        else this->size = img.size();
    }

    MatView(const MatView& view, cv::Point origin = cv::Point(0,0), cv::Size size = cv::Size(-1,-1))
        :img(view.img), origin(view.origin+origin)
    {

        if(size.width >= 0 && size.height >= 0)
            this->size = size;
        else this->size = view.size;
    }

    MatView flipped_up_down_view()
    {
        MatView view = *this;
        view.img = cv::Mat(view.img.rows, view.img.cols, view.img.type(), view.img.data+(view.img.rows-1)*view.img.step, -view.img.step);
        return view;
    }

    cv::Mat getMat(cv::Rect rect = cv::Rect(0,0,-1,-1))
    {
        return img(cv::Rect(rect.x+origin.x, rect.y+origin.y, rect.width>=0?rect.width:size.width, rect.height>=0?rect.height:size.height));
    }
};*/

inline cv::Mat subimage_mat(cv::Mat mat, int offsetX, int offsetY, int width, int height)
{
    return cv::Mat(height, width, mat.type(), mat.data+offsetX+offsetY*mat.step, mat.step);
}

inline cv::Mat flipped_up_down_mat(cv::Mat fullView)
{
    return cv::Mat(fullView.rows, fullView.cols, fullView.type(), fullView.data+(fullView.rows-1)*fullView.step, -fullView.step);
}

inline int getOpenCVType(OFX::EBitDepth bitDepth, OFX::EPixelComponent components)
{
    if(bitDepth == OFX::eBitDepthUByte)
    {
        if(components == OFX::ePixelComponentAlpha)
            return CV_8UC1;
        else if(components == OFX::ePixelComponentRGB)
            return CV_8UC3;
        else if(components == OFX::ePixelComponentRGBA)
            return CV_8UC4;
    }
    else if(bitDepth == OFX::eBitDepthUShort)
    {
        if(components == OFX::ePixelComponentAlpha)
            return CV_16UC1;
        else if(components == OFX::ePixelComponentRGB)
            return CV_16UC3;
        else if(components == OFX::ePixelComponentRGBA)
            return CV_16UC4;
    }
    else if(bitDepth == OFX::eBitDepthFloat)
    {
        if(components == OFX::ePixelComponentAlpha)
            return CV_32FC1;
        else if(components == OFX::ePixelComponentRGB)
            return CV_32FC3;
        else if(components == OFX::ePixelComponentRGBA)
            return CV_32FC4;
    }
    BOOST_THROW_EXCEPTION( exception::Unsupported()
        << exception::user() + "Bit depth (" + mapBitDepthEnumToString(bitDepth) + ") or Pixel component ("
                           + mapPixelComponentEnumToString(components) + ") not recognized by the plugin." );
}

/**
 * @brief Return a full gil view of an image.
 *        If we only have a tiled image, we use subimage_view to fit to the rod.
 * @param img the ofx image object
 * @param rod normally we don't need this parameter because we can get it from the image,
 *            but there is a bug in nuke (which return the bounds),
 *            so we need to use the rod of the clip and not from the image.
 */
inline cv::Mat getOpenCVMat( OFX::Image* img, const OfxRectI& pixelRod, const EImageOrientation orientation )
{
    int type = getOpenCVType(img->getPixelDepth(), img->getPixelComponents());
	//TUTTLE_TLOG( TUTTLE_TRACE, "getGilView => " << img->getUniqueIdentifier() );

	//	OfxRectI imgrod = img->getRegionOfDefinition(); // bug in nuke returns bounds... not the clip rod with renderscale...
	const OfxRectI bounds = img->getBounds();
    printf("bounds %d %d %d %d\n", bounds.x1, bounds.y1, bounds.x2, bounds.y2);
    printf("rods %d %d %d %d\n", pixelRod.x1, pixelRod.y1, pixelRod.x2, pixelRod.y2);
//  TUTTLE_TLOG_VAR( TUTTLE_TRACE, bounds );
//	TUTTLE_TLOG_VAR( TUTTLE_TRACE, imgrod );
//	TUTTLE_TLOG_VAR( TUTTLE_TRACE, rod );
    const cv::Size tileSize( bounds.x2 - bounds.x1, bounds.y2 - bounds.y1 );

	// Build view
	/**
	 * About image ordering from OpenFX documentation:
	 * 
	 * Images are always left to right, bottom to top,
	 * with the pixel data pointer being at the bottom left of the image.
	 * The pixels in a scan line are contiguously packed.
	 * Scanlines need not be contiguously packed.
	 * The number of bytes between between a pixel in the same column,
	 * but separated by a scan line is known as the rowbytes of an image.
	 * Rowbytes can be negative, allowing for compositing systems with a native
	 * top to bottom scanline order to trivially support bottom to top images.
	 */

    cv::Mat tileView(tileSize.height, tileSize.width, type, img->getPixelData(), img->getRowDistanceBytes());
	
    TUTTLE_TLOG( TUTTLE_INFO, "[OFX to OpenCV] Row distance from image " << img->getRowDistanceBytes() << " bytes" );
    //TUTTLE_TLOG( TUTTLE_INFO, "[OFX to OpenCV] Tile view, row size = " << tileView.pixels().row_size() );
	
    cv::Mat fullView;
	const bool isTile = (
		bounds.x1 != pixelRod.x1 || bounds.y1 != pixelRod.y1 ||
	    bounds.x2 != pixelRod.x2 || bounds.y2 != pixelRod.y2 );
	// if the tile is equals to the full image
	// directly return the tile
	if( ! isTile )
	{
        TUTTLE_TLOG( TUTTLE_INFO, "[OFX to OpenCV] Tile is equal to the full view" );
		fullView = tileView;
	}
	else
	{
		// view the tile as a full image
		////TUTTLE_TLOG( TUTTLE_TRACE, "Tile to full view" );
        fullView = subimage_mat( tileView, pixelRod.x1 - bounds.x1, pixelRod.y1 - bounds.y1, pixelRod.x2 - pixelRod.x1, pixelRod.y2 - pixelRod.y1 );
    }
	
    //TUTTLE_TLOG( TUTTLE_INFO, "[OFX to OpenCV] Full view, row size = " << fullView.pixels().row_size() );
	
    cv::Mat resView = fullView;
	switch( orientation )
	{
		case eImageOrientationIndependant: // use memory order
		{
            TUTTLE_TLOG( TUTTLE_INFO, "[OFX to OpenCV] Image orientation independant"  );
			if( isTile ) // can't manage ordering
				break;

			if( img->getRowDistanceBytes() < 0 ) // if the host use buffer ordered from top to bottom
			{
                //flipped_up_down_view
                resView = flipped_up_down_mat(fullView);
			}
			break;
		}
		case eImageOrientationFromTopToBottom:
		{
            TUTTLE_TLOG( TUTTLE_INFO, "[OFX to OpenCV] Image orientation from top to bottom"  );
			BOOST_ASSERT( ! isTile ); // can't manage ordering with tiles currently (no RoW information in OpenFX)

            resView = flipped_up_down_mat(fullView);
			break;
		}
		case eImageOrientationFromBottomToTop:
		{
            TUTTLE_TLOG( TUTTLE_INFO, "[OFX to OpenCV] Image orientation from bottom to top"  );
			// by default in OpenFX we are in this order
			break;
		}
	}
	
	////TUTTLE_TLOG_VAR( TUTTLE_TRACE, resView.pixels().row_size() );
	return resView;
}


}
}



#endif

