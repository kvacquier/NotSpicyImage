#ifndef _LABELFINDER_
#define _LABELFINDER_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "PointInt.h"
#include "Stack.h"
#include "LabelSegmenter.h"
#include "SegmentRecognizer.h"

#define MINWIDTH 4 //minimal size of plate (image:plate ratio) 1:x
#define MINHEIGHT 5
#define INTENSITY 50 //dark recognition value of 5x5 sample
#define RECTLINEX 4 // line cutter (if line is longer than rectangle width:x)
#define RECTLINEY 3 // line cutter (if line is longer than rectangle height:x)
#define BINARIZE 50 //binarization treshold
#define PADDING 10

class LabelFinder
{
	cv::Mat temp;
	cv::Mat temp2;
	cv::Mat tempout;
	cv::Mat source;
	cv::Mat outimg;
	int filter[3][3];
	int histogram[256];
	PointInt rectcoor;
	int rectwidth,rectheight;
	int minlength,minheight;

	void grayScale(cv::Mat&,cv::Mat&);
	void histEquation(cv::Mat&);
	void edges(cv::Mat&,cv::Mat&,unsigned int treshold);
	void binarize(cv::Mat&,unsigned int treshold);
	bool dark(const cv::Mat&);
	bool findRect(const cv::Mat&);
	bool findRect2(const cv::Mat&,cv::Mat&);
	void cut(PointInt p,unsigned int w,unsigned int h);
	void blur(cv::Mat&,cv::Mat&,int square);
	int trySegments();

public:	
	LabelFinder(cv::Mat&);
	~LabelFinder();
	
	void show(const cv::Mat&);
	void rectPos();
	bool findPlate(unsigned int edge);
	cv::Mat& output();

};
#endif