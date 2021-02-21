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
    me->rect_pos = *pos;
    me->radius = radius;
    me->creation_time = SDL_GetTicks();
    me->remaining_time = 500;
    me->x_center = pos->x + pos->w/2;
    me->y_center = pos->y + pos->h/2;

    return me;
}

void Circle_dtor(Circle *me){
    if(me != NULL)
        free(me);
}

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/
int Circle_is_pos_on_circle(Circle *me, int x, int y){
    float distance = sqrt((x-me->x_center)*(x-me->x_center)
                        +(y-me->y_center)*(y-me->y_center));
    return (distance <= me->radius)?1:0;
}

int Circle_draw(Circle *me){
    if(SDL_RenderCopy(me->renderer, me->textures->circle, NULL, &(me->rect_pos)) != 0){
        return -1;
    }
    if(SDL_RenderCopy(me->renderer, me->textures->overlay, NULL, &(me->rect_pos))){
        return -1;
    }
    return SDL_RenderCopy(me->renderer, me->textures->approach, NULL, &(me->rect_pos));
    // return 0;
}