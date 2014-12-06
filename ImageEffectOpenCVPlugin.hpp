#ifndef _TUTTLE_PLUGIN_IMAGEEFFECT_OPENCV_PLUGIN_HPP_
#define _TUTTLE_PLUGIN_IMAGEEFFECT_OPENCV_PLUGIN_HPP_

#include <tuttle/plugin/global.hpp>
#include <tuttle/plugin/exceptions.hpp>
#include "ofxToOpenCV/all.hpp"

#include <ofxsImageEffect.h>
#include <ofxsMultiThread.h>

namespace tuttle {
namespace plugin {

/** 
 * @brief 
 *
 */
class ImageEffectOpenCVPlugin : public OFX::ImageEffect
{
public:
    ImageEffectOpenCVPlugin( OfxImageEffectHandle handle )
        : OFX::ImageEffect( handle )
    {
        _clipSrc = fetchClip( kOfxImageEffectSimpleSourceClipName );
        _clipDst = fetchClip( kOfxImageEffectOutputClipName );
    }
    virtual ~ImageEffectOpenCVPlugin(){}

public:
    // do not need to delete these, the ImageEffect is managing them for us
    OFX::Clip *_clipDst; ///< Destination image clip
    OFX::Clip *_clipSrc; ///< Source image clip
};

/**
 * @brief This render function, instanciate a Process class templated with the image type (layout and bit depth).
 * @param[in]   args     Rendering parameters
 */
template< class Process,
          class Plugin >
void doOpenCVRender( Plugin& plugin, const OFX::RenderArguments& args )
{
    Process procObj( plugin );

    procObj.setupAndProcess( args );
}

}
}


#endif
