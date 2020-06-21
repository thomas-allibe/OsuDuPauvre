#include "initialize_functions.h"

/*-------------------------------------------------------------------------*/
/**
  @brief    get settings from an ini file
  @param    ini_path     path for ini file
  @return   dictionary containing settings, NULL if error

 */
/*--------------------------------------------------------------------------*/
dictionary *getSettings(char *ini_path){
/*---------------------------------Variables--------------------------------*/
    dictionary *settings = NULL;
    FILE *ini_file = NULL;
    int flag_new_key = 0;
    int flag_new_section = 0;

    //Default ini file, with default values
    char setting_sections[NB_SECTIONS][15] ={
        "window",
        "sounds",
        "controls"
    };
    char setting_keys[NB_KEYS][20] = {
        "window:width",
        "window:height"
    };
    char setting_values[NB_KEYS][10] ={
        "1280",
        "720"
    };
/*------------------------------------Core----------------------------------*/

    settings = iniparser_load(ini_path);
    if(settings == NULL){
        //The file doesn't exist, create it
        fprintf(stderr, "%s non ouvert\n", ini_path);
        ini_file = fopen(ini_path, "w+");
        if(ini_file == NULL){
            //Can't create the file, don't try anymore
            SDL_SetError("Settings.ini file doesn't exist, can't create one.");
            return NULL;
        }

        //Re-load the file to get an empty ini dictionary
        settings = iniparser_load(ini_path);
        if(settings == NULL){
            //Can't open Settings.ini
            SDL_SetError("Can't open Settings.ini");
            return NULL;
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
        //The file exists, check the presence of each keys
        //Check sections
        for(int i=0 ; i<NB_SECTIONS ; i++){
            if(iniparser_find_entry(settings, setting_sections[i]) == 0){
                //The key dosen't exist, create it with default values
                fprintf(stderr, "Section %s ajoutee\n", setting_sections[i]);
                iniparser_set(settings, setting_sections[i], NULL);
                flag_new_section = 1;
            }
        }
        //Check keys
        for(int i=0 ; i<NB_KEYS ; i++){
            if(iniparser_find_entry(settings, setting_keys[i]) == 0){
                //The key dosen't exist, create it with default values
                fprintf(stderr, "Key %s ajoutee\n", setting_keys[i]);
                iniparser_set(settings, setting_keys[i], setting_values[i]);
                flag_new_key = 1;
            }
        }
        //Update ini file
        if(flag_new_key == 1 || flag_new_section == 1){
            fprintf(stderr, "Ini modifie\n");
            //Dump  the dictionary into the ini file to add content
            ini_file = fopen(ini_path, "w+");
            iniparser_dump_ini(settings, ini_file);
            fclose(ini_file);
        }
    }

    return settings;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    create a game_window instance
  @param    settings     settings dictionary
  @param    gw     pointer to uninitialize GameWindow instance
  @param    title     game title for window title
  @return   0 if ok, -1 if error

 */
/*--------------------------------------------------------------------------*/
int createMainWindow(dictionary *settings, GameWindow *gw, const char *title){
/*---------------------------------Variables--------------------------------*/
    //Options
    WindowOptions wo;
    wo.title = title;
    wo.x = SDL_WINDOWPOS_CENTERED; wo.y = SDL_WINDOWPOS_CENTERED;
    wo.w = iniparser_getint(settings, "window:width", 1280);
    wo.h = iniparser_getint(settings, "window:height", 720);
    wo.flags = SDL_WINDOW_SHOWN;

    RendererOptions ro;
    ro.index = -1;
    ro.flags = SDL_RENDERER_ACCELERATED;

/*------------------------------------Core----------------------------------*/
    if(GameWindow_ctor(gw, &wo, &ro) < 0){
        return -1;
    }

    return 0;
}