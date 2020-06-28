#ifndef CIRCLE_H
#define CIRCLE_H

#define C_ERR_PRINT 0 //0: no fprintf of SDL_GetError()

#include <SDL2/SDL.h>
#include <stdio.h>
#include "Classes/game_component.h" //Superclass

/****************************************************************************
 *    PUBLIC TYPES
 ***************************************************************************/
//GameWindow Attributes
typedef struct{
    GameComponent super;
    Uint32 remaining_time;
}Circle;

/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/
//Constructor prototypes
/*-------------------------------------------------------------------------*/
/**
  @brief    Create an instance of Circle class
  @param    me     pointer for the new Circle
  @param    r      pointer to the renderer
  @param    t      pointer to the texture for this class
  @return   0 if succeeded, less than 0 if failed.

 */
/*--------------------------------------------------------------------------*/
int Circle_ctor(Circle * const me, SDL_Renderer *r, SDL_Texture *t);

//Destructor prototype
/*-------------------------------------------------------------------------*/
/**
  @brief    Destroy a Circle instance
  @param    me     pointer to the Circle to free
  @return   void

 */
/*--------------------------------------------------------------------------*/
void Circle_dtor(Circle * const me);

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/

#endif /* CIRCLE_H */