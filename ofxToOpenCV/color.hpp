#ifndef _TUTTLE_OFXTOOPENCV_COLOR_HPP_
#define	_TUTTLE_OFXTOOPENCV_COLOR_HPP_

#include <ofxPixels.h>

#include <opencv2/opencv.hpp>

namespace tuttle {
namespace plugin {

inline cv::Scalar ofxToOpenCV( const OfxRGBAColourD& c )
{
    return cv::Scalar( c.r, c.g, c.b, c.a );
}

}
}


#endif

