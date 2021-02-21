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
// #include "initialize_functions.h"
#include "my_events.h"
#include "iniparser/iniparser.h"
#include "Classes/game_window.h"
#include "Classes/gameboard.h"
#include "Classes/background.h"
/*
#include "Classes/game_component.h"
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

/* -------------------------------- Variables ------------------------------- */

    Input user_input = {0, 0, SDL_FALSE, SDL_FALSE, SDL_FALSE, SDL_FALSE, SDL_FALSE, SDL_FALSE};
    int statut = EXIT_SUCCESS;
    Uint32 t_current;
    Uint32 t_previous;
    Uint32 t_lag = 0;
    Uint32 t_elapsed;

    //Classes
    GameWindow *gw = NULL;
        //Settings
        WindowOptions *wo = malloc(sizeof(WindowOptions));
        wo->title = GAME_TITLE;
        wo->x = SDL_WINDOWPOS_CENTERED; wo->y = SDL_WINDOWPOS_CENTERED;
        wo->flags = SDL_WINDOW_SHOWN;

        RendererOptions *ro = malloc(sizeof(RendererOptions));
        ro->index = -1;
        ro->flags = SDL_RENDERER_ACCELERATED;
    
    GameBoard *gb = NULL;

/* ----------------------------- Verif Arguments ---------------------------- */

    // Uint32 delay = 2000;
    // if(argc > 1){
	//     delay = atoi(argv[1]);
	// }

/* ------------------------------ Initalization ----------------------------- */
    
    fprintf(stdout, "Start Initialize\n");
    //No error
    setDefaultSettingsGV();
    if(libInit() != 0){
        statut = EXIT_FAILURE;
        goto Error;
    }
    if(getIniSettings("settings.ini") != 0){
        statut = EXIT_FAILURE;
        goto Error;
    }

    wo->w = GAME_SETTINGS.window_w;
    wo->h = GAME_SETTINGS.window_h;

    gw = GameWindow_ctor(wo, ro);
    if(gw == NULL){
        statut = EXIT_FAILURE;
        goto Error;
    }
    free(wo); wo = NULL;
    free(ro); ro = NULL;

    if(GameWindow_setIcon(gw, "../images/w_ico_1.bmp") != 0){
        statut = EXIT_FAILURE;
        goto Error;
    }
    gb = GameBoard_ctor(gw->renderer);
    if(gb == NULL){
        statut = EXIT_FAILURE;
        goto Error;
    }
    fprintf(stdout, "End Initialize\n");
    t_previous = SDL_GetTicks();

/* -------------------------------- Main Loop ------------------------------- */

    while(1){
    /* ---------------------------------- Time ---------------------------------- */

        t_current = SDL_GetTicks();
        t_elapsed = t_current - t_previous;
        t_previous = t_current;
        t_lag += t_elapsed;

    /* -------------------------------- UserEvent ------------------------------- */

        getUserEvent(&user_input);
        if(user_input.quit == SDL_TRUE)
            goto Quit;

    /* ----------------------------- EventProcessing ---------------------------- */

        while(t_lag >= MS_PER_UPDATE){
            GameBoard_processEvent(gb, &user_input);
            GameBoard_update(gb);
            t_lag -= MS_PER_UPDATE;
        }

    /* --------------------------------- Render --------------------------------- */
        
        if(GameBoard_render(gb, t_lag/MS_PER_UPDATE) != 0){
            statut = EXIT_FAILURE;
            goto Error;
        }

    }

/* ---------------------------------- Error --------------------------------- */

Error:
    fprintf(stderr, "Error:\n%s\n", SDL_GetError());

/* ---------------------------------- Quit ---------------------------------- */
Quit:
    fprintf(stderr, "\nQuit\n");
    // if(gb != NULL)
        GameBoard_dtor(gb);
    if(gw != NULL)
        GameWindow_dtor(gw);
    libDeInit();

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