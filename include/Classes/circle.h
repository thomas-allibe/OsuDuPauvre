#ifndef CIRCLE_H
#define CIRCLE_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "global_variables.h"

typedef struct{
	SDL_Texture *circle;
	SDL_Texture *overlay;
	SDL_Texture *approach;
}Circle_Textures;

/****************************************************************************
 *    PUBLIC TYPES
 ***************************************************************************/
//GameWindow Attributes
typedef struct{
    SDL_Renderer *renderer;
	const Circle_Textures *textures;
    SDL_Rect pos; //Circle position
	SDL_Rect ac_pos; //Approach circle position
	int ac_w_start;
	int ac_w_end;
	int ac_h_start;
	int ac_h_end;
    int duration;
    int time_left; //Needs to be signed
    int x_center;
    int y_center;
    int radius;
}Circle;

/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/

/**
  @brief    Create an instance of Circle class
  @param    r		pointer to the renderer
  @param    ct		pointer to a struct containing textures for circle, hoverlay & approach circle.
  @param    radius	radius for hit box
  @param    pos   xy position & width, height
  @return   Pointer to the GameBoard instance. NULL if failed.
			Use Circle_dtor() to destroy it.
 */
Circle* Circle_ctor(SDL_Renderer *r, Circle_Textures *ct, int radius, SDL_Rect *pos);

/**
  @brief    Destroy a Circle instance
  @param    me     pointer to the Circle to free
  @return   void
 */
void Circle_dtor(Circle *me);

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/

/**
  @brief	Update object acording to elapsed time
  @param	me	pointer to Circle instance
  @param	dt	elapsed time
  @return	0 on succes, -1 on error
 */
void Circle_update(Circle *me, int dt);

/**
  @brief	Draw the circle texture on renderer linked in ctor.
  @param	me	pointer to the Circle.
  @return	0 if succeded, -1 if failed.
 */
int Circle_draw(Circle *me, double prediction);

/**
  @brief    Return whether or not the click is on the circle
  @param    me     pointer to the Circle
  @param    x     x position of the mouse
  @param    y     y position of the mouse
  @return   return 1 if the mouse is on the circle, else return 0
 */
int Circle_is_pos_on_circle(Circle *me, int x, int y);

/**
  @brief	Check if circle has time left
  @param	me	pointer to circle
  @return	0 if it has, 1 if it hasn't
 */
int Circle_no_time_left(Circle *me);

#endif /* CIRCLE_H */