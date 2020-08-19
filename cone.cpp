#include "cone.h"

Cone::Cone() {
	texture = NULL;
	pixels = NULL;
	pitch = NULL;
	this->roiBounds = cv::Rect();
}

Cone::Cone(cv::Rect roiBounds){
	texture = NULL;
	pixels = NULL;
	pitch = NULL;
	this->roiBounds = roiBounds;
}

Cone::~Cone() {
	free();
}


void Cone::free() {
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
	}
}

bool Cone::initForRender(SDL_Renderer* render) {

	texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_BGR24, SDL_TEXTUREACCESS_STREAMING, roiBounds.width, roiBounds.height);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	if (texture == NULL) {
		return false;
	}
	return true;
}


bool Cone::lockTexture() {
	SDL_LockTexture(texture, NULL, &pixels, &pitch);
	if (pixels == NULL) {
		return false;
	}
	return true;
}

bool Cone::unlockTexture() {
	if (pixels == NULL) {
		return false;
	}
	SDL_UnlockTexture(texture);
	pixels = NULL;
	pitch = 0;
	return true;
}

bool Cone::updateTexture() {
	if (!lockTexture()) {
		return false;
	}
	std::memcpy(pixels, data.data, data.step * data.rows);
	return unlockTexture();
}

bool Cone::hasDiff(cv::Mat* newData){
	cv::Mat tmp;
	//auto point = data->at<uchar>(cv::Point(x, y));
	cv::absdiff(data, *newData, tmp);
	return tmp.empty() == false;
}


void Cone::diffMatrix(cv::Mat* diff) {
	cv::absdiff(*diff, data, newInput);
	diff->copyTo(data);
}


void Cone::digestInput(cv::Mat* inputData) {
	/*
	Cone is a smaller but more detailed ROI of world. Get ROI, process data from that.
	*/
	cv::Mat sample = inputData->operator()(roiBounds);
	if (data.data == NULL) {
		sample.copyTo(data);
		updatePending = true;
	}
	else {

		updatePending = hasDiff(&sample);
		if(updatePending) {
			diffMatrix(&sample);
		}
	}
}


void Cone::renderState(SDL_Renderer* render) {
	if (!updateTexture()) {
		return;
	}
	SDL_Rect dst;
	dst.x = roiBounds.x;
	dst.y = roiBounds.y;
	dst.w = roiBounds.width;
	dst.h = roiBounds.height;
	SDL_RenderCopy(render, texture, NULL, &dst);
}