#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "mySDL2_tools.h"
#include "my_events.h"
#include "global_variables.h"
#include "Classes/game_component.h"
#include "Classes/background.h"
#include "Classes/circle.h"

/****************************************************************************
 *    PUBLIC TYPES
 ***************************************************************************/
//GameWindow Attributes
typedef enum{
	Starting,
	Pause,
	Running
}GBState;

typedef struct{
    int type;
    union{
    /* 1 */ Circle *circle;
    /* 2 */ int *SliderFoo;
    /* 3 */ int *SpinerFoo;
    };
}HitItems;

typedef struct{
    //SDL_video
    SDL_Renderer *renderer;
    SDL_PixelFormat *pixel_format;
    //Objects / Textures
    Background *background;
    Circle_Textures circle_textures;

    HitItems *items;
    Circle *test_circle;
    int nb_items;
    int next_item_index;
    //Player
    int health;
    Uint32 score;
    Uint16 combo;
    //Game
    GBState GameState;
}GameBoard;

/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/

/**
  @brief    Create an instance of GameBoard class
  @param    r      pointer to the renderer
  @param    t      pointer to the texture for this class
  @return   Pointer to the GameBoard instance. NULL if failed.
			Use GameBoard_dtor() to destroy it.
 */
GameBoard* GameBoard_ctor(SDL_Renderer *renderer);

/**
  @brief    Destroy a GameBoard instance
  @param    me     pointer to the GameBoard to free
  @return   void
 */
void GameBoard_dtor(GameBoard *me);

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/

/**
  @brief    Process the last user input
  @param    me     pointer to the GameBoard instance
  @param    user_event     pointer to the user event structure
  @return   0 if succeeded, less than 0 if failed
 */
int GameBoard_processEvent(GameBoard *me, Input *user_input);

/**
  @brief    update all "moving" objects
  @param    me     pointer to the GameBoard instance
  @param    user_event     pointer to the user event structure
  @return   0 if succeeded
 */
int GameBoard_update(GameBoard *me);

/**
  @brief	Render GameBoard
  @param	me  pointer to GameBoard instance
  @param  frame_fraction  fraction of where we are between two updates
  @return	0 on success, -1 on error
 */
int GameBoard_render(GameBoard *me, Uint32 frame_fraction);

#endif /* GAMEBOARD_H */