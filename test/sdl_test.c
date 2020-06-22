#include "SDL2/SDL.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    SDL_Rect rectangle;

    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_GetDisplayUsableBounds(0, &rectangle);
    printf("w = %d\nh = %d\nx = %d\ny = %d\n", rectangle.w, rectangle.h, rectangle.x, rectangle.y);

    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        return 1;
    }
    printf("w = %d\nh = %d\nf = %d\n", dm.w, dm.h, dm.refresh_rate);


    return 0;
}