#define OFXPLUGIN_VERSION_MAJOR 0
#define OFXPLUGIN_VERSION_MINOR 0

#include "TransformImage2DPluginFactory.hpp"
#include <tuttle/plugin/Plugin.hpp>

namespace OFX {
namespace Plugin {

void getPluginIDs( OFX::PluginFactoryArray& ids )
{
    mAppendPluginFactory( ids, alchemy::plugin::transformImage2D::TransformImage2DPluginFactory, "alchemy.transformImage2D" );
}

}
}

