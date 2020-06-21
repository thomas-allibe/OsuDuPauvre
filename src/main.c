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
#include "initialize_functions.h"
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

typedef enum {Initialize, , Wait, Error, Quit, Stop}State;

typedef struct{
    int foo;
}Input;

//Functions

int libInit();
void libDeInit();

void getUserEvent();

//Global Variables
const char *GAME_TITLE = "OsuDuPauvre!";

/*######################################################################################*/
/*#										   MAIN        								   #*/
/*######################################################################################*/
int main(int argc, char *argv[]){
/*---------------------------------------Variables--------------------------------------*/
    State state_machine = Initialize;
    int statut = EXIT_SUCCESS;

    dictionary *settings = NULL;
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
            
            case Initialize:
                fprintf(stderr, "Initialize\n");

                if(libInit() < 0){
                    state_machine = Error;
                    break;
                }

                settings = getSettings("settings.ini");
                if(settings == NULL){
                    state_machine = Error;
                    break;
                }

                if(createMainWindow(settings, &gw, GAME_TITLE) < 0){
                    state_machine = Error;
                    break;
                }

                state_machine = Wait;
                break;

            case Wait:
                fprintf(stderr, "Wait\n");                
                
                SDL_Delay(atoi(argv[1]));

                state_machine = Quit;
                break;
            
            case Error:
                fprintf(stderr, "Error\n");
                
                statut = EXIT_FAILURE;
                
                state_machine = Quit;
                break;
            
            case Quit:
                fprintf(stderr, "Quit\n");
                
                iniparser_freedict(settings);
                GameWindow_dtor(&gw);
                libDeInit();

                state_machine = Stop;
                break;
            
            default:
                fprintf(stderr, "Default\n");
                //Code
                state_machine = Quit;
        }
    }
    
    return statut;
}

/*--------------------------------------------------------------------------------------*/
/*-									   libInit()      							       -*/
/*-                    Functions for library initialization : SDL2                     -*/
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
/*--------------------------------------------------------------------------------------*/
/*-									   libInit()      							       -*/
/*-                          get user event for this cycle                             -*/
/*--------------------------------------------------------------------------------------*/
void getUserEvent(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){

    }
}