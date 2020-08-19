#include "eye.h"


Eye::Eye(int visWidth, int visHeight, int x, int y){
	texture = NULL;
	pixels = NULL;
	pitch = NULL;
	
	visRangeWidth = visWidth;
	visRangeHeight = visHeight;
	this->x = x;
	this->y = y;
	viewPort = cv::Rect(x, y, visRangeWidth, visRangeHeight);
	rods = Rod(viewPort);
	cones = Cone(cv::Rect(visWidth / 4, visHeight / 4, visWidth / 4, visWidth / 4));
}

Eye::~Eye() {
	free();
}


void Eye::free() {
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
	}
	rods.free();
	cones.free();
}


bool Eye::lockTexture() {
	SDL_LockTexture(texture, NULL, &pixels, &pitch);
	if (pixels == NULL) {
		return false;
	}
	return true;
}

bool Eye::unlockTexture() {
	if (pixels == NULL) {
		return false;
	}
	SDL_UnlockTexture(texture);
	pixels = NULL;
	pitch = 0;
	return true;
}

bool Eye::updateTexture() {
	if (!lockTexture()) {
		return false;
	}
	std::memcpy(pixels, data.data, data.step * data.rows);
	return unlockTexture();
}


bool Eye::initForRendering(SDL_Renderer* render) {
	texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_BGR24, SDL_TEXTUREACCESS_STREAMING, visRangeWidth, visRangeHeight);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	if (texture == NULL) {
		return false;
	}
	return true;
}

void Eye::digestInput(cv::Mat* inData) {
	// TODO seperate these into a messaging system for cell types where eye consums rods and cones as producers
	// major [c/g]pu block that is avoidable here.
	// get viewPort data
	cv::Mat temp;
	inData->operator()(viewPort).copyTo(temp);
	updatePending = false;
	rods.digestInput(temp);
	cones.digestInput(&temp);
	if (data.data == NULL) {
		temp.copyTo(data);
	}
	cv::Mat rodTemp, coneTemp;
	if (rods.updatePending) {
		cones.data.copyTo(coneTemp);
		temp.copyTo(rodTemp);
		int from_to[] = {  0, 0, 0,1, 0,2 };
		cv::mixChannels(&rods.data, 1, &rodTemp, 1, from_to, 3);
		updatePending = true;
	}
	updatePending = cones.updatePending;
	if (cones.updatePending) {
		cones.data.copyTo(coneTemp);
		updatePending = true;
	}
	rodTemp.copyTo(data);
	coneTemp.copyTo(data(cones.roiBounds));

}


void Eye::renderState(SDL_Renderer* render) {
	SDL_Rect loc;
	loc.x = viewPort.x;
	loc.y = viewPort.y;
	loc.w = viewPort.width;
	loc.h = viewPort.height;
	SDL_RenderCopy(render, texture, NULL, NULL);
}
