
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
#include "world_in.h"
#include "cleanup.h"
#include "rod.h"
#include "cone.h"
#include <string>


static int WORLD_HEIGHT = 1080;
static int WORLD_WIDTH = 1920;


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

    WorldInput worldIn = WorldInput();
    if (!worldIn.initInput(WORLD_HEIGHT, WORLD_WIDTH)) {
        return 2;
    }

    // Create an SDL window 
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Could not initialize SDL." << std::endl;
        return 1;
    }

    SDL_Window* worldWindow = SDL_CreateWindow("World View", 0, SDL_WINDOWPOS_CENTERED, WORLD_WIDTH / 2, WORLD_HEIGHT / 2, SDL_WINDOW_SHOWN);
    if (validate_creation(worldWindow, "world window")) {
        SDL_Quit();
        return 1;
    }
    SDL_Window* visionWindow = SDL_CreateWindow("Vision Window", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN);
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

    SDL_Renderer* worldRender = SDL_CreateRenderer(worldWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (validate_creation(worldRender, "world render")) {
        cleanup(visionRender, visionWindow, worldWindow);
        SDL_Quit();
        return 1;
    }

 #if defined(_DEBUG)
    
#endif   
    cv::Mat inputThisFrame;
    inputThisFrame = worldIn.getNextFrame();


    Rod rodIn = Rod();
    rodIn.startProcessing(visionRender);
    
    cv::Rect coneRoi = cv::Rect(200, 200, 200, 200);
    Cone coneIn = Cone(coneRoi);
    if (!coneIn.startProcessing(visionRender)) {
        return 3;
    }

    SDL_Texture* worldTexture = SDL_CreateTexture(worldRender, SDL_PIXELFORMAT_BGR24, SDL_TEXTUREACCESS_STREAMING, WORLD_WIDTH, WORLD_HEIGHT);
    SDL_SetTextureBlendMode(worldTexture, SDL_BLENDMODE_BLEND);
    if (validate_creation(worldTexture, "rodTexture")) {
        cleanup(visionRender, visionWindow, worldRender, worldWindow);
        SDL_Quit();
        return 1;
    }
    void* worldPixels;
    int worldPitch;
   // This is where most initializtion for a program should be performed
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
            SDL_RenderClear(worldRender);
            //SDL_LockTexture(rodTexture, NULL, &texPixels, &texPitch);
            //std::memcpy(texPixels, frameRef.data, frameRef.step*frameRef.size().height);
            //SDL_UnlockTexture(rodTexture);
            inputThisFrame = worldIn.getNextFrame();
            SDL_LockTexture(worldTexture, NULL, &worldPixels, &worldPitch);
            std::memcpy(worldPixels, inputThisFrame.data, inputThisFrame.step * inputThisFrame.rows);
            SDL_UnlockTexture(worldTexture);
            rodIn.digestInput(inputThisFrame);
            rodIn.updateTexture();
            rodIn.renderState(visionRender);
            coneIn.digestInput(&inputThisFrame);
            coneIn.updateTexture();
            coneIn.renderState(visionRender);
            SDL_RenderCopy(worldRender, worldTexture, NULL, NULL);
            renderTexture(worldTexture, worldRender, 0, 0);
            SDL_RenderPresent(worldRender);
            SDL_RenderPresent(visionRender);
            SDL_Delay(75);
        }
    }

    // Clean up.
    //rodIn.free();
    coneIn.free();
    cleanup(visionWindow, visionRender);
    SDL_Quit();
   

    return 0;
}
