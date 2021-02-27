#include "Classes/circle.h"


/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/
Circle* Circle_ctor(SDL_Renderer *r, Circle_Textures *ct, int radius, SDL_Rect *pos){

/* ---------------------------- Memory Allocation --------------------------- */
    
    Circle *me = NULL;
    me = (Circle*) malloc(sizeof(Circle));
    if(me == NULL){
        SDL_SetError("Erreur malloc : Circle_ctor()");
        return NULL;
    }
    
    me->renderer = r;
    me->textures = ct;
    me->pos = *pos;
    me->radius = radius;
    me->x_center = pos->x + pos->w/2;
    me->y_center = pos->y + pos->h/2;

// ### Approach circle pos & size
    if(GAME_SETTINGS.game_AR > 5){
        me->duration = 1200 + 600*(5 - GAME_SETTINGS.game_AR)/5;
    }
    else if(GAME_SETTINGS.game_AR < 5){
        me->duration = 1200 - 750*(GAME_SETTINGS.game_AR - 5)/5;
    }
    else{
        me->duration = 1200;
    }
    me->time_left = me->duration;
    SDL_QueryTexture(me->textures->approach, NULL, NULL, &me->ac_w_end, &me->ac_h_end);
    me->ac_w_start = me->ac_w_end * 3;
    me->ac_h_start = me->ac_h_end * 3;

    me->ac_pos.w = me->ac_w_start;
    me->ac_pos.h = me->ac_h_start;
    me->ac_pos.x = me->x_center - me->ac_pos.w/2;
    me->ac_pos.y = me->y_center - me->ac_pos.h/2;
    
    return me;
}

void Circle_dtor(Circle *me){
    if(me != NULL)
        free(me);
}

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/

void Circle_update(Circle *me, int dt){
    me->time_left -= dt;
}

int Circle_draw(Circle *me, double prediction){
    if(SDL_RenderCopy(me->renderer, me->textures->circle, NULL, &(me->pos))){
        return -1;
    }
    if(SDL_RenderCopy(me->renderer, me->textures->overlay, NULL, &(me->pos))){
        return -1;
    }
    //Compute new approach circle dimensions w, h then x, y position
    me->ac_pos.w = ((me->ac_w_start-me->ac_w_end)*(me->time_left-prediction*MS_PER_UPDATE))
                    /me->duration + me->ac_w_end;
    me->ac_pos.h = ((me->ac_h_start-me->ac_h_end)*(me->time_left-prediction*MS_PER_UPDATE))
                    /me->duration + me->ac_h_end;
    me->ac_pos.x = me->x_center - me->ac_pos.w/2;
    me->ac_pos.y = me->y_center - me->ac_pos.h/2;
    return SDL_RenderCopy(me->renderer, me->textures->approach, NULL, &(me->ac_pos));
}

int Circle_is_pos_on_circle(Circle *me, int x, int y){
    float distance = sqrt((x-me->x_center)*(x-me->x_center)
                        +(y-me->y_center)*(y-me->y_center));
    return (distance <= me->radius)?1:0;
}

int Circle_no_time_left(Circle *me){
    return me->time_left <= 0;
}