/*
 * Vulkan Windowed Program
 *
 * Copyright (C) 2016, 2018 Valve Corporation
 * Copyright (C) 2016, 2018 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
Vulkan C++ Windowed Project Template
Create and destroy a Vulkan surface on an SDL window.
Modified from starter template.
*/



#include <SDL.h>
#include "SDL_image.h"
#include <iostream>
#include <vector>

int main(int argc, char *args[])
{
    // Create an SDL window 
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Could not initialize SDL." << std::endl;
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("View Window", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
    if(window == NULL) {
        std::cout << "Could not create SDL window." << std::endl;
        return 1;
    }

    SDL_Renderer* viewRender = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (viewRender == NULL) {
        SDL_DestroyWindow(window);
        std::cout << "Render creation error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

/*    SDL_Surface *testBmp = IMG_Load("./testImages/fruit-painting.bmp");
    if (testBmp == nullptr){
		SDL_DestroyRenderer(viewRender);
		SDL_DestroyWindow(window);
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	} */
    std::string p = "testImages/shapes-1.png";
    SDL_Texture* testTexture = IMG_LoadTexture(viewRender, p.c_str());

    if (testTexture == NULL) {
		SDL_DestroyRenderer(viewRender);
		SDL_DestroyWindow(window);
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
    }


#if defined(_DEBUG)
    
#endif

   // This is where most initializtion for a program should be performed
   SDL_Rect dst;
   SDL_QueryTexture(testTexture, NULL, NULL, &dst.w, &dst.h);

    // Poll for user input.
    bool stillRunning = true;
    while(stillRunning) {

        SDL_Event event;
        while(SDL_PollEvent(&event)) {

            switch(event.type) {

            case SDL_QUIT:
                stillRunning = false;
                break;

            default:
                // Do nothing.
                stillRunning = true;
            }
        }
        SDL_RenderClear(viewRender);
        SDL_RenderCopy(viewRender, testTexture, NULL, NULL);
        SDL_RenderPresent(viewRender);

        SDL_Delay(100);
    }

    // Clean up.
    SDL_DestroyTexture(testTexture);
    SDL_DestroyRenderer(viewRender);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
