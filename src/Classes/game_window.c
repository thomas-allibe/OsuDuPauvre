#include "Classes/game_window.h" /* GameWindow Class Interface*/

/* Constructor implementation */
int GameWindow_ctor(GameWindow * const me, WindowOptions w_op, RendererOptions r_op){
    //Be sure pointers are equal to NULL
    me->window = NULL;
    me->renderer = NULL;

/*------------------------------------Create Windows------------------------------------*/
    
    me->window = SDL_CreateWindow(w_op.title, w_op.x, w_op.y,
                                            w_op.w, w_op.h,
                                            w_op.flags);
    if(me->window == NULL){
        fprintf(stderr, "Error SDL_CreateWindow : %s", SDL_GetError());
        return -1;
    }
    
/*------------------------------------Create Renderer-----------------------------------*/

    me->renderer = SDL_CreateRenderer(me->window, r_op.index, r_op.flags);
    if(me->renderer == NULL){
        fprintf(stderr, "Error SDL_CreateRenderer : %s", SDL_GetError());
        return -2;
    }

    return 0;
}

/* Destructor implementation */
void GameWindow_dtor(GameWindow * const me){
    SDL_DestroyRenderer(me->renderer);
    SDL_DestroyWindow(me->window);
}