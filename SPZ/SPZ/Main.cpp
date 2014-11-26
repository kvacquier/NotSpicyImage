#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include "LabelFinder.h"


int main( int argc, char** argv )
{
    if( argc != 2)
    {
     std::cout <<" Usage: display_image ImageToLoadAndDisplay" << std::endl;
     return -1;
    }

    cv::Mat image;
    image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        std::cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

	cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
    cv::imshow( "Display window", image );                   // Show our image inside it.

    cv::waitKey(0);                                          // Wait for a keystroke in the window

	std::cout << "Image loaded" << std::endl;

	LabelFinder label(image);
	
	if (label.findPlate(400))	//400 edge intensity
	
		std::cout << "Licence plate detected" << std::endl;
		//label.rectPos();

	else
		std::cout << "Licence plate not detected" << std::endl;
	
    return 0;
}