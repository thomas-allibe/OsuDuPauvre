#include "initialize_functions.h"

/*-------------------------------------------------------------------------*/
/**
  @brief    create a game_window instance
  @param    settings     settings dictionary
  @param    gw     pointer to uninitialize GameWindow instance
  @param    title     game title for window title
  @return   0 if ok, -1 if error

 */
/*--------------------------------------------------------------------------*/
int createMainWindow(GameWindow *gw){
/*---------------------------------Variables--------------------------------*/
    //Options
    WindowOptions wo;
    wo.title = GAME_TITLE;
    wo.x = SDL_WINDOWPOS_CENTERED; wo.y = SDL_WINDOWPOS_CENTERED;
    wo.w = GAME_SETTINGS.window_w;
    wo.h = GAME_SETTINGS.window_h;
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