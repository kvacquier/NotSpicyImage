#ifndef _SEGMENTRECOGNIZER_
#define _SEGMENTRECOGNIZER_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Segment.h"

class SegmentRecognizer
{
	cv::Mat source;
	Segment segment;
	bool lu();
	bool ru();
	bool mqu();
	bool mm();
	bool mqd();
	bool md();
	bool mu();
	bool rd();
public:
	SegmentRecognizer(const cv::Mat& image);
	~SegmentRecognizer();
	char recognize(const Segment& segment);
};
#endif