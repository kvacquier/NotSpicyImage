#include "SegmentRecognizer.h"
#include "Segment.h"


SegmentRecognizer::SegmentRecognizer(const cv::Mat& image)
{
	this->source.create(image.rows,image.cols,CV_8UC1);
	this->source = image.clone();
}
SegmentRecognizer::~SegmentRecognizer()
{
}
char SegmentRecognizer::recognize(const Segment& segment)
{
	this->segment = segment;

	if(this->mqu() && !this->mm() && !this->mqd() && this->ru() && !this->mu() && this->rd()) //lu,md
		return '1';
	if(!this->mqu() && !this->mm() && !this->mqd() && this->ru() && this->lu() && this->md() && this->mu() &this->rd())
			return '0';
	if(!this->mqu() && !this->mm() && this->mqd() && !this->rd())
		if(this->ru() && !this->lu() && !this->md() && this->mu())
			return '7';
		else if(this->ru() && this->md() && !this->lu() && this->mu())
			return '2';
	if(!this->mqu() && this->mm() && this->mqd() && this->ru() && this->lu() && !this->md() && this->mu() && !this->rd())
			return '9';

	if(this->mm() && this->mqd() && this->ru() && this->lu() && this->mqu() && !this->mu())//md
			return '4';
	if(this->mm() && !this->mqd() && this->md() && this->rd())
		if(!this->mu() && this->mqu()) //ru,lu
			return '6';		
	    else if(this->mu() && !this->lu())//mqu,ru
			return '3';
	if(!this->mqu() && this->mm() && !this->mqd() && this->mu() && this->md() && this->rd())
		if(!this->ru() && this->lu())
			return '5';
	    else if(this->ru() && this->lu())
			return '8';
		



	return ' ';
}
bool SegmentRecognizer::lu()
{
	if(this->source.at<uchar>(this->segment.y + this->segment.height / 3,this->segment.x) < 255 ||
		this->source.at<uchar>(this->segment.y + this->segment.height / 3,this->segment.x + 1) < 255 ||
		this->source.at<uchar>(this->segment.y + this->segment.height / 3,this->segment.x + 2) < 255 ||
		this->source.at<uchar>(this->segment.y + this->segment.height / 3,this->segment.x + 3) < 255 ||
		this->source.at<uchar>(this->segment.y + this->segment.height / 3,this->segment.x + 4) < 255)
		return true;
	return false;
}
bool SegmentRecognizer::ru()
{
	if(this->source.at<uchar>(this->segment.y + this->segment.height / 3,this->segment.x + this->segment.width) < 255  ||
		this->source.at<uchar>(this->segment.y + this->segment.height / 3,this->segment.x + this->segment.width - 1) < 255  ||
		this->source.at<uchar>(this->segment.y + this->segment.height / 3,this->segment.x + this->segment.width - 2) < 255 ||
		this->source.at<uchar>(this->segment.y + this->segment.height / 3,this->segment.x + this->segment.width - 3) < 255 ||
		this->source.at<uchar>(this->segment.y + this->segment.height / 3,this->segment.x + this->segment.width - 4) < 255) 
		return true;
	return false;
}
bool SegmentRecognizer::md()
{
	if(this->source.at<uchar>(this->segment.y + this->segment.height,this->segment.x + 2 * this->segment.width / 3) < 255 ||
		this->source.at<uchar>(this->segment.y + this->segment.height - 1,this->segment.x + 2 * this->segment.width / 3) < 255) 
		return true;
	return false;
}
bool SegmentRecognizer::mm()
{
	if(this->source.at<uchar>(this->segment.y + this->segment.height / 2,this->segment.x + this->segment.width / 2) < 255 ||
		this->source.at<uchar>(this->segment.y + this->segment.height / 2 - 1,this->segment.x + this->segment.width / 2) < 255 ||
		this->source.at<uchar>(this->segment.y + this->segment.height / 2 - 2,this->segment.x + this->segment.width / 2) < 255 ||
		this->source.at<uchar>(this->segment.y + this->segment.height / 2 - 3,this->segment.x + this->segment.width / 2) < 255 ||
		this->source.at<uchar>(this->segment.y + this->segment.height / 2 + 1,this->segment.x + this->segment.width / 2) < 255)
		return true;
	return false;
}
bool SegmentRecognizer::mqu()
{
	if(this->source.at<uchar>(this->segment.y + this->segment.height / 4,this->segment.x + this->segment.width / 2) < 255 ||
		this->source.at<uchar>(this->segment.y + this->segment.height / 4 - 1,this->segment.x + this->segment.width / 2) < 255 ||
		this->source.at<uchar>(this->segment.y + this->segment.height / 4 + 1,this->segment.x + this->segment.width / 2) < 255)
		return true;
	return false;
}
bool SegmentRecognizer::mqd()
{
	if(this->source.at<uchar>(this->segment.y + 3 * this->segment.height / 4,this->segment.x + this->segment.width / 2) < 255 ||		
		this->source.at<uchar>(this->segment.y + 3 * this->segment.height / 4 - 1,this->segment.x + this->segment.width / 2) < 255 ||
		this->source.at<uchar>(this->segment.y + 3 * this->segment.height / 4 - 2,this->segment.x + this->segment.width / 2) < 255 ||
		this->source.at<uchar>(this->segment.y + 3 * this->segment.height / 4 + 1,this->segment.x + this->segment.width / 2) < 255||
		this->source.at<uchar>(this->segment.y + 3 * this->segment.height / 4 + 2,this->segment.x + this->segment.width / 2) < 255)
		return true;
	return false;
}
bool SegmentRecognizer::mu()
{
	if(this->source.at<uchar>(this->segment.y,this->segment.x + this->segment.width / 3) < 255 ||
		this->source.at<uchar>(this->segment.y + 1,this->segment.x + this->segment.width / 3) < 255 )
		return true;
	return false;
}
bool SegmentRecognizer::rd()
{
	if(this->source.at<uchar>(this->segment.y + 2 * this->segment.height / 3,this->segment.x + this->segment.width) < 255  ||
		this->source.at<uchar>(this->segment.y + 2 * this->segment.height / 3,this->segment.x + this->segment.width - 1) < 255) 
		return true;
	return false;
}
