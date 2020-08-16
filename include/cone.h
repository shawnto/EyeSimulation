#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <SDL.h>
#include <numeric>
#include <queue>
/*
Roughly simulate Rod Cells of eye.

Can probably be abstracted with the Rod class, but explicitly defining for now.

*/

class Cone {
private:


	SDL_Texture* texture;
	// texture pixels for streaming
	void* pixels;
	// texture pitch for streaming
	int pitch;
	// input data and cone's ROI.
	cv::Mat data;
	cv::Rect roiBounds;
	

	

public:
	Cone(cv::Rect roiBounds);
	~Cone();

	int height, width;
	
	// free texture if exists
	void free();
	// init texture for the passed renderer and set values for streaming
	bool startProcessing(SDL_Renderer* render);
	bool lockTexture();
	bool unlockTexture();
	bool updateTexture();
	void digestInput(cv::Mat* inputData);
	// render current texture state to the given renderer
	void renderState(SDL_Renderer* render);
};
