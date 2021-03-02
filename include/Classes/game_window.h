#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#define GW_ERR_PRINT 0  //0: no fprintf of SDL_GetError()

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "global_variables.h"

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

/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/

/**
  @brief    Create an instance of GameWindow class
  @param    w_op   struct for window settings  
  @param    r_op   struct for renderer settings  
  @return   Pointer to the GameWindow instance. NULL if failed.
            Use GameWindow_dtor to destroy it.

 */
GameWindow* GameWindow_ctor(WindowOptions *w_op, RendererOptions *r_op);

/**
  @brief    Destroy a GameWindow instance
  @param    me     pointer to the GameWindow to free
  @return   void

 */
void GameWindow_dtor(GameWindow *me);

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/

/**
  @brief    Change the icon of the GameWindow instance
  @param    me     pointer to the GameWindow
  @param    path   path to the icon
  @return   0 if succeeded, if failed

 */
int GameWindow_setIcon(GameWindow *me, const char *path);

int GameWindow_setFullscreen(GameWindow *me);

int GameWindow_setWindowed(GameWindow *me);

#endif /* GAME_WINDOW_H */