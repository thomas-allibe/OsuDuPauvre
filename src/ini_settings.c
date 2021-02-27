#include "ini_settings.h"


/*-------------------------------------------------------------------------*/
/**
  @brief    Set the default values for GAME_SETTINGS GV
  @return   void
 */
/*--------------------------------------------------------------------------*/
void setDefaultSettingsGV(){
//Window
    GAME_SETTINGS.window_w = 1280;
    GAME_SETTINGS.window_h = 720;
//Controls 
    GAME_SETTINGS.controls_hit1 = SDLK_w;
    GAME_SETTINGS.controls_hit2 = SDLK_x;
    GAME_SETTINGS.controls_restart = SDLK_q;
    GAME_SETTINGS.controls_pause = SDLK_p;
//Game
    GAME_SETTINGS.game_AR = 9;
}


/*-------------------------------------------------------------------------*/
/**
  @brief    get settings from an ini file
  @param    ini_path     path for ini file
  @return   dictionary containing settings, NULL if error

 */
/*--------------------------------------------------------------------------*/
int getIniSettings(char *ini_path){

/*---------------------------------Variables--------------------------------*/
    dictionary *settings = NULL;
    FILE *ini_file = NULL;

    int tmp_int = -1;
    const char *tmp_char = NULL;

    SDL_Rect bounds;

    int flag_new_key = 0;
    int flag_new_section = 0;

    //Default ini file, with default values
    char setting_sections[NB_SECTIONS][SECTION_MAX_LENGHT] = {SETTING_SECTIONS};
    char setting_keys[NB_KEYS][KEY_MAX_LENGHT] = {SETTING_KEYS};
    char setting_values[NB_KEYS][VALUE_MAX_LENGHT];
        //Set default values, IN THE SAME ORDER AS SETTING_KEYS
        sprintf(setting_values[0], "%d", GAME_SETTINGS.window_w);
        sprintf(setting_values[1], "%d", GAME_SETTINGS.window_h);
        sprintf(setting_values[2], "%c", GAME_SETTINGS.controls_hit1);
        sprintf(setting_values[3], "%c", GAME_SETTINGS.controls_hit2);
        sprintf(setting_values[4], "%c", GAME_SETTINGS.controls_restart);
        sprintf(setting_values[5], "%c", GAME_SETTINGS.controls_pause);
        sprintf(setting_values[6], "%d", GAME_SETTINGS.game_AR);
    // WARNING
    // The else case (of settings == NULL) uses these arrays content individualy
    // If they are modified, the code below has to be modified
    
/* ---------------------------- Open Config File ---------------------------- */

    settings = iniparser_load(ini_path);
    if(settings == NULL){
    //The file doesn't exist, create it
        fprintf(stderr, "%s non ouvert\n", ini_path);
        ini_file = fopen(ini_path, "w+");
        if(ini_file == NULL){
            //Can't create the file, don't try anymore
            SDL_SetError("Settings.ini file doesn't exist, can't create one.");
            return -1;
        }

    //Re-load the file to get an empty ini dictionary
        settings = iniparser_load(ini_path);
        if(settings == NULL){
            //Can't open Settings.ini
            SDL_SetError("Can't open Settings.ini");
            return -2;
        }

    //Fill the dictionary with default settings
            //All sections
        for(int i=0 ; i<NB_SECTIONS ; i++){
            iniparser_set(settings, setting_sections[i], NULL);
        }
            //All keys & values
        for(int i=0 ; i<NB_KEYS ; i++){
            iniparser_set(settings, setting_keys[i], setting_values[i]);
        }

    //Dump  the dictionary into the ini file
        iniparser_dump_ini(settings, ini_file);

        fclose(ini_file);
    }
    else{
        //The file exists, check the value of each keys
        //If the value isn't valid, use the default one
        
    //Check sections presence
        for(int i=0 ; i<NB_SECTIONS ; i++){
            if(iniparser_find_entry(settings, setting_sections[i]) == 0){
                //The key dosen't exist, create it with default values
                fprintf(stderr, "Section %s ajoutee\n", setting_sections[i]);
                iniparser_set(settings, setting_sections[i], NULL);
                flag_new_section = 1;
            }
        }
        
    //Get display informations
        if(SDL_GetDisplayUsableBounds(0, &bounds) != 0){
            //Non critical error, use default instead
            bounds.w = MAX_WIDTH;
            bounds.h = MAX_HIGHT;
        }
        
/* ----------------------------- Window section ----------------------------- */

    //### Window Width / key = "window:width" ###
        tmp_int = iniparser_getint(settings, setting_keys[0], -1);
        if( _valid_int(tmp_int, MIN_WIDTH, bounds.w) ){
            GAME_SETTINGS.window_w = tmp_int;
        }
        else{
            //Keep GAME_SETTINGS.window_w default value
            //Since ini value is invalid, replace it with default
            iniparser_set(settings, setting_keys[0], setting_values[0]);
            flag_new_key = 1;
        }
        

    //### Window Hight / key = "window:hight" ###
        tmp_int = iniparser_getint(settings, setting_keys[1], -1);
        if( _valid_int(tmp_int, MIN_HIGHT, bounds.h) ){
            GAME_SETTINGS.window_h = tmp_int;
        }
        else{
            iniparser_set(settings, setting_keys[1], setting_values[1]);
            flag_new_key = 1;
        }

/* ---------------------------- Controls section ---------------------------- */

    //### Controls Hit1 / key = "controls:hit1" ###    
        if((tmp_char = iniparser_getstring(settings, setting_keys[2], NULL)) != NULL){    
            if( _valid_char(*tmp_char) ){
                GAME_SETTINGS.controls_hit1 = (SDL_Keycode) tolower(*tmp_char);
            }
        }
        else{
            iniparser_set(settings, setting_keys[2], setting_values[2]);
            flag_new_key = 1;
        }

    //### Controls Hit2 / key = "controls:hit2" ###
        if((tmp_char = iniparser_getstring(settings, setting_keys[3], NULL)) != NULL){    
            if( _valid_char(*tmp_char) ){
                GAME_SETTINGS.controls_hit2 = (SDL_Keycode) tolower(*tmp_char);
            }
        }
        else{
            iniparser_set(settings, setting_keys[3], setting_values[3]);
            flag_new_key = 1;
        }
    //### Controls Restart / key = "controls:restart" ###
        if((tmp_char = iniparser_getstring(settings, setting_keys[4], NULL)) != NULL){    
            if( _valid_char(*tmp_char) ){
                GAME_SETTINGS.controls_restart = (SDL_Keycode) tolower(*tmp_char);
            }
        }
        else{
            iniparser_set(settings, setting_keys[4], setting_values[4]);
            flag_new_key = 1;
        }
    //### Controls Pause / key = "controls:pause" ###
        if((tmp_char = iniparser_getstring(settings, setting_keys[5], NULL)) != NULL){    
            if( _valid_char(*tmp_char) ){
                GAME_SETTINGS.controls_pause = (SDL_Keycode) tolower(*tmp_char);
            }
        }
        else{
            iniparser_set(settings, setting_keys[5], setting_values[5]);
            flag_new_key = 1;
        }
    //### Game Approach Rate / key = "game:AR" ###
        tmp_int = iniparser_getint(settings, setting_keys[6], -1);
        if( _valid_int(tmp_int, 0, 10) ){
            GAME_SETTINGS.game_AR = tmp_int;
        }
        else{
            iniparser_set(settings, setting_keys[6], setting_values[6]);
            flag_new_key = 1;
        }

/*  //Not implemented anymore      
        //Check keys
        for(int i=0 ; i<NB_KEYS ; i++){
            if(iniparser_find_entry(settings, setting_keys[i]) == 0){
                //The key dosen't exist, create it with default values
                fprintf(stderr, "Key %s ajoutee\n", setting_keys[i]);
                iniparser_set(settings, setting_keys[i], setting_values[i]);
                flag_new_key = 1;
            }
        }
*/

/* ----------------------------- Update ini file ---------------------------- */

        if(flag_new_key == 1 || flag_new_section == 1){
            fprintf(stderr, "Ini modifie\n");
            //Dump  the dictionary into the ini file to add modified content
            ini_file = fopen(ini_path, "w+");
            iniparser_dump_ini(settings, ini_file);
            fclose(ini_file);
        }
    }

    iniparser_freedict(settings);

    return 0;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    check the value of an int, no bad input checking
  @param    number     number to check
  @param    min        min value     
  @param    max        max value
  @return   1 if min <= number <= max, 0 if not

 */
/*--------------------------------------------------------------------------*/
int _valid_int(int number, int min, int max){
    return ( min <= number  &&  number <= max );
}

/*-------------------------------------------------------------------------*/
/**
  @brief    check the value of an char
  @param    c     char to check
  @return   1 if c is a number or a letter, 0 if not

 */
/*--------------------------------------------------------------------------*/
int _valid_char(char c){
    return (('0'<= c && c <= '9') ||
            ('a'<= c && c <= 'z') ||
            ('A'<= c && c <= 'Z')   );
}

/*-------------------------------------------------------------------------*/
/**
  @brief    convert a char to the corresponding keyboard scancode
  @param    c     char to convert, number or letter !!!
  @return   the scancode if c is valid, SDL_SCANCODE_UNKNOWN if not
 
    Upper case & lower case return the same scancode
 */
/*--------------------------------------------------------------------------*/
SDL_Scancode _char_to_scancode(char c){
    if(c == '0')
        return SDL_SCANCODE_0;
    if('1'<= c && c <= '9')
        return SDL_SCANCODE_1 + (SDL_Scancode)(c - '1');
    if('a'<= c && c <= 'z')
        return SDL_SCANCODE_A + (SDL_Scancode)(c - 'a');
    if('A'<= c && c <= 'Z')
        return SDL_SCANCODE_A + (SDL_Scancode)(c - 'A');

    return SDL_SCANCODE_UNKNOWN;
}