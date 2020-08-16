#pragma once

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

class WorldInput
{
public:
	
	WorldInput();

	// Initialize capture
	bool initInput(int frameHeight, int frameWidth);

	cv::Size worldSize;

	// Increment the frame buffer and return the new current frame
	cv::Mat getNextFrame();

	// return current frame without incrementing frame buffer
	cv::Mat getCurrentFrame();

private:

	cv::VideoCapture capture;
	cv::Mat worldFrame;


	

};
