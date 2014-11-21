#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <math.h>
#include "LabelFinder.h"


LabelFinder::LabelFinder(cv::Mat &image)
{
	this->source.create(image.rows,image.cols,CV_8UC1);
	this->temp.create(image.rows,image.cols,CV_8UC1);
	this->temp2.create(image.rows,image.cols,CV_8UC1);
	
	this->grayScale(image,this->source);
	this->temp = this->source.clone();
	this->temp2 = this->source.clone();

	this->minlength = temp.cols / MINWIDTH;		//minimal dimensions of plate
	this->minheight = temp.rows / MINHEIGHT;
	
	int tfilter[3][3] = {{1,0,-1},			//edge detect matrix
						{2,0,-2},
			   		    {1,0,-1}};

	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
		
			this->filter[i][j] =  tfilter[i][j];
}
LabelFinder::~LabelFinder()
{
}
void LabelFinder::grayScale(cv::Mat &image,cv::Mat &tmp)
{
	for(size_t i = 0;i < tmp.rows; i++)
	{
		for(size_t j = 0;j < tmp.cols; j++)
		{
			tmp.at<uchar>(i,j) = (image.at<cv::Vec3b>(i,j)[0] + image.at<cv::Vec3b>(i,j)[1] + image.at<cv::Vec3b>(i,j)[2])/3;
			
		}
	}
	image = tmp.clone();
}
void LabelFinder::histEquation(cv::Mat &image)
{
	for(int i = 0; i < 256; i++)
		this->histogram[i] = 0;
	for(int i = 0; i < image.rows; i ++)
		for(int j = 0; j < image.cols; j++)
			this->histogram[image.at<uchar>(i,j)] += 1;
	for(int i = 1; i < 256; i++)
		this->histogram[i] += this->histogram[i-1];
	for(int i = 1; i < 256; i++)
		this->histogram[i] = 256*this->histogram[i]/this->histogram[255];
	for(int i = 0; i < image.rows; i ++)
		for(int j = 0; j < image.cols; j++)
			image.at<uchar>(i,j) = this->histogram[image.at<uchar>(i,j)];
	
}
void LabelFinder::binarize(cv::Mat &image,unsigned int treshold)
{
	if(!this->dark(image))
		for(int i = 0; i < image.rows; i++)
			for(int j = 0; j < image.cols; j++)
				image.at<uchar>(i,j) = (image.at<uchar>(i,j) < treshold) ? 0 : 255;
	else
		for(int i = 0; i < image.rows; i++)
			for(int j = 0; j < image.cols; j++)
				image.at<uchar>(i,j) = (image.at<uchar>(i,j) > (220 - treshold)) ? 0 : 255;
}
bool LabelFinder::dark(const cv::Mat &image)
{
	int intensity = 0;
	for(int i = 0; i < 5; i++)
		for(int j = 0; j < 5; j++)
			intensity += image.at<uchar>(i,j + 10);
	if((intensity / 25) < INTENSITY)
		return true;

	return false;
}
void LabelFinder::blur(cv::Mat &image,cv::Mat &tmp, int square)
{
	int sum = 0;
	if(square < 0)
		square *= -1;
	if(square % 2 == 0)
		square++;
	if (square > image.rows)
		square = image.rows;
	else if (square > image.cols)
		square = image.cols;

	for(int i = square / 2; i < image.rows - square / 2; i++)
		for(int j = square / 2; j < image.cols - square / 2; j++)
		{
			for(int k = (square / 2) * -1; k <= (square / 2); k++)
				for(int l = (square / 2) * -1; l <= (square / 2); l++)
					sum += image.at<uchar>(i + k,j + l);
			sum = sum /= square*square;
			tmp.at<uchar>(i,j) = sum;
			sum = 0;
		}
		image = tmp.clone();
}
void LabelFinder::show(const cv::Mat& image)
{
	cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
    cv::imshow( "Display window", image );                   // Show our image inside it

    cv::waitKey(0);                                          // Wait for a keystroke in the window
}
void LabelFinder::edges(cv::Mat& src,cv::Mat& tmp,unsigned int treshold)
{
int colorx = 0;
int colory = 0;

	for(size_t i = 0;i < src.rows; i++)
	{
		for(size_t j = 0;j < src.cols; j++)
		{
			if((i < src.rows - 1)&&(i > 0))
				if((j < src.cols - 1)&&(j > 0))
			{
				colorx = 0;
				colory = 0;
				for(size_t k = 0; k < 3;k++)
				{
					for(size_t l = 0; l < 3; l++)
					{

						colorx += static_cast<int>(src.at<uchar>(i + k - 1,j + l - 1)) * filter[k][l];
						colory += static_cast<int>(src.at<uchar>(i + k - 1,j + l - 1)) * filter[l][k];
					}

				
				}
				tmp.at<uchar>(i,j) = (abs(colorx) > treshold) ? 255 : 0; //vertical line

				if (tmp.at<uchar>(i,j) == 0)							//horizontal line
					tmp.at<uchar>(i,j) = (abs(colory) > treshold) ? 255 : 0;
			}
			else
				tmp.at<uchar>(i,j) = 0;
			else
				tmp.at<uchar>(i,j) = 0;
		}
	}
	
	src = tmp.clone();
		
}
bool LabelFinder::findRect(const cv::Mat& src)
{
	int counter = 0;
	int i = 1,j = 3;
	int k;
	bool found = false;

	while(i < src.cols - 1 && !found)				//vertical line detection
	{
		while(j < src.rows - 2 && !found)
		{
			if(src.at<uchar>(j,i - 1) == 255||		//3 cols in the same time (line doesn't have to be straight)
				src.at<uchar>(j,i) == 255||
				src.at<uchar>(j,i + 1) == 255)

				counter++;									//if line continues, increment

			else if(src.at<uchar>(j - 1,i - 1) == 255||	//detect end of line
				src.at<uchar>(j - 1,i) == 255||
				src.at<uchar>(j - 1,i + 1) == 255)
			{

				if(counter > minheight)	//if line has minimal length
				{
					k = i;
					while (i < src.cols)						//check for horizontal line (not straight => six pixels)
						if( src.at<uchar>(j - 3,k) == 255||
							src.at<uchar>(j - 2,k) == 255||
							src.at<uchar>(j - 1,k) == 255||
							src.at<uchar>(j,k) == 255||
							src.at<uchar>(j + 1,k) == 255||
							src.at<uchar>(j + 2,k) == 255)

							if((k-i) > 10 &&							//check vertical line if earlier than end of horizontal
								src.at<uchar>(j - 3,k) == 255&&
								src.at<uchar>(j - 4,k) == 255&&
								src.at<uchar>(j - 5,k) == 255)

								break;
							else k++;
											
						else break;

					if(k - i > minlength)	//if line has minimal length, rectangle found
					{

						found = true;
						this->rectcoor.x= i;
						this->rectcoor.y = j - counter;
						this->rectwidth = k - i - 1;
						this->rectheight = counter;

					}
				}
				counter = 0;
			}

			else counter = 0;
			j++;
		}
		i++;
		j = 3;
	}

	counter = this->rectheight / RECTLINEY + 1;						
	j = this->rectcoor.x - 1;
	while(counter > this->rectheight / RECTLINEY)								//cutting borders - left
	{
		j++;
		counter = 0;
		for(int i = 0; i < this->rectheight; i++)
			if(src.at<uchar>(i + this->rectcoor.y,j) == 255)
			
				counter++;
				

	}
	if (j > this->rectcoor.x)
	{		
		this->rectwidth -= j - this->rectcoor.x;
		this->rectcoor.x = j;
	}

	counter = this->rectheight / RECTLINEY + 1;						
	j = this->rectcoor.x + this->rectwidth + 1;
	while(counter > this->rectheight / RECTLINEY)								//cutting borders - right
	{
		j--;
		counter = 0;
		for(int i = 0; i < this->rectheight; i++)
			if(src.at<uchar>(i + this->rectcoor.y,j) == 255)
			
				counter++;
				

	}
	if (j < (this->rectcoor.x + this->rectwidth))
	{
		this->rectwidth -= (this->rectcoor.x + this->rectwidth) - j;
	}

	counter = this->rectwidth / RECTLINEX + 1;						
	j = this->rectcoor.y - 1;
	while(counter > this->rectwidth / RECTLINEX)								//cutting borders - top
	{
		j++;
		counter = 0;
		for(int i = 0; i < this->rectwidth; i++)
			if(src.at<uchar>(j,i + this->rectcoor.x) == 255)
			
				counter++;
				

	}
	if (j > this->rectcoor.y)
	{		
		this->rectheight -=  j - this->rectcoor.y;
		this->rectcoor.y =  j;
	}


	counter = this->rectwidth / RECTLINEX + 1;						
	j = this->rectcoor.y + this->rectheight + 1;
	while(counter > this->rectwidth / RECTLINEX)								//cutting borders - bottom
	{
		j--;
		counter = 0;
		for(int i = 0; i < this->rectwidth; i++)
			if(src.at<uchar>(j,i + this->rectcoor.x) == 255)
			
				counter++;
				

	}
	if (j < (this->rectcoor.y + this->rectheight))
	{		
		this->rectheight -=  (this->rectcoor.y + this->rectheight) - j;
	}



	return found;		
}
void LabelFinder::cut(PointInt p,unsigned int w,unsigned int h)
{
	this->outimg.create(h,w,CV_8UC1);

	for(size_t i = 0; i < outimg.rows; i++)
		for(size_t j = 0; j < outimg.cols; j++)
			this->outimg.at<uchar>(i,j) = this->source.at<uchar>(i + p.y,j + p.x);
}
bool LabelFinder::findPlate(unsigned int edge)
{
	this->edges(this->temp,this->temp2,edge);
	this->show(this->temp);
	this->findRect2(this->temp,this->temp2);
	this->show(this->temp2);
	//this->show(this->temp);
	/*if (!this->findRect(this->temp))
		return false;
	this->cut(this->rectcoor,this->rectwidth,this->rectheight);
	this->show(this->outimg);
	this->histEquation(this->outimg);
	this->show(this->outimg);
	this->binarize(this->outimg,BINARIZE);
	this->show(this->outimg);*/

	return true;
}
cv::Mat& LabelFinder::output()
{
	return this->outimg;
}
void LabelFinder::rectPos()
{
	std::cout << this->rectcoor.x << std::endl;
	std::cout << this->rectcoor.y << std::endl;
	std::cout << this->rectheight << std::endl;
	std::cout << this->rectwidth << std::endl;
};
bool LabelFinder::findRect2(const cv::Mat& src, cv::Mat& tmp)
{
	int sum = 0;
	int max = 0;

	for(int i = 1; i < src.rows; i++)
	{
		for(int j = 1; j < src.cols; j++)
			if(abs(static_cast<int>(src.at<uchar>(i,j)) - static_cast<int>(src.at<uchar>(i,j-1))) != 0)
				sum += 1;
			tmp.at<uchar>(i,0) = (sum > 255)? 255:sum;
		if(sum > max)
			max = (sum > 255)? 255 : sum;
		sum = 0;
	}

	for(int i = 1; i < tmp.rows; i++)					//filter
		if(static_cast<int>(tmp.at<uchar>(i,0)) < max/2)
				tmp.at<uchar>(i,0) = 0;

	max = 0;
	for(int i = 1; i < src.cols; i++)
	{
		for(int j = 1; j < src.rows; j++)
			if(abs(static_cast<int>(src.at<uchar>(j,i)) - static_cast<int>(src.at<uchar>(j-1,i))) != 0)
				sum += 1;
		
			tmp.at<uchar>(0,i) = (sum > 255)? 255 : sum;
		if(sum > max)
			max = (sum > 255)? 255 : sum;		
		sum = 0;
	}
	
	for(int i = 1; i < tmp.cols; i++) //filter
			if(static_cast<int>(tmp.at<uchar>(0,i)) < max/2)
				tmp.at<uchar>(0,i) = 0;
	max = 0;
	for(int i = 1; i < tmp.rows; i++)
		for(int j = 1; j < tmp.cols; j++)
		{
			tmp.at<uchar>(i,j) = (tmp.at<uchar>(i,0) + tmp.at<uchar>(0,j) > 255)? 255 :tmp.at<uchar>(i,0) + tmp.at<uchar>(0,j);
			if(tmp.at<uchar>(i,j) > max)
				max = (tmp.at<uchar>(i,j) > 255) ? 255 : tmp.at<uchar>(i,j);
		}
	show(tmp);	
		this->histEquation(tmp);
	for(int i = 1; i < tmp.rows; i++)
		for(int j = 1; j < tmp.cols; j++)
			if(static_cast<int>(tmp.at<uchar>(i,j)) < 253)
				tmp.at<uchar>(i,j) = 0;
   show(tmp);
	sum = 0;
	max = (tmp.rows / 10)*(tmp.cols / 8);
	for(int i = 0; i < 10; i++)
		for(int j = 0; j < 8; j++)
		{
			for(int k = 0; k < (tmp.rows / 10);k++)
				for(int l = 0; l < (tmp.cols / 8);l++)
					if(tmp.at<uchar>(k+(i*(tmp.rows / 10)),l+(j*(tmp.cols / 8))) > 253)
						sum++;

			if(sum > (max * 0.05))
				for(int k = 0; k < (tmp.rows / 10);k++)
					for(int l = 0; l < (tmp.cols / 8);l++)
						tmp.at<uchar>(k+(i*(tmp.rows / 10)),l+(j*(tmp.cols / 8))) = 255;
			else
				for(int k = 0; k < (tmp.rows / 10);k++)
					for(int l = 0; l < (tmp.cols / 8);l++)
						tmp.at<uchar>(k+(i*(tmp.rows / 10)),l+(j*(tmp.cols / 8))) = 0;
			sum = 0;

		}
			
	return false;
}