#include "cv.h"
#include "highgui.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) 
{
	//OpenCV video capture object
    cv::VideoCapture camera;
	
	//OpenCV image object
    cv::Mat image;
	
	//camera id . Associated to device number in /dev/videoX
	int cam_id; 
	
	//pixel value (4-element vector)
	cv::Scalar_<unsigned char> px_value;

	//Color para cambiar los diferentes pixeles adyacentes al centro (4-element vector)
	cv::Scalar_<unsigned char> modValue;
	modValue[0] = 0;
	modValue[1] = 0;
	modValue[2] = 255;
	modValue[3] = 0;
	
	//check user args
	switch(argc)
	{
		case 1: //no argument provided, so try /dev/video0
			cam_id = 0;  
			break; 
		case 2: //an argument is provided. Get it and set cam_id
			cam_id = atoi(argv[1]);
			break; 
		default: 
			std::cout << "Invalid number of arguments. Call program as: webcam_capture [video_device_id]. " << std::endl; 
			std::cout << "EXIT program." << std::endl; 
			break; 
	}
	
	//advertising to the user 
	std::cout << "Opening video device " << cam_id << std::endl;

    //open the video stream and make sure it's opened
    if( !camera.open(cam_id) ) 
	{
        std::cout << "Error opening the camera. May be invalid device id. EXIT program." << std::endl;
        return -1;
    }

    //capture loop. Out of user press a key
    while(1)
	{
		//Read image and check it. Blocking call up to a new image arrives from camera.
        if(!camera.read(image)) 
		{
            std::cout << "No frame" << std::endl;
            cv::waitKey();
        }

	// Se recorren las 20 columnas y filas adyacentes al punto central
	
	int valueCols = image.cols/2 - 10;
	int valueRows = image.rows/2 - 10;
	
	px_value = image.at<cv::Scalar_<unsigned char> >(image.rows/2, image.cols/2, 0);

	for(int i = valueRows; i <= valueRows + 20; i++)
	{
		for(int j = valueCols; j <= valueCols + 20; j++)
		{
			// Se mantiene el punto central sin cambiar.
			if(!(j == valueCols + 10 && i == valueRows + 10))
			{
				image.at<cv::Scalar_<unsigned char> >(i,j,0) = modValue;
			}
		}
	}
        
        //show image in a window
        cv::imshow("Output Window", image);
		
		//print image dimensions
		std::cout << "image size is: " << image.rows << "x" << image.cols << std::endl; 
		std::cout << "image depth: " << image.depth() << std::endl; 
		std::cout << "num channels: " << image.channels() << std::endl;  
		std::cout << "central pixel value (BGR): " << (int)px_value[0] << "," << (int)px_value[1] << "," << (int)px_value[2] << std::endl; 
		
		//Waits 1 millisecond to check if a key has been pressed. If so, breaks the loop. Otherwise continues.
        if(cv::waitKey(1) >= 0) break;
    }   
}
