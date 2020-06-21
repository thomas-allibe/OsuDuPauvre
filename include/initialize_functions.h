#ifndef INITIALIZE_FUNCTIONS_H
#define INITIALIZE_FUNCTIONS_H

#include <SDL2/SDL_error.h>
#include "Classes/game_window.h"
#include "iniparser/iniparser.h"

//Number of sections & keys in Setting.ini file
#define NB_SECTIONS 3
#define NB_KEYS 2

/*-------------------------------------------------------------------------*/
/**
  @brief    get settings from an ini file
  @param    ini_path     path for ini file
  @return   dictionary containing settings, NULL if error

 */
/*--------------------------------------------------------------------------*/
dictionary *getSettings(char *ini_path);

/*-------------------------------------------------------------------------*/
/**
  @brief    create a game_window instance
  @param    settings     settings dictionary
  @param    gw     pointer to uninitialize GameWindow instance
  @param    title     game title for window title
  @return   0 if ok, -1 if error

 */
/*--------------------------------------------------------------------------*/
int createMainWindow(dictionary *settings, GameWindow *gw, const char *title);


#endif /* INITIALIZE_FUNCTIONS_H */