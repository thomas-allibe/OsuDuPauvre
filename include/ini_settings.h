#ifndef INI_SETTINGS_H
#define INI_SETTINGS_H

#include <ctype.h>
#include <SDL2/SDL.h>
#include "global_variables.h"
#include "iniparser/iniparser.h"

//Ini file
#define SECTION_MAX_LENGHT 15
#define KEY_MAX_LENGHT 20
#define VALUE_MAX_LENGHT 10
//Default ini file with default settings
#define NB_SECTIONS 4
	#define SETTING_SECTIONS "window", "sounds", "controls", "game"
#define NB_KEYS 7
	#define SETTING_KEYS "window:width", "window:height", "controls:hit1", \
    "controls:hit2", "controls:restart", "controls:pause", "game:ar"

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

/*-------------------------------------------------------------------------*/
/**
  @brief    check the value of an int, no bad input checking
  @param    number     number to check
  @param    min        min value     
  @param    max        max value
  @return   1 if min <= number <= max, 0 if not

 */
/*--------------------------------------------------------------------------*/
int _valid_int(int number, int min, int max);

/*-------------------------------------------------------------------------*/
/**
  @brief    check the value of an char
  @param    c     char to check
  @return   1 if c is a number or a letter, 0 if not

 */
/*--------------------------------------------------------------------------*/
int _valid_char(char c);

/*-------------------------------------------------------------------------*/
/**
  @brief    convert a char to the corresponding keyboard scancode
  @param    c     char to convert, number or letter !!!
  @return   the scancode if c is valid, SDL_SCANCODE_UNKNOWN if not
 
    Upper case & lower case return the same scancode
 */
/*--------------------------------------------------------------------------*/
SDL_Scancode _char_to_scancode(char c);

#endif /* INI_SETTINGS_H */