#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <SDL.h>
#include <numeric>
#include <queue>
/*
Roughly simulate Rod Cells of eye.

Citations:
Grayscale formula from:
http://www.realtimerendering.com/


*/



class Rod {
private:
	int HEIGHT = 800;
	int WIDTH = 800;
	int blurSigma = 4;

	// Processes movement
	//void checkMovement();


	SDL_Texture* texture;
	// texture pixels for streaming
	void* pixels;
	// texture pitch for streaming
	int pitch;
	// input data
	cv::Mat data;

	// rod viewPort
	cv::Rect viewPort = cv::Rect(0, 0, this->WIDTH, this->HEIGHT);
	

	

public:
	Rod();
	~Rod();

	int height, width;
	
	// free texture if exists
	void free();
	// init texture for the passed renderer and set values for streaming
	bool startProcessing(SDL_Renderer* render);
	bool lockTexture();
	bool unlockTexture();
	bool updateTexture();
	void digestInput(cv::Mat inputData);
	// render current texture state to the given renderer
	void renderState(SDL_Renderer* render);

	bool Rod::hasDiff(cv::Mat* data, int x, int y);
	void Rod::diffMatrix(cv::Mat* data, cv::Mat* diff);
};
