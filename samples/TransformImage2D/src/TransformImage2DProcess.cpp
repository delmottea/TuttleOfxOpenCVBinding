
#include <ofxToOpenCV/all.hpp>
#include "TransformImage2DProcess.hpp"

namespace alchemy {
namespace plugin {
namespace transformImage2D {

using namespace tuttle;

TransformImage2DProcess::TransformImage2DProcess( TransformImage2DPlugin &effect )
: ImageOpenCVFilterProcessor( effect, eImageOrientationFromBottomToTop )
, _plugin( effect )
{

}

void TransformImage2DProcess::setup( const OFX::RenderArguments& args )
{
    ImageOpenCVFilterProcessor::setup( args );
	_params = _plugin.getProcessParams( args.renderScale );

}

/**
 * @brief Function called by rendering thread each time a process must be done.
 * @param[in] procWindowRoW  Processing window
 */
void TransformImage2DProcess::multiThreadProcessImages( const OfxRectI& procWindowRoW )
{
    OfxRectI procWindowOutput = this->translateRoWToOutputClipCoordinates( procWindowRoW );
	
    cv::Rect rect = ofxToOpenCV(procWindowOutput);
    cv::Mat affineMat = _params.computeAffineMatrix();
    affineMat.at<float>(0,2) -= this->_dstPixelRod.x1+rect.x;
    affineMat.at<float>(1,2) -= this->_dstPixelRod.y1+rect.y;
    cv::Mat tmp = affineMat;
    invertAffineTransform(tmp, affineMat);
    affineMat.at<float>(0,2) -= this->_srcPixelRod.x1;
    affineMat.at<float>(1,2) -= this->_srcPixelRod.y1;
    cv::warpAffine(_srcView, _dstView(rect), affineMat, cv::Size(rect.width, rect.height), cv::WARP_INVERSE_MAP);
    if( this->progressForward( rect.width*rect.height) )
        return;
}

}
}
}
