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

	

	

public:
	Cone();
	Cone(cv::Rect roiBounds);
	~Cone();

	bool updatePending = true;
	// input data and cone's ROI.
	cv::Mat data, newInput;
	cv::Rect roiBounds;
	// free texture if exists
	void free();
	// init texture for the passed renderer and set values for streaming
	bool initForRender(SDL_Renderer* render);
	bool lockTexture();
	bool unlockTexture();
	bool updateTexture();
	void digestInput(cv::Mat* inputData);
	bool hasDiff(cv::Mat* newData);
	void diffMatrix(cv::Mat* diff);
	// render current texture state to the given renderer
	void renderState(SDL_Renderer* render);
};
