#include "Classes/gameboard.h"

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
int GameBoard_ctor(GameBoard * const me, SDL_Renderer *r, SDL_Texture *t){
    //SDL_video
    me->renderer = r;

    //Objects / Textures
    Background_ctor(&me->background, me->renderer, "../images/bg_1.png");

    me->circle_texture = NULL;
    me->circle_texture = loadImageStream("../images/BG_1.png", PNG, r);
    if(me->circle_texture == NULL){
        #ifndef GB_ERR_PRINT
        fprintf(stderr, "Erreur loadImageStatic : %s", SDL_GetError());
        #endif
        return -1;
    }

    me->nb_items = 0;

    //Player
    me->score = 0;
    me->combo = 0;

    //Game
    me->GameState = Pause;
    
}

//Destructor prototype
/*-------------------------------------------------------------------------*/
/**
  @brief    Destroy a GameBoard instance
  @param    me     pointer to the GameBoard to free
  @return   void

 */
/*--------------------------------------------------------------------------*/
void GameBoard_dtor(GameBoard * const me){
    Background_dtor(&me->background);
    //Circles / Sliders / Spiner
    for(int i=0 ; i<me->nb_items ; i++){
        if(me->items[i].type == 1)
            Circle_dtor(&me->items[i].circle);
    }
    SDL_DestroyTexture(&me->circle_texture);
}

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
int GameBoard_processEvent(GameBoard * const me, Input *user_input){
    //Hit processing
    if(user_input->hit1 == 1 || user_input->hit2 == 1){
        switch(me->GameState){
            case Pause:
                me->GameState = Running;
                break;

            case Running:
                //Check for item
                if(me->nb_items > 0){
                    if(me->items[me->next_item_index].type == 1){
                        
                    }
                }
                //Check for menu
                    //No menu for now
                break;
            default:;
        }
    }

    return 0;
}