#ifndef INITIALIZE_FUNCTIONS_H
#define INITIALIZE_FUNCTIONS_H

#include <SDL2/SDL.h>
#include "global_variables.h"
#include "Classes/game_window.h"


/*-------------------------------------------------------------------------*/
/**
  @brief    create a game_window instance
  @param    settings     settings dictionary
  @param    gw     pointer to uninitialize GameWindow instance
  @param    title     game title for window title
  @return   0 if ok, -1 if error

 */
/*--------------------------------------------------------------------------*/
int createMainWindow(GameWindow *gw);


#endif /* INITIALIZE_FUNCTIONS_H */