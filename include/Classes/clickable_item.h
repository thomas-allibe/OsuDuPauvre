#ifndef CLICKABLE_ITEM_H
#define CLICKABLE_ITEM_H

#define CI_ERR_PRINT 0 //0: no fprintf of SDL_GetError()

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdint.h>
#include "Classes/game_component.h"

/****************************************************************************
 *    PUBLIC TYPES
 ***************************************************************************/

typedef uint32_t SDL_Time;

//GameWindow Attributes
typedef struct{
    GameComponent super; //Superclass
    SDL_Time remaining;
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
 

//Destructor prototype
/*-------------------------------------------------------------------------*/
/**
  @brief    Destroy a GameComponent instance
  @param    me     pointer to the GameComponent to free
  @return   void

 */
/*--------------------------------------------------------------------------*/


/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/


#endif /* CLICKABLE_ITEM_H */