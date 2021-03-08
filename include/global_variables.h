#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include <SDL2/SDL.h>

/* ------------------------------ Global Define ----------------------------- */

#define MS_PER_UPDATE 20

/* ------------------------------ Global Assets ----------------------------- */

#define ASSET_TEXTURE_PAUSE_BG "../images/pause-overlay.png"
#define ASSET_TEXTURE_CIRCLE "../images/hitcircle_2.png"
#define ASSET_TEXTURE_CIRCLE_HOVERLAY "../images/hitcircleoverlay.png"
#define ASSET_TEXTURE_CIRCLE_APPROACH "../images/approachcircle.png"
#define ASSET_TEXTURE_CIRCLE_MISS "../images/hit0@2x.png"
#define ASSET_TEXTURE_CIRCLE_50 "../images/hit50.png"
#define ASSET_TEXTURE_CIRCLE_100 "../images/hit100.png"
#define ASSET_TEXTURE_CIRCLE_300 "../images/hit300.png"

#define ASSET_SOUND_HIT "../sounds/drum-hitclap.wav"
#define ASSET_SOUND_BREAK "../sounds/combobreak.wav"
#define ASSET_SOUND_COUNT "../sounds/count1s.wav"

#define ASSET_MUSIC_DIR "../sounds/music/"

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
    SDL_Keycode controls_restart;
    SDL_Keycode controls_pause;
//Game
    Uint8 game_AR;
}Settings;

/* ------------------------------- Global enum ------------------------------ */


/* -------------------------- Global variables (GV) ------------------------- */

extern const char *GAME_TITLE;
extern Settings GAME_SETTINGS;

#endif /* GLOBAL_VARIABLES_H */