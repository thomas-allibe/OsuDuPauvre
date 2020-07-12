#ifndef GAME_COMPONENT_H
#define GAME_COMPONENT_H

#define GC_ERR_PRINT 0 //0: no fprintf of SDL_GetError()

#include <SDL2/SDL.h>
#include <stdio.h>

/****************************************************************************
 *    PUBLIC TYPES
 ***************************************************************************/
//GameWindow Attributes
typedef struct{
    //SDL_bool is_source; //This GC has allocated a SDL_Texture ?
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Rect position;
}GameComponent;

/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/
//Constructor prototypes
/*-------------------------------------------------------------------------*/
/**
  @brief    Create an instance of GameComponent class
  @param    me     pointer for the new GameComponent
  @param    r      pointer to the renderer
  @param    t      pointer to the texture for this class
  @return   0 if succeeded, less than 0 if failed.

 */
/*--------------------------------------------------------------------------*/
int GameComponent_ctor(GameComponent * const me, SDL_Renderer *r, SDL_Texture *t);

//Destructor prototype
/*-------------------------------------------------------------------------*/
/**
  @brief    Destroy a GameComponent instance
  @param    me     pointer to the GameComponent to free
  @return   void

 */
/*--------------------------------------------------------------------------*/
void GameComponent_dtor(GameComponent * const me);

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/

/*-------------------------------------------------------------------------*/
/**
  @brief    Copy the texture on the renderer.
  @param    me     pointer to GameComponent
  @return   0 if succeeded, less than 0 if failed.

 */
/*--------------------------------------------------------------------------*/
int GameComponent_renderCopy(GameComponent * const me);

/*-------------------------------------------------------------------------*/
/**
  @brief    Set width & height according to original
  @param    me     pointer to GameComponent
  @return   0 if succeeded, less than 0 if failed

 */
/*--------------------------------------------------------------------------*/
int GameComponent_set_default_WH(GameComponent *me);

/*-------------------------------------------------------------------------*/
/**
  @brief    Set new x & y position
  @param    me     pointer to GameComponent
  @param    x      x position in pixels
  @param    y      y position in pixels
  @return   void

 */
/*--------------------------------------------------------------------------*/
void GameComponent_set_XY(GameComponent *me, int x, int y);

/*-------------------------------------------------------------------------*/
/**
  @brief    Set new width & height
  @param    me     pointer to GameComponent
  @param    w      width in pixels
  @param    h      height in pixels
  @return   void

 */
/*--------------------------------------------------------------------------*/
void GameComponent_set_WH(GameComponent *me, int w, int h);

#endif /* GAME_COMPONENT_H */