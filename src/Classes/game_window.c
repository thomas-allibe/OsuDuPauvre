#include "Classes/game_window.h" /* GameWindow Class Interface*/

/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/
/* Constructor implementation */
/*-------------------------------------------------------------------------*/
/**
  @brief    Create an instance of GameWindow class
  @param    me     pointer for the new GameWindow
  @param    w_op   struct for window settings  
  @param    r_op   struct for renderer settings  
  @return   0 if succeeded, less than 0 if failed.

 */
/*--------------------------------------------------------------------------*/
int GameWindow_ctor(GameWindow * const me, WindowOptions *w_op, RendererOptions *r_op){
    //Be sure pointers are equal to NULL
    me->window = NULL;
    me->renderer = NULL;

//### Create Windows

    me->window = SDL_CreateWindow(w_op->title, w_op->x, w_op->y,
                                            w_op->w, w_op->h,
                                            w_op->flags);
    if(me->window == NULL){
        #if GW_ERR_PRINT
        fprintf(stderr, "Error SDL_CreateWindow : %s", SDL_GetError());
        #endif
        return -1;
    }
    
//### Create Renderer

    me->renderer = SDL_CreateRenderer(me->window, r_op->index, r_op->flags);
    if(me->renderer == NULL){
        //Destroy the window
        SDL_DestroyWindow(me->window); me->window = NULL;
        #if GW_ERR_PRINT
        fprintf(stderr, "Error SDL_CreateRenderer : %s", SDL_GetError());
        #endif
        return -1;
    }

    return 0;
}

/* Destructor implementation */
/*-------------------------------------------------------------------------*/
/**
  @brief    Destroy a GameWindow instance
  @param    me     pointer to the GameWindow to free
  @return   void

 */
/*--------------------------------------------------------------------------*/
void GameWindow_dtor(GameWindow * const me){
    SDL_DestroyRenderer(me->renderer);
        me->renderer = NULL;
    SDL_DestroyWindow(me->window);
        me->window = NULL;
}

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/
/*-------------------------------------------------------------------------*/
/**
  @brief    Change the icon of the GameWindow instance
  @param    me     pointer to the GameWindow
  @param    path   path to the icon
  @return   0 if succeeded, if failed

 */
/*--------------------------------------------------------------------------*/
int GameWindow_setIcon(GameWindow * const me, const char *path){
    SDL_Surface *surface = NULL;

    surface = SDL_LoadBMP(path);
    if(surface == NULL){
        #if GW_ERR_PRINT
        fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
        #endif
        return -1;
    }

    SDL_SetWindowIcon(me->window, surface);

    SDL_FreeSurface(surface);

    return 0;
}