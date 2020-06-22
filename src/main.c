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
/* Custom includes */
#include "global_variables.h"
#include "ini_settings.h"
#include "initialize_functions.h"
#include "my_events.h"
#include "iniparser/iniparser.h"
#include "Classes/game_window.h"
/*
#include "Classes/game_component.h"
#include "Classes/background.h"
#include "Classes/clickable_item.h"
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
    int statut = EXIT_SUCCESS;

    //Classes
    GameWindow gw = {NULL, NULL};

/*------------------------------------Verif Arguments-----------------------------------*/
    if(argc < 2){
	    	printf("Pas assez d'arguments : ./OsuDuPauvre.exe TIME\n");
	    	return 1;
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

                state_machine = GetUserEvent;
                break;
            
//### GET USER EVENT            
            case GetUserEvent:

                state_machine = getUserEvent(&user_input);

                break;
            
//### WAIT            
            case Wait:
                fprintf(stderr, "Wait\n");                
                
                SDL_Delay(atoi(argv[1]));

                state_machine = Quit;
                break;
            
//### ERROR            
            case Error:
                fprintf(stderr, "Error\n");
                
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

    return 0;
}

void libDeInit(){
    //SDL DEINIT
    SDL_Quit();
}