#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <SDL.h>
#include <numeric>
#include <queue>
/*
Roughly simulate Rod Cells of eye.

*/



class Rod {
private:
	int HEIGHT = 800;
	int WIDTH = 800;
	int blurSigma = 4;
	bool shouldRender = false;
	
	// Processes movement
	//void checkMovement();


	SDL_Texture* texture;
	// texture pixels for streaming
	void* pixels;
	// texture pitch for streaming
	int pitch;
	
	// rod viewPort, for debugging/rendering mode
	cv::Rect viewPort = cv::Rect(0, 0, this->WIDTH, this->HEIGHT);
	

	

public:
	Rod();
	Rod(cv::Rect viewBounds);
	~Rod();

	bool updatePending = true;
	// input data
	cv::Mat data, newInput;
	cv::Rect viewBounds;
	// free texture if exists
	void free();
	// init texture for the passed renderer and set values for streaming
	bool initForRender(SDL_Renderer* render);
	bool lockTexture();
	bool unlockTexture();
	bool updateTexture();
	void digestInput(cv::Mat inputData);
	// render current texture state to the given renderer
	void renderState(SDL_Renderer* render);

	bool hasDiff(cv::Mat* newData);
	void diffMatrix(cv::Mat* data, cv::Mat* diff);
};
