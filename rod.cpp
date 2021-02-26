#include "rod.h"


Rod::Rod(){
	texture = NULL;
	pixels = NULL;
	pitch = NULL;
}

Rod::Rod(cv::Rect viewBounds){
	texture = NULL;
	pixels = NULL;
	pitch = NULL;
	this->viewBounds = viewBounds;
}

Rod::~Rod() {
	free();
}


void Rod::free() {
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
	}
}

bool Rod::initForRender(SDL_Renderer* render) {

	texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_BGR24, SDL_TEXTUREACCESS_STREAMING, this->WIDTH, this->HEIGHT);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	if (texture == NULL) {
		return false;
	}
	shouldRender = true;
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
	std::memcpy(pixels, data.data, data.step * data.rows);
	return unlockTexture();
}


bool Rod::hasDiff(cv::Mat* newData){
	cv::Mat tmp = data - *newData;
	//auto point = data->at<uchar>(cv::Point(x, y));
	cv::absdiff(data, *newData, tmp);
	return tmp.empty() == false;
}


void Rod::diffMatrix(cv::Mat* data, cv::Mat* diff) {
	cv::absdiff(*diff, *data, newInput);
	diff->copyTo(*data);
}


void Rod::digestInput(cv::Mat inputData) {
	cv::Mat temp, blurred;
	int kern = (blurSigma * 5) | 1;
	//inputData(viewPort).copyTo(temp);
	cv::cvtColor(inputData, temp, cv::COLOR_BGR2GRAY);
	cv::GaussianBlur(temp, blurred, cv::Size(kern, kern), blurSigma, blurSigma);
	if (data.data == NULL) {
		blurred.copyTo(data);
	}
	if (hasDiff(&blurred)) {
		updatePending = true;
		diffMatrix(&data, &blurred);
	}
	else {
		updatePending = false;
	}
	if (shouldRender) {
		int from_to[] = { 0, 0, 0,1, 0,2 };
		cv::mixChannels(&data, 1, &data, 1, from_to, 3);
	}
}



void Rod::renderState(SDL_Renderer* render) {
	/*
	Debugging tool to render how the current function processes info
	*/
	if (!updateTexture()) {
		return;
	}
	SDL_Rect tmp;
	tmp.x = viewPort.x;
	tmp.y = viewPort.y;
	tmp.w = viewPort.width;
	tmp.h = viewPort.height;
    SDL_RenderCopy(render, texture, NULL, &tmp);
}