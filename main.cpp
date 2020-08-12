
/*
Create and destroy a surface on an SDL window.
Modified from starter template.
*/



#include <SDL.h>
#include "SDL_image.h"
#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include "render_utils.h"
#include "cleanup.h"
#include <string>


template <typename T>
int validate_creation(T* obj, std::string identifier){
    if (obj == NULL) {
        std::cout << "Could not initialize " << identifier << std::endl;
        return 1;
    }
    return 0;

}


int main(int argc, char *args[])
{

    cv::VideoCapture captRefrnc(0, cv::CAP_DSHOW);

    if (!captRefrnc.isOpened()) {
        std::cout << "could not open" << std::endl;
        return 1;
    }
    cv::Mat frameRef;

    captRefrnc >> frameRef;

    if (frameRef.empty()) {
        return 1;
    }

    cv::Size worldSize = cv::Size((int)captRefrnc.get(cv::CAP_PROP_FRAME_WIDTH),
        (int)captRefrnc.get(cv::CAP_PROP_FRAME_HEIGHT));
    std::cout << "starting.." << std::endl;
    captRefrnc.set(cv::CAP_PROP_CONVERT_RGB, 1);
    std::cout << frameRef.data << std::endl;
    // Create an SDL window 
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Could not initialize SDL." << std::endl;
        return 1;
    }

    SDL_Window* visionWindow = SDL_CreateWindow("Vision Window", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
    if (validate_creation(visionWindow, "vision window")) {
        SDL_Quit();
        return 1;
    }


    SDL_Renderer* visionRender = SDL_CreateRenderer(visionWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (visionRender == NULL) {
        cleanup(visionWindow);
        std::cout << "Render creation error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    //std::string p = "testImages/shapes-1.png";
   // SDL_Texture* testTexture = IMG_LoadTexture(viewRender, p.c_str());

    SDL_Texture* rodTexture = SDL_CreateTexture(visionRender, SDL_PIXELFORMAT_BGR24, SDL_TEXTUREACCESS_STREAMING, worldSize.width, worldSize.height);
    SDL_SetTextureBlendMode(rodTexture, SDL_BLENDMODE_BLEND);
    if (validate_creation(rodTexture, "rodTexture")) {
        cleanup(visionRender, visionWindow);
        SDL_Quit();
        return 1;
    }




#if defined(_DEBUG)
    
#endif

   // This is where most initializtion for a program should be performed
   //SDL_Rect dst;
   //SDL_QueryTexture(testTexture, NULL, NULL, &dst.w, &dst.h);
    void* texPixels = NULL;
    int texPitch = NULL;
    // Poll for user input.
    bool stillRunning = true;
    while(stillRunning) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            switch (event.type) {
#pragma once

            case SDL_QUIT:
                stillRunning = false;
                break;

            default:
                // Do nothing.
                stillRunning = true;
            }
            SDL_RenderClear(visionRender);
            captRefrnc >> frameRef;
            SDL_LockTexture(rodTexture, NULL, &texPixels, &texPitch);
            std::memcpy(texPixels, frameRef.data, frameRef.elemSize() * frameRef.size().height * frameRef.size().width);
            SDL_UnlockTexture(rodTexture);
            renderTexture(rodTexture, visionRender, 0, 0);
            SDL_RenderPresent(visionRender);

            SDL_Delay(50);
        }
    }

    // Clean up.
    cleanup(visionWindow, visionRender, rodTexture);
    SDL_Quit();
   

    return 0;
}
