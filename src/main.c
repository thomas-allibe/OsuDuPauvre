/*######################################################################################*/
/*#					                    OsuDuPauvre	                                   #*/
/*#					                        		                                   #*/
/*#					                Thomas ALLIBE E2I3                                 #*/
/*######################################################################################*/

/* Std C includes */
#include <stdio.h>
#include <stdlib.h>
/* SDL includes */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
/* Custom includes */
#include "global_variables.h"
#include "ini_settings.h"
#include "initialize_functions.h"
#include "my_events.h"
#include "iniparser/iniparser.h"
#include "Classes/game_window.h"
#include "Classes/gameboard.h"
/*
#include "Classes/game_component.h"
#include "Classes/background.h"
#include "Classes/circle.h"
*/
/*======================================================================================*/
/*=									    Prototypes		   							   =*/
/*======================================================================================*/

//Typedef


//Functions
int libInit();
void libDeInit();

//global_variables.h
const char *GAME_TITLE = "OsuDuPauvre!";
Settings GAME_SETTINGS; //Values are set in initialize_functions.c

/*######################################################################################*/
/*#										   MAIN        								   #*/
/*######################################################################################*/
int main(int argc, char *argv[]){
/*---------------------------------------Variables--------------------------------------*/
    State state_machine = Initialize;
    Input user_input;
        user_input.quit = SDL_FALSE;
        user_input.hit1 = SDL_FALSE;
        user_input.hit1_r = SDL_FALSE;
        user_input.hit2 = SDL_FALSE;
        user_input.hit2_r = SDL_FALSE;
        user_input.mouse_x = 0;
        user_input.mouse_y = 0;
    
    int statut = EXIT_SUCCESS;

    //Classes
    GameWindow gw = {NULL, NULL};
    GameBoard gb;

/*------------------------------------Verif Arguments-----------------------------------*/
    Uint32 delay = 2000;
    if(argc > 1){
	    delay = atoi(argv[1]);
	}

/*-------------------------------------State Machine------------------------------------*/
    while(state_machine != Stop){
        switch(state_machine){
//### INITIALIZE            
            case Initialize:
                fprintf(stderr, "Initialize\n");

                //No error
                state_machine = Wait;
                
                setDefaultSettingsGV();

                if(libInit() != 0){
                    state_machine = Error;
                    break;
                }
                if(getIniSettings("settings.ini") != 0){
                    state_machine = Error;
                    break;
                }
                if(createMainWindow(&gw) != 0){
                    state_machine = Error;
                    break;
                }
                if(GameWindow_setIcon(&gw, "../images/w_ico_1.bmp") != 0){
                    state_machine = Error;
                    break;
                }

                state_machine = GetUserEvent;
                break;
            
//### GET USER EVENT            
            case GetUserEvent:
                state_machine = getUserEvent(&user_input);
                break;
            
//### UPDATE OBJECTS
            case UpdateObjects:
                fprintf(stderr, "x= %4d, y= %4d, h1= %1d:%1d, h2= %1d:%1d\r",
                        user_input.mouse_x, user_input.mouse_y,
                        user_input.hit1, user_input.hit1_r,
                        user_input.hit2, user_input.hit2_r);
                user_input.hit1 = SDL_FALSE;
                user_input.hit1_r = SDL_FALSE;
                user_input.hit2 = SDL_FALSE;
                user_input.hit2_r = SDL_FALSE;

                state_machine = GetUserEvent;
                break;            

//### RENDER CHANGES
            case RenderChanges:


                break;
//### WAIT            
            case Wait:
                fprintf(stderr, "Wait\n");                
                
                SDL_Delay(delay);

                state_machine = Quit;
                break;
            
//### ERROR            
            case Error:
                fprintf(stderr, "%s\n", SDL_GetError());
                
                statut = EXIT_FAILURE;
                
                state_machine = Quit;
                break;
            
//### QUIT            
            case Quit:
                fprintf(stderr, "Quit\n");
                
                GameWindow_dtor(&gw);
                libDeInit();

                state_machine = Stop;
                break;
            
//### DEFAULT            
            default:
                fprintf(stderr, "Default\n");
                //Code
                state_machine = Quit;
        }//End switch
    }//End while
    
    return statut;
}

/*--------------------------------------------------------------------------------------*/
/*-								 libInit() / libDeInit     					           -*/
/*-             Functions for library initialization & deinitialization                -*/
/*-                                     SDL2                                           -*/
/*--------------------------------------------------------------------------------------*/
int libInit(){
    //SDL INIT
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        fprintf(stderr, "Error SDL_Init : %s", SDL_GetError());
        return -1;
    }

    //SDL_Image INIT
    if(IMG_Init(IMG_INIT_PNG) == 0){
        fprintf(stderr, "Error IMG_Init : %s", SDL_GetError());
        return -1;
    }

    return 0;
}

void libDeInit(){
    //SDL_Image DEINIT
    IMG_Quit();
    //SDL DEINIT
    SDL_Quit();
}