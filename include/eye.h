#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/hal/interface.h>
#include <SDL.h>
#include "rod.h"
#include "cone.h"

class Eye {
	/*
	Roughly simulate a human eye. Expect the final perception of visuals to continue to bubble up
	to a seperate processing module. But, can set DEBUG=true to get a frame to frame render of the eye
	input.

	It is too computationally expensive to simulate over 100m rod cells and 6-7m cones. However,
	we can see from surrounding literature that the primary need of that scale is to collect information
	inherent to digitized images. Current approach is to extract relevant functionality to pass on to the 
	perception "engine."

	Rods and cones: track a section/slice of the input and send novel/relevant/new information down the pipeline
	Cones:  capture detail in the region of interest or focal point.
	Rods: capture lumeninence and new information in and around the ROI/FP.

	*/

	public:
		Eye(int visWidth, int visHeight, int x, int y);
		~Eye();
		cv::Rect viewPort;
		int visRangeWidth, visRangeHeight, x, y;
		bool updatePending = true;
		Rod rods;
		Cone cones;
		cv::Mat data, cellState;
		// sync rod and cone data
		void digestInput(cv::Mat* inputData);
		// if debugging, push output to the set renderer
		void renderState(SDL_Renderer* render);
		bool lockTexture();
		bool unlockTexture();
		bool updateTexture();
		void free();
		bool initForRendering(SDL_Renderer* render);
	private:
		SDL_Texture* texture;
		void* pixels;
		int pitch;
		
		


};
