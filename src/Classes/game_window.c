#include "Classes/game_window.h" /* GameWindow Class Interface*/

/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/
GameWindow* GameWindow_ctor(WindowOptions *w_op, RendererOptions *r_op){

/* --------------------------------- Assert --------------------------------- */

    SDL_assert(w_op);
    SDL_assert(r_op);

/* ---------------------------- Memory Allocation --------------------------- */
    
    GameWindow *me = NULL;
    me = (GameWindow*) malloc(sizeof(GameWindow));
    if(me == NULL){
        SDL_SetError("Erreur malloc : GameWindow_ctor()");
        return NULL;
    }

/* -------------------------------- Set NULL -------------------------------- */

    me->window = NULL;
    me->renderer = NULL;

/* ----------------------------- Create Windows ----------------------------- */
    
    me->window = SDL_CreateWindow(w_op->title, w_op->x, w_op->y,
                                            w_op->w, w_op->h,
                                            w_op->flags);

    if(me->window == NULL){
        GameWindow_dtor(me);
        return NULL;
    }
    
/* ----------------------------- Create Renderer ---------------------------- */

    me->renderer = SDL_CreateRenderer(me->window, r_op->index, r_op->flags);
    if(me->renderer == NULL){
        GameWindow_dtor(me);
        return NULL;
    }
    if(SDL_RenderClear(me->renderer)){
        GameWindow_dtor(me);
        return NULL;
    }
    if(SDL_SetRenderDrawBlendMode(me->renderer, SDL_BLENDMODE_NONE)){
        GameWindow_dtor(me);
        return NULL;
    }

    return me;
}

void GameWindow_dtor(GameWindow *me){
    SDL_assert(me);

    if(me->renderer != NULL){
        // SDL_RenderFlush(me->renderer);
        SDL_DestroyRenderer(me->renderer);
        me->renderer = NULL;
    }
    if(me->window != NULL){
        SDL_DestroyWindow(me->window);
        me->window = NULL;
    }
    free(me);
}

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/

int GameWindow_setIcon(GameWindow *me, const char *path){
    SDL_Surface *surface = NULL;

    SDL_assert(me);
    SDL_assert(path);

    surface = SDL_LoadBMP(path);
    if(surface == NULL){
        return -1;
    }
    SDL_SetWindowIcon(me->window, surface);
    SDL_FreeSurface(surface);

    return 0;
}