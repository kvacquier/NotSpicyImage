#ifndef _LABELSEGMENTER_
#define _LABELSEGMENTER_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include "Segment.h"

#define NOISEFILTER 50 //size of filtered grain width*height
#define LINEREMAINING 5 //line filter if line width is longer than plate width:x

bool sortSegments(const Segment& lhs,const Segment& rhs);

class LabelSegmenter
{
	cv::Mat source;
	std::vector<Segment> segments;
	void noiseFilter(int grain);

public:
	LabelSegmenter(const cv::Mat& image);
	~LabelSegmenter();
	int findSegments();
	void show();
	void listSegments();
	Segment& getSegment(int pos);
};
#endif