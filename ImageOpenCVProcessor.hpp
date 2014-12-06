#ifndef _TUTTLE_PLUGIN_IMAGEOPENCVPROCESSOR_HPP_
#define _TUTTLE_PLUGIN_IMAGEOPENCVPROCESSOR_HPP_

#include <tuttle/plugin/ImageProcessor.hpp>

//#include <terry/globals.hpp>
#include "ofxToOpenCV/image.hpp"

namespace tuttle {
namespace plugin {

/**
 * @brief Base class that can be used to process images of any type using boost::gil library view to access images.
 */
class ImageOpenCVProcessor : public ImageProcessor
{
public:

protected:
    cv::Mat _dstView; ///< image to process into

public:
    ImageOpenCVProcessor( OFX::ImageEffect& effect, const EImageOrientation imageOrientation )
		: ImageProcessor( effect, imageOrientation )
	{}
    virtual ~ImageOpenCVProcessor() {}

	virtual void setup( const OFX::RenderArguments& args )
	{
		ImageProcessor::setup( args );
		_dstView           = getView( _dst.get(), _dstPixelRod );

		#ifndef TUTTLE_PRODUCTION
		// init dst buffer with red to highlight uninitialized pixels
		const OfxRectI dstBounds = this->translateRoWToOutputClipCoordinates( _dst->getBounds() );
        cv::Mat dstToFill = _dstView(cv::Rect(dstBounds.x1, dstBounds.y1, dstBounds.x2 - dstBounds.x1, dstBounds.y2 - dstBounds.y1 ));
        const cv::Scalar errorColor( 1.0, 0.0, 0.0, 1.0 );
        dstToFill.setTo(errorColor );
		#endif
	}

	/**
	 * @brief Return a full gil view of an image.
	 */
    cv::Mat getView( OFX::Image* img, const OfxRectI& pixelRod ) const
	{
        return tuttle::plugin::getOpenCVMat( img, pixelRod, _imageOrientation );
	}
};

}
}

#endif

