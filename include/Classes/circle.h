#ifndef CIRCLE_H
#define CIRCLE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_Mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "global_variables.h"

// #define errChk(x) x;
//if(x){return -1;}

#define EXTRA_LIFE_SPAN 75
#define SHOW_PTS_DURATION 300

#define HIT_FLAG_MISS -1
#define HIT_FLAG_50 1
#define HIT_FLAG_100 2
#define HIT_FLAG_300 3

typedef struct{
	SDL_Texture *circle;
	SDL_Texture *overlay;
	SDL_Texture *approach;
	SDL_Texture *miss;
	SDL_Texture *_50;
	SDL_Texture *_100;
	SDL_Texture *_300;
}Circle_Textures;

typedef struct{
    Mix_Chunk *hit_sound;
    Mix_Chunk *combo_break;
}Circle_Sounds;

/****************************************************************************
 *    PUBLIC TYPES
 ***************************************************************************/
//GameWindow Attributes
typedef struct{
    SDL_Renderer *renderer;
	const Circle_Textures *textures;
	const Circle_Sounds *sounds;
    SDL_Rect pos; //Circle position
	SDL_Rect ac_pos; //Approach circle position
	int ac_w_start;
	int ac_w_end;
	int ac_h_start;
	int ac_h_end;
    int ac_hit;
	int fade_in;
	int life_time;
    int x_center;
    int y_center;
    int radius;
	Sint8 hit_flag;//HIT_FLAG_MISS,HIT_FLAG_50,HIT_FLAG_100,HIT_FLAG_300
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
Circle* Circle_ctor(SDL_Renderer *r, Circle_Textures *ct, Circle_Sounds *cs, int radius, SDL_Rect *pos);

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
void Circle_update(Circle *me, int dt, Uint16 combo);

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
  @brief	Compute if circle has been hit with the right timing
  @param	me	pointer to circle
  @param    x     x position of the mouse
  @param    y     y position of the mouse
  @return	hit points (50 / 100 / 300), 0 if timing/position is bad
            or -1 if circle is disapearing
 */
int Circle_hit(Circle *me, int x, int y);

/**
  @brief	Check if circle has time left
  @param	me	pointer to circle
  @return	0 if it has, 1 if it hasn't
 */
int Circle_has_time_left(Circle *me);

/**
  @brief	Check if circle has been missed
  @param	me	pointer to circle
  @return	1 if it has, 0 if not
 */
int Circle_is_missed(Circle *me);

/**
  @brief	Check if circle needs to be destroyed
  @param	me	pointer to circle
  @return	1 if it needs, 0 if it doesn't
 */
int Circle_to_destroy(Circle *me);

#endif /* CIRCLE_H */