#ifndef MY_EVENTS_H
#define MY_EVENTS_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include "global_variables.h"


typedef struct{
    Sint32 mouse_x;
    Sint32 mouse_y;
    SDL_bool quit;
    SDL_bool hit1;
    SDL_bool hit1_r; //repeat
    SDL_bool hit2;
    SDL_bool hit2_r; //repeat
    SDL_bool pause;
}Input;

/*-------------------------------------------------------------------------*/
/**
  @brief    get event from user
  @param    usr_input     struct pointer containing the input to process
  @return   the next state for the state machine

 */
/*--------------------------------------------------------------------------*/
void GetUserEvent(Input *usr_input);

/**
  @brief	Reset Input struct
  @param	usr_input	struct pointer containing the input to reset
  @return	void
 */
void ResetUserInput(Input *usr_input);

#endif /* MY_EVENTS_H */