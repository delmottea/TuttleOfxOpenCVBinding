#ifndef _TUTTLE_OFXTOOPENCV_POINT_HPP_
#define	_TUTTLE_OFXTOOPENCV_POINT_HPP_

#include <ofxCore.h>

#include <opencv2/opencv.hpp>

namespace tuttle {
namespace plugin {

inline cv::Point2d ofxToOpenCV( const OfxPointD& p )
{
    return cv::Point2d( p.x, p.y );
}

inline OfxPointD openCVToOfx( const cv::Point2d& p )
{
	OfxPointD r = { p.x, p.y };

	return r;
}

inline cv::Point ofxToOpenCV( const OfxPointI& p )
{
    return cv::Point( p.x, p.y );
}

inline OfxPointI openCVToOfx( const cv::Point& p )
{
	OfxPointI r = { p.x, p.y };

	return r;
}

}
}


#endif

