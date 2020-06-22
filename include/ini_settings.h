#ifndef INI_SETTINGS_H
#define INI_SETTINGS_H

#include <SDL2/SDL.h>
#include "global_variables.h"
#include "iniparser/iniparser.h"

//Ini file
#define SECTION_MAX_LENGHT 15
#define KEY_MAX_LENGHT 20
#define VALUE_MAX_LENGHT 10
//Default ini file with default settings
#define NB_SECTIONS 3
  #define SETTING_SECTIONS "window", "sounds", "controls"
#define NB_KEYS 2
  #define SETTING_KEYS "window:width", "window:height"
//  #define SETTING_VALUES "1280", "720" //Replaced by GAME_SETTINGS GV

//Limit setting values
#define MIN_WIDTH 848
#define MIN_HIGHT 480
#define MAX_WIDTH 1280
#define MAX_HIGHT 720

/*-------------------------------------------------------------------------*/
/**
  @brief    Set the default values for GAME_SETTINGS GV
  @return   void
 */
/*--------------------------------------------------------------------------*/
void setDefaultSettingsGV();

/*-------------------------------------------------------------------------*/
/**
  @brief    get settings from an ini file
  @param    ini_path     path for ini file
  @return   dictionary containing settings, NULL if error

 */
/*--------------------------------------------------------------------------*/
int getIniSettings(char *ini_path);



#endif /* INI_SETTINGS_H */