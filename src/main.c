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

typedef enum {Initialize, Wait, Error, Quit, Stop}State;

//Functions

int libInit();
void libDeInit();

/*######################################################################################*/
/*#										   MAIN        								   #*/
/*######################################################################################*/
int main(int argc, char *argv[]){
/*---------------------------------------Variables--------------------------------------*/
    State state_machine = Initialize;
    int statut = EXIT_SUCCESS;


    GameWindow gw = {NULL, NULL};

    char *game_title = "OsuDuPauvre!";

    WindowOptions wo;
    wo.title = game_title;
    wo.x = SDL_WINDOWPOS_CENTERED; wo.y = SDL_WINDOWPOS_CENTERED;
    wo.w = 1080; wo.h = 720;
    wo.flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

    RendererOptions ro;
    ro.index = -1;
    ro.flags = SDL_RENDERER_ACCELERATED;

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
                if(GameWindow_ctor(&gw, wo, ro) < 0){
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