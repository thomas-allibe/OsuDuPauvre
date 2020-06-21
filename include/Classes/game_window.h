#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <stdio.h>
#include <SDL2/SDL.h>

//GameWindow Attributes
typedef struct{
    SDL_Window *window;
    SDL_Renderer *renderer;
}GameWindow;

//GameWindow_ctor arguments
typedef struct{
    const char *title;
    int x; int y;
    int w; int h;
    Uint32 flags;
}WindowOptions;

typedef struct{
    int index;
    Uint32 flags;
}RendererOptions;

//GameWindow Methods
//Constructor prototype
int GameWindow_ctor(GameWindow * const me, WindowOptions *w_op, RendererOptions *r_op);
//Destructor prototype
void GameWindow_dtor(GameWindow * const me);

//GameWindow Generic Methods
/**/

#endif /* GAME_WINDOW_H */