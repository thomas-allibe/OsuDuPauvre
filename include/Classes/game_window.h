#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#define GW_ERR_PRINT 0  //0: no fprintf of SDL_GetError()

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

/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/

//Constructor prototype
/*-------------------------------------------------------------------------*/
/**
  @brief    Create an instance of GameWindow class
  @param    me     pointer for the new GameWindow
  @param    w_op   struct for window settings  
  @param    r_op   struct for renderer settings  
  @return   0 if succeeded, less than 0 if failed.

 */
/*--------------------------------------------------------------------------*/
int GameWindow_ctor(GameWindow * const me, WindowOptions *w_op, RendererOptions *r_op);
//Destructor prototype
/*-------------------------------------------------------------------------*/
/**
  @brief    Destroy a GameWindow instance
  @param    me     pointer to the GameWindow to free
  @return   void

 */
/*--------------------------------------------------------------------------*/
void GameWindow_dtor(GameWindow * const me);

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/

/*-------------------------------------------------------------------------*/
/**
  @brief    Change the icon of the GameWindow instance
  @param    me     pointer to the GameWindow
  @param    path   path to the icon
  @return   0 if succeeded, if failed

 */
/*--------------------------------------------------------------------------*/
int GameWindow_setIcon(GameWindow * const me, const char *path);

#endif /* GAME_WINDOW_H */