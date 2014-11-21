#include "LabelSegmenter.h"
#include "Stack.h"
#include "PointInt.h"
#include "Segment.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

LabelSegmenter::LabelSegmenter(const cv::Mat& image)
{
	this->source.create(image.rows,image.cols,CV_8UC1);
	this->source = image.clone();
}
LabelSegmenter::~LabelSegmenter()
{

}
int LabelSegmenter::findSegments()
{
	Stack stack;
	PointInt point;
	Segment segment;
	
	for(int i = 0; i < this->source.rows; i++){
		for(int j = 0; j < this->source.cols; j++){

			if(this->source.at<uchar>(i,j) == 0)
			{
				segment.setSegment(1,1,j,i);
				point = PointInt(j,i);
				stack.push(point);
			

				while(!stack.isEmpty())
				{

					if((point.x+1)<this->source.cols)
					{
						if(this->source.at<uchar>(point.y,point.x+1) == 0)
						{
							stack.push(PointInt(point.x+1,point.y));
						}


					}
					if((point.x-1)>=0)
					{
						if(this->source.at<uchar>(point.y,point.x - 1) == 0)
						{
							stack.push(PointInt(point.x-1,point.y));
						}


					}
					if((point.y+1)<this->source.rows)
					{
						if(this->source.at<uchar>(point.y+1,point.x) == 0)
						{
							stack.push(PointInt(point.x,point.y+1));
						}


					}
					if((point.y-1)>=0)
					{
						if(this->source.at<uchar>(point.y-1,point.x) == 0)
						{
							stack.push(PointInt(point.x,point.y-1));
						}


					}
					if (point.x < segment.x)
						segment.x = point.x;
					if (point.y < segment.y)
						segment.y = point.y;
					if (point.x - segment.x > segment.width)
						segment.width = point.x - segment.x;
					if (point.y - segment.y > segment.height)
						segment.height = point.y - segment.y;
					point = stack.pop();
					this->source.at<uchar>(point.y,point.x) = this->segments.size() + 1;
				}
				this->segments.push_back(segment);
			}
		}
	}
	//std::cout << segments.size() << std::endl;
	this->noiseFilter(NOISEFILTER);
	//std::cout << "Noise deleted" << std::endl;
	//std::cout << segments.size() << std::endl;
	std::sort(this->segments.begin(),this->segments.end(),sortSegments);
	return this->segments.size();
	
};
void LabelSegmenter::show()
{
	cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
    cv::imshow( "Display window", this->source );                   // Show our image inside it

    cv::waitKey(0);                                          // Wait for a keystroke in the window
}
void LabelSegmenter::listSegments()
{
	for(int i = 0; i < this->segments.size(); i++)
	{
		std::cout << "Segment " << i+1 << std::endl;
		std::cout << this->segments[i].x << std::endl;
		std::cout << this->segments[i].y << std::endl;
		std::cout << this->segments[i].height << std::endl;
		std::cout << this->segments[i].width << std::endl << std::endl;


	}
}
void LabelSegmenter::noiseFilter(int grain)
{
	int i = 0;	
	while (i < segments.size())
	{
		if((this->segments[i].width*this->segments[i].height) < grain) //small elements
			this->segments.erase(this->segments.begin() + i);
		else if((this->segments[i].width > this->source.cols / LINEREMAINING)) //line remainings
			this->segments.erase(this->segments.begin() + i);
		else i++;
	}
}
Segment& LabelSegmenter::getSegment(int pos)
{
		if(pos > 0 && pos < this->segments.size() + 1)	
		{
			return this->segments[pos-1];
		}
}; //return problem
bool sortSegments(const Segment& lhs,const Segment& rhs)
{
	if(lhs.y > rhs.y + 15)
		if(lhs.x < rhs.x)
			return lhs.x > rhs.x;
		else
			return lhs.x < rhs.x;
	else	
		return lhs.x < rhs.x;
}