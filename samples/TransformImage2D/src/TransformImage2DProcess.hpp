#ifndef _ALCHEMY_PLUGIN_TRANSFORMIMAGE2D_PROCESS_HPP_
#define _ALCHEMY_PLUGIN_TRANSFORMIMAGE2D_PROCESS_HPP_

#include <ImageOpenCVFilterProcessor.hpp>
#include "TransformImage2DPlugin.hpp"

namespace alchemy {
namespace plugin {
namespace transformImage2D {

using namespace tuttle;
using namespace tuttle::plugin;

/**
 * @brief TransformImage2D process
 *
 */
class TransformImage2DProcess : public ImageOpenCVFilterProcessor
{
protected:
    TransformImage2DPlugin&    _plugin;            ///< Rendering plugin
    TransformImage2DProcessParams _params; ///< parameters

public:
    TransformImage2DProcess( TransformImage2DPlugin& effect );

	void setup( const OFX::RenderArguments& args );

    void multiThreadProcessImages( const OfxRectI& procWindowRoW );
};

}
}
}

#endif
