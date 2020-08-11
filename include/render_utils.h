#ifndef RENDER_UTILS_H
#include <SDL.h>

/*
Basic render functions from:
https://github.com/Twinklebear/TwinklebearDev-Lessons/blob/master/Lesson6/src/main.cpp

Expect to change quite a bit in the future.
*/

void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, SDL_Rect dst, SDL_Rect* clip = nullptr);

void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, SDL_Rect* clip = nullptr);

#endif // !RENDER_UTILS_H

