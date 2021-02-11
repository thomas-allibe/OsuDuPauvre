#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#define GB_ERR_PRINT 0 //0: no fprintf of SDL_GetError()

#include <SDL2/SDL.h>
#include <stdio.h>
#include "mySDL2_tools.h"
#include "global_variables.h"
#include "Classes/game_component.h"
#include "Classes/background.h"
#include "Classes/circle.h"

/****************************************************************************
 *    PUBLIC TYPES
 ***************************************************************************/
//GameWindow Attributes
typedef struct{
    //SDL_video
    SDL_Renderer *renderer;
    //Objects / Textures
    Background background;
    SDL_Texture *circle_texture;
    HitItems items[25];
    int nb_items;
    int next_item_index;
    //Player
    int health;
    Uint32 score;
    Uint16 combo;
    //Game
    GBState GameState;
}GameBoard;

typedef enum{
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

/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/
//Constructor prototypes
/*-------------------------------------------------------------------------*/
/**
  @brief    Create an instance of GameBoard class
  @param    me     pointer for the new GameBoard
  @param    r      pointer to the renderer
  @param    t      pointer to the texture for this class
  @return   0 if succeeded, less than 0 if failed.

 */
/*--------------------------------------------------------------------------*/
int GameBoard_ctor(GameBoard * const me, SDL_Renderer *r, SDL_Texture *t);

//Destructor prototype
/*-------------------------------------------------------------------------*/
/**
  @brief    Destroy a GameBoard instance
  @param    me     pointer to the GameBoard to free
  @return   void

 */
/*--------------------------------------------------------------------------*/
void GameBoard_dtor(GameBoard * const me);

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/

/*-------------------------------------------------------------------------*/
/**
  @brief    Process the last user input
  @param    me     pointer to the GameBoard instance
  @param    user_event     pointer to the user event structure
  @return   0 if succeeded, less than 0 if failed

 */
/*--------------------------------------------------------------------------*/
int GameBoard_processEvent(GameBoard * const me, Input *user_input);

/*-------------------------------------------------------------------------*/
/**
  @brief    update all "moving" objects
  @param    me     pointer to the GameBoard instance
  @param    user_event     pointer to the user event structure
  @return   0 if succeeded

 */
/*--------------------------------------------------------------------------*/
int GameBoard_forward(GameBoard * const me);

#endif /* GAMEBOARD_H */