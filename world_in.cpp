#include "world_in.h"

/*	WorldInput();

	
	// start input capture
	bool initInput();


	// Increment the frame buffer and return the new current frame
	cv::Mat getNextFrame();

	// return current frame without incrementing frame buffer
	cv::Mat getCurrentFrame();

private:

	cv::VideoCapture capture;
	cv::Mat worldFrame;

	*/

WorldInput::WorldInput() {
	worldFrame = NULL;

}

bool WorldInput::initInput(int frameHeight, int frameWidth) {
	capture = cv::VideoCapture::VideoCapture(0);

    if (!capture.isOpened()) {
        std::cout << "could not open" << std::endl;
        return false;
    }

    capture.set(cv::CAP_PROP_FRAME_HEIGHT, frameHeight);
    capture.set(cv::CAP_PROP_FRAME_WIDTH, frameWidth);
    capture >> worldFrame;



    if (worldFrame.empty()) {
        return false;
    }

    worldSize = cv::Size((int)capture.get(cv::CAP_PROP_FRAME_WIDTH),
        (int)capture.get(cv::CAP_PROP_FRAME_HEIGHT));

	int w = worldSize.width;
	int h = worldSize.height;

	uchar d = *worldFrame.data;
	int t = worldFrame.type();

	return true;
}


cv::Mat WorldInput::getNextFrame() {
	capture >> worldFrame;
	return worldFrame;
}

cv::Mat WorldInput::getCurrentFrame() {
	return worldFrame;
}





