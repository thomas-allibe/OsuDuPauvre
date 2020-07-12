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
    SDL_Texture *approach_circle;
    Uint32 remaining_time;
    int x_center;
    int y_center;
    int radius;
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
  @param    radius      radius for hit box
  @return   0 if succeeded, less than 0 if failed.

 */
/*--------------------------------------------------------------------------*/
int Circle_ctor(Circle * const me, SDL_Renderer *r, SDL_Texture *t, int radius);

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

/*-------------------------------------------------------------------------*/
/**
  @brief    Return whether or not the click is on the circle
  @param    me     pointer to the Circle
  @param    x     x position of the mouse
  @param    y     y position of the mouse
  @return   return 1 if the mouse is on the circle, else return 0

 */
/*--------------------------------------------------------------------------*/
int Circle_is_click_on_circle(Circle * const me, int x, int y);

#endif /* CIRCLE_H */