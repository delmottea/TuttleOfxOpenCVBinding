#include "TransformImage2DPluginFactory.hpp"
#include "TransformImage2DPlugin.hpp"
#include "TransformImage2DDefinitions.hpp"

#include <limits>

namespace alchemy {
namespace plugin {
namespace transformImage2D {

using namespace tuttle;
using namespace tuttle::plugin;

static const bool kSupportTiles = true;


/**
 * @brief Function called to describe the plugin main features.
 * @param[in, out] desc Effect descriptor
 */
void TransformImage2DPluginFactory::describe( OFX::ImageEffectDescriptor& desc )
{
    printf("describe\n");
	desc.setLabels(
        "AlchemyTransformImage2D",
        "TransformImage2D",
        "TransformImage2D"
		);
    desc.setPluginGrouping( "alchemy" );

	// add the supported contexts, only filter at the moment
	desc.addSupportedContext( OFX::eContextFilter );
	desc.addSupportedContext( OFX::eContextGeneral );

	// add supported pixel depths
	desc.addSupportedBitDepth( OFX::eBitDepthUByte );
	desc.addSupportedBitDepth( OFX::eBitDepthUShort );
	desc.addSupportedBitDepth( OFX::eBitDepthFloat );

	// plugin flags
	desc.setSupportsTiles( kSupportTiles );
	desc.setRenderThreadSafety( OFX::eRenderFullySafe );
}

/**
 * @brief Function called to describe the plugin controls and features.
 * @param[in, out]   desc       Effect descriptor
 * @param[in]        context    Application context
 */
void TransformImage2DPluginFactory::describeInContext( OFX::ImageEffectDescriptor& desc,
                                                  OFX::EContext context )
{
	OFX::ClipDescriptor* srcClip = desc.defineClip( kOfxImageEffectSimpleSourceClipName );
	srcClip->addSupportedComponent( OFX::ePixelComponentRGBA );
	srcClip->addSupportedComponent( OFX::ePixelComponentRGB );
	srcClip->addSupportedComponent( OFX::ePixelComponentAlpha );
	srcClip->setSupportsTiles( kSupportTiles );

	// Create the mandated output clip
	OFX::ClipDescriptor* dstClip = desc.defineClip( kOfxImageEffectOutputClipName );
	dstClip->addSupportedComponent( OFX::ePixelComponentRGBA );
	dstClip->addSupportedComponent( OFX::ePixelComponentRGB );
	dstClip->addSupportedComponent( OFX::ePixelComponentAlpha );
	dstClip->setSupportsTiles( kSupportTiles );


    OFX::Double2DParamDescriptor* anchor = desc.defineDouble2DParam( kParamAnchor );
    anchor->setLabel( "Anchor" );
    anchor->setDefault( 0, 0 );

    OFX::Double2DParamDescriptor* position = desc.defineDouble2DParam( kParamPosition );
    position->setLabel( "Position" );
    position->setDefault( 0, 0 );

    OFX::DoubleParamDescriptor* rotation = desc.defineDoubleParam( kParamRotation );
    rotation->setLabel( "Rotation" );
    rotation->setDefault( 0 );

    OFX::Double2DParamDescriptor* scale = desc.defineDouble2DParam( kParamScale );
    scale->setLabel( "Scale" );
    scale->setDefault( 1, 1 );

}

/**
 * @brief Function called to create a plugin effect instance
 * @param[in] handle  Effect handle
 * @param[in] context Application context
 * @return  plugin instance
 */
OFX::ImageEffect* TransformImage2DPluginFactory::createInstance( OfxImageEffectHandle handle,
                                                            OFX::EContext context )
{
    return new TransformImage2DPlugin( handle );
}

}
}
}

