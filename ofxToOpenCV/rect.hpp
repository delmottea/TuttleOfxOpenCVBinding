#ifndef _TUTTLE_OFXTOOPENCV_RECT_HPP_
#define	_TUTTLE_OFXTOOPENCV_RECT_HPP_

#include <ofxCore.h>

#include <tuttle/common/system/compatibility.hpp>
#include <opencv2/opencv.hpp>

namespace tuttle {
namespace plugin {

inline cv::Rect_<double> ofxToOpenCV( const OfxRectD& r )
{
    return cv::Rect_<double>( r.x1, r.y1, r.x2-r.x1, r.y2-r.y1 );
}

inline OfxRectD openCVToOfx( const cv::Rect_<double>& r )
{
    const OfxRectD rect = { r.x, r.y, r.x+r.width, r.y+r.height };
	return rect;
}

inline cv::Rect ofxToOpenCV( const OfxRectI& r )
{
        return cv::Rect( r.x1, r.y1, r.x2-r.x1, r.y2-r.y1);
}

inline OfxRectI openCVToOfx( const cv::Rect& r )
{
    const OfxRectI rect = { r.x, r.y, r.x+r.width, r.y+r.height };
	return rect;
}

}
}


#endif

