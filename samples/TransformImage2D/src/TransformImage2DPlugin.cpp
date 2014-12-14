#include "TransformImage2DPlugin.hpp"
#include "TransformImage2DProcess.hpp"
#include "TransformImage2DDefinitions.hpp"

namespace alchemy {
namespace plugin {
namespace transformImage2D {

using namespace tuttle;
using namespace tuttle::plugin;

cv::Rect_<double> applyAffineMat(cv::Mat affine, cv::Rect_<double> rect)
{
    cv::Point2d p2[4];
    for(int i = 0; i < 4; i++)
    {
        cv::Point2d p(i%2==0?rect.x:(rect.x+rect.width), i/2==0?rect.y:(rect.y+rect.height));
        p2[i] = cv::Point2d(affine.at<float>(0)*p.x+affine.at<float>(1)*p.y+affine.at<float>(2), affine.at<float>(3)*p.x+affine.at<float>(4)*p.y+affine.at<float>(5));
    }
    cv::Point2d min(p2[0].x, p2[0].y), max(p2[0].x, p2[0].y);
    for(int i = 1; i < 4; i++)
    {
        min.x = std::min(min.x, p2[i].x);
        min.y = std::min(min.y, p2[i].y);
        max.x = std::max(max.x, p2[i].x);
        max.y = std::max(max.y, p2[i].y);
    }
    return cv::Rect_<double>(min.x, min.y, max.x-min.x, max.y-min.y);
}

cv::Mat TransformImage2DProcessParams::computeAffineMatrix() const
{
    cv::Mat affine(2,3,CV_32F);
    float c = (float)cos(_rotation);
    float s = (float)sin(_rotation);
    affine.at<float>(0,0) = _scale.x*c; affine.at<float>(0,1) =  -_scale.y*s; affine.at<float>(0,2) =  _position.x+_anchor.x-_scale.x*_anchor.x*c+_scale.y*_anchor.y*s;
    affine.at<float>(1,0) = _scale.x*s; affine.at<float>(1,1) =   _scale.y*c; affine.at<float>(1,2) =  _position.y+_anchor.y-_scale.x*_anchor.x*s-_scale.y*_anchor.y*c;
    return affine;
}

TransformImage2DPlugin::TransformImage2DPlugin( OfxImageEffectHandle handle )
: ImageEffectOpenCVPlugin( handle )
{
    _paramAnchor = fetchDouble2DParam( kParamAnchor);
    _paramPosition = fetchDouble2DParam( kParamPosition);
    _paramRotation = fetchDoubleParam( kParamRotation);
    _paramScale = fetchDouble2DParam( kParamScale);
}

TransformImage2DProcessParams TransformImage2DPlugin::getProcessParams( const OfxPointD& renderScale ) const
{
    TransformImage2DProcessParams params;

    params._anchor = ofxToOpenCV(_paramAnchor->getValue());
    params._position = ofxToOpenCV(_paramPosition->getValue());
    params._rotation = _paramRotation->getValue();
    params._scale = ofxToOpenCV(_paramScale->getValue());
	
	return params;
}

void TransformImage2DPlugin::changedParam( const OFX::InstanceChangedArgs &args, const std::string &paramName )
{
}

bool TransformImage2DPlugin::getRegionOfDefinition( const OFX::RegionOfDefinitionArguments& args, OfxRectD& rod )
{
    const TransformImage2DProcessParams params = getProcessParams();
	OfxRectD srcRod = _clipSrc->getCanonicalRod( args.time );
    rod = openCVToOfx(applyAffineMat(params.computeAffineMatrix(), ofxToOpenCV(srcRod)));
    return true;
}

void TransformImage2DPlugin::getRegionsOfInterest( const OFX::RegionsOfInterestArguments& args, OFX::RegionOfInterestSetter& rois )
{
    const TransformImage2DProcessParams params = getProcessParams();
	OfxRectD srcRoi;
    cv::Mat invAffine;
    invertAffineTransform(params.computeAffineMatrix(), invAffine);
    srcRoi = openCVToOfx(applyAffineMat(invAffine, ofxToOpenCV(args.regionOfInterest)));

	rois.setRegionOfInterest( *this->_clipSrc, srcRoi );
}

bool TransformImage2DPlugin::isIdentity( const OFX::RenderArguments& args, OFX::Clip*& identityClip, double& identityTime )
{
    const TransformImage2DProcessParams params = getProcessParams();
	
    static const cv::Point2d zero(0, 0);
    if( params._position == zero && params._scale == zero && params._rotation == 0 )
	{
		identityClip = this->_clipSrc;
		identityTime = args.time;
		return true;
	}
	return false;
}

/**
 * @brief The overridden render function
 * @param[in]   args     Rendering parameters
 */
void TransformImage2DPlugin::render( const OFX::RenderArguments &args )
{
    doOpenCVRender<TransformImage2DProcess>( *this, args );
}


}
}
}
