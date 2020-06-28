#ifndef MY_EVENTS_H
#define MY_EVENTS_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include "global_variables.h"


/*-------------------------------------------------------------------------*/
/**
  @brief    get event from user
  @param    usr_input     struct pointer containing the input to process
  @return   the next state for the state machine

 */
/*--------------------------------------------------------------------------*/
State getUserEvent(Input *usr_input);

#endif /* MY_EVENTS_H */