#ifndef BACKGROUND_H
#define BACKGROUND_H

#define BG_ERR_PRINT 0 //0: no fprintf of SDL_GetError()

#include "SDL2/SDL.h"
#include "mySDL2_tools.h"
#include "Classes/game_component.h" //Superclass

/****************************************************************************
 *    PUBLIC TYPES
 ***************************************************************************/
//GameWindow Attributes
typedef struct{
    GameComponent super; //Super class attributes
    //For now, only super class
}Background;

/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/

/* Constructor implementation */
/*-------------------------------------------------------------------------*/
/**
  @brief    Create an instance of Background class
  @param    me     pointer for the new Background
  @param    r      pointer to the renderer
  @param    path   path to the texutre file
  @return   0 if succeeded, less than 0 if failed.

 */
/*--------------------------------------------------------------------------*/
int Background_ctor(Background * const me, SDL_Renderer *r, const char *path);

//Destructor prototype
/*-------------------------------------------------------------------------*/
/**
  @brief    Destroy a Background instance
  @param    me     pointer to the Background to free
  @return   void

    Detect if it's a source GC
 */
/*--------------------------------------------------------------------------*/
void Background_dtor(Background * const me);

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/


#endif /* BACKGROUND_H */