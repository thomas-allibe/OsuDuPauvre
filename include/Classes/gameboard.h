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

#define START_TIME 500
#define SPAWN_PERIOD 500

/****************************************************************************
 *    PUBLIC TYPES
 ***************************************************************************/
//GameWindow Attributes
typedef enum{
	Starting,
	Pause,
	Running
}GBState;

typedef struct ItemList{
	struct ItemList *next;
	struct ItemList *prev;
	int type;
    union{
    /* 1 */ Circle *circle;
    /* 2 */ int *SliderFoo;
    /* 3 */ int *SpinerFoo;
    };
}ItemList;

typedef struct{
    //SDL_video
    SDL_Renderer *renderer;
    SDL_PixelFormat *pixel_format;
    //Objects / Textures
    Background *background;
    Background *pause_bg;
    Circle_Textures circle_textures;
	SDL_Rect circle_pos;
	int circle_radius;
    ItemList *item_list;
    ItemList *item_list_end;
    SDL_Rect spawn_region;
    int nb_items;
    int next_item_index;
    int spawn_time;
    //Player
    int health;
    Uint32 score;
    Uint16 combo;
    //Game
    GBState GameState;
    int start_time;
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
  @param    dt     time enlapsed
  @return   0 if succeeded
 */
int GameBoard_update(GameBoard *me, Uint32 dt);

/**
  @brief	Render GameBoard
  @param	me  pointer to GameBoard instance
  @param  prediction  fraction of where we are between two updates
  @return	0 on success, -1 on error
 */
int GameBoard_render(GameBoard *me, double prediction);

/* -------------------------------------------------------------------------- */
/*                                ItemsList API                               */
/* -------------------------------------------------------------------------- */

/**
  @brief	Initialize ItemList
  @param	item	One among Circle, Slider or Spiner
  @param	type	Item Type, see ItemList definition
  @return	ItemList first element pointer
 */
ItemList *ItemList_init(void *item, int type);

/**
  @brief	Add an Item at the end of the list
  @param	list	ItemList	
  @param	item	One among Circle, Slider or Spiner	
  @param	type	Item Type, see ItemList definition
  @return	Last list node on success, NULL on error
 */
ItemList *ItemList_add_item(ItemList *list, void *item, int type);

/**
  @brief	Remove the first Item in the list
  @param	list_in ItemList	
  @param	item_out	Item removed from list
  @param	type_out	Item Type removed from list, see ItemList definition
  @return	the new list pointer (2nd item) on success, NULL on error or if the list is empty
 */
ItemList *ItemList_remove_first(ItemList *list_in, void **item_out, int *type_out);

/**
  @brief	free the first Item in the list using the corresponding dtor
  @param	list_in ItemList	
  @return	the new list pointer (2nd item) on success, NULL on error or if the list is empty
 */
ItemList *ItemList_free_first(ItemList *list_in);

/**
  @brief	Free the entire list and apply dtor methods for each items.
  @param	list  list to free
  @return void
 */
void ItemList_free_list_and_items(ItemList *list);

/**
  @brief	Update all items in list using the appropriate method
  @param	list  item list to update
  @param	dt  enlapsed time
  @return	0 on success, -1 on error
 */
// int ItemList_update_items(ItemList *list, Uint32 dt);

/**
  @brief	Render all items in list using the appropriate method
  @param	list  item list to render
  @param	prediction  fraction of where we are between two updates
  @return	0 on success, -1 on error
 */
int ItemList_render_items(ItemList *list, double prediction);

/* -------------------------------------------------------------------------- */
/*                                    OTHER                                   */
/* -------------------------------------------------------------------------- */
/**
  @brief	generate a random number between min and max.
          Need to use sdtlib.h srand() before
  @param	min	min value
  @param	max	max value
  @return	a random unsigned number between min and max
 */
int Random(int min, int max);

#endif /* GAMEBOARD_H */