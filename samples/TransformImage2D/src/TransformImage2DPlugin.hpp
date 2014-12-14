#ifndef _ALCHEMY_PLUGIN_TRANSFORMIMAGE2D_PLUGIN_HPP_
#define _ALCHEMY_PLUGIN_TRANSFORMIMAGE2D_PLUGIN_HPP_

#include "TransformImage2DDefinitions.hpp"

#include <ImageEffectOpenCVPlugin.hpp>

namespace alchemy {
namespace plugin {
namespace transformImage2D {

using namespace tuttle;
using namespace tuttle::plugin;

class TransformImage2DProcessParams
{
public:
    cv::Point2d _anchor;
    cv::Point2d _position;
    double _rotation;
    cv::Point2d _scale;

    cv::Mat computeAffineMatrix() const;
};

/**
 * @brief TransformImage2D plugin
 */
class TransformImage2DPlugin : public ImageEffectOpenCVPlugin
{
public:
    TransformImage2DPlugin( OfxImageEffectHandle handle );

public:
    TransformImage2DProcessParams getProcessParams( const OfxPointD& renderScale = OFX::kNoRenderScale ) const;

    void changedParam( const OFX::InstanceChangedArgs &args, const std::string &paramName );

	bool getRegionOfDefinition( const OFX::RegionOfDefinitionArguments& args, OfxRectD& rod );
	void getRegionsOfInterest( const OFX::RegionsOfInterestArguments& args, OFX::RegionOfInterestSetter& rois );
	bool isIdentity( const OFX::RenderArguments& args, OFX::Clip*& identityClip, double& identityTime );

    void render( const OFX::RenderArguments &args );
	
public:
    OFX::Double2DParam* _paramAnchor;
    OFX::Double2DParam* _paramPosition;
    OFX::DoubleParam* _paramRotation;
    OFX::Double2DParam* _paramScale;
};

}
}
}

#endif
