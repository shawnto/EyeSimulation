#include "rod.h"


Rod::Rod(int texHeight, int texWidth) {
	height = texHeight;
	width = texWidth;
	texture = NULL;
	pixels = NULL;
	pitch = NULL;
	data = NULL;
}

Rod::~Rod() {
	free();
}


void Rod::free() {
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
	}
}

bool Rod::startProcessing(SDL_Renderer* render) {

	texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_BGR24, SDL_TEXTUREACCESS_STREAMING, width, height);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	if (texture == NULL) {
		return false;
	}
	return true;
}


bool Rod::lockTexture() {
	SDL_LockTexture(texture, NULL, &pixels, &pitch);
	if (pixels == NULL) {
		return false;
	}
	return true;
}

bool Rod::unlockTexture() {
	if (pixels == NULL) {
		return false;
	}
	SDL_UnlockTexture(texture);
	pixels = NULL;
	pitch = 0;
	return true;
}

bool Rod::updateTexture() {
	if (!lockTexture()) {
		return false;
	}
	std::memcpy(pixels, data.data, data.step * data.size().height);
	return unlockTexture();
}


bool Rod::hasDiff(cv::Mat* data, int x, int y) {
	cv::Scalar mean, stdDev;
	cv::meanStdDev(*data, mean, stdDev);

	//auto point = data->at<uchar>(cv::Point(x, y));
	return stdDev[0] > 1;
}


void Rod::diffMatrix(cv::Mat* data, cv::Mat* diff) {
	
	cv::Rect shiftRect = cv::Rect(0, 0, 2, 2);
	cv::Mat roi, diffRoi;

	// TODO complete this iterating through matrix
	for (int y = 0; y < data->rows; y++) {
		if (y > 0){
			shiftRect.y = y - 1;
		}
		else {
			shiftRect.y = y;
		}
		if (y < data->rows - 2) {
			shiftRect.height = 3;
		}
		else {
			shiftRect.height = 2;
		}
		for (int x = 0; x < data->cols; x++) {
			if (x > 0){
				shiftRect.x = x - 1;
			}
			else {
				shiftRect.x = x;
			}
			if (x < data->cols - 2) {
				shiftRect.width = 3;
			}
			else {
				shiftRect.width = 2;
			}
			roi = data->operator()(shiftRect);
			auto point = diff->at<cv::Vec3b>(cv::Point(x, y));
			if (hasDiff(&roi, x, y)) {
				diff->at<cv::Vec3b>(cv::Point(x, y)) = cv::Vec3b(200, 0, 0);
			}
			else {
				diff->at<cv::Vec3b>(cv::Point(x, y)) = cv::Vec3b(0, 200, 0);

			}

		}
	}
}


void Rod::digestInput(cv::Mat inputData) {
	cv::Mat blurred, grayed;
	//inputData.copyTo(diffed);
	int kern = (blurSigma * 5) | 1;
	cv::GaussianBlur(inputData, blurred, cv::Size(kern, kern), blurSigma, blurSigma);
	cv::cvtColor(blurred, grayed, cv::COLOR_BGR2GRAY, 1);
	int from_to[] = { 0, 0, 0,1, 0,2 };
	cv::mixChannels(&grayed, 1, &blurred, 1, from_to, 3);
	// Rod shouldn't call this. Push this functionality to further down the pipeline.
	//diffMatrix(&grayed, &diffed);
	data = blurred;
}



void Rod::renderState(SDL_Renderer* render) {
	if (!updateTexture()) {
		return;
	}
    SDL_RenderCopy(render, texture, NULL, NULL);
}