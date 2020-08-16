#include "cone.h"


Cone::Cone(cv::Rect roiBounds){
	texture = NULL;
	pixels = NULL;
	pitch = NULL;
	data = NULL;
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

bool Cone::startProcessing(SDL_Renderer* render) {

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
	std::memcpy(pixels, data.data, data.step * data.cols);
	return unlockTexture();
}


void Cone::digestInput(cv::Mat* inputData) {
	/*
	Cone is a smaller but more detailed ROI of world. Get ROI, set texture for render from that.
	*/
	
	
	auto tmp = inputData->operator()(roiBounds);
	tmp.copyTo(data);
	
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