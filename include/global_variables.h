#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include <SDL2/SDL.h>

/* ------------------------------ Global Define ----------------------------- */

#define MS_PER_UPDATE 16

/* ------------------------------ Global Struct ----------------------------- */

typedef struct{
    //If you add parameters,
    //add them in initialize_functions.h #define section
//Window section
    int window_w;
    int window_h;
//Controls section
    SDL_Keycode controls_hit1;
    SDL_Keycode controls_hit2;
}Settings;

/* ------------------------------- Global enum ------------------------------ */

typedef enum {
/* 0 */ Initialize,
/* 1 */ GetUserEvent,
/* 2 */ Wait,
/* 3 */ UpdateObjects,
/* 4 */ RenderChanges,
/* 5 */ Error,
/* 6 */ Quit,
/* 7 */ Stop
}State;


/* -------------------------- Global variables (GV) ------------------------- */

extern const char *GAME_TITLE;
extern Settings GAME_SETTINGS;

#endif /* GLOBAL_VARIABLES_H */