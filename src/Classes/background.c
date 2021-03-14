#include "Classes/background.h"

/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/

Background* Background_ctor_file(SDL_Renderer *r, const char *path){

/* --------------------------------- Assert --------------------------------- */

    SDL_assert(r);
    SDL_assert(path);

/* ---------------------------- Memory Allocation --------------------------- */
    
    Background *me = NULL;
    me = (Background*) malloc(sizeof(Background));
    if(me == NULL){
        return NULL;
    }

/* -------------------------------- Set  NULL ------------------------------- */

    me->texture = NULL;

/* ----------------------------- Texture Loading ---------------------------- */

    me->renderer = r;
    me->texture = loadImageStatic(path, PNG, r);
    if(me->texture == NULL){
        Background_dtor(me);
        return NULL;
    }

/* -------------------------------- Set Mode -------------------------------- */

    me->mode = BG_DrawMode_Full;
    me->pos.x = 0;
    me->pos.y = 0;
    SDL_GetRendererOutputSize(me->renderer, &me->pos.w, &me->pos.h);

    return me;
}

Background* Background_ctor_color(SDL_Renderer *r, SDL_PixelFormat *pf, SDL_Color c, size_t w, size_t h){

/* --------------------------------- Assert --------------------------------- */

    SDL_assert(r);
    SDL_assert(pf);
    SDL_assert(pf);
    SDL_assert(w > 0);
    SDL_assert(h > 0);

/* -------------------------------- Variables ------------------------------- */
    
    int pitch = sizeof(Uint32) * w;
    Uint32 *texturePixels = NULL;

/* ---------------------------- Memory Allocation --------------------------- */
    
    Background *me = NULL;
    me = (Background*) malloc(sizeof(Background));
    if(me == NULL){
        SDL_SetError("Erreur malloc : Background_ctor_color()");
        return NULL;
    }

    texturePixels = (Uint32*) malloc(sizeof(Uint32) * w * h);
    if(texturePixels == NULL){
        SDL_SetError("Erreur malloc : Background_ctor_color()");
        return NULL;
    }

    // SDL_PixelFormat *pixelFormat = NULL;
    // pixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    // if(pixelFormat == NULL){
    //     SDL_SetError("Erreur malloc : Background_ctor_color()");
    //     return NULL;
    // }
/* -------------------------------- Set NULL -------------------------------- */

    me->texture = NULL;

/* ---------------------------- Texture Creation ---------------------------- */

    me->renderer = r;
    me->texture = SDL_CreateTexture(r,
                                    SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_STATIC,
                                    w, h);
    if(me->texture == NULL){
        Background_dtor(me);
        // free(texturePixels);
        return NULL;
    }

    for(size_t i=0 ; i<(w*h) ; i++){
        texturePixels[i] = SDL_MapRGBA(pf, c.r, c.g, c.b, c.a);
    }
    
    if(SDL_UpdateTexture(me->texture, NULL, texturePixels, pitch) != 0){
        Background_dtor(me);
        free(texturePixels);
        return NULL;
    }

    // SDL_FreeFormat(pixelFormat);
    free(texturePixels);

/* -------------------------------- Set Mode -------------------------------- */

    me->mode = BG_DrawMode_Full;
    me->pos.x = 0;
    me->pos.y = 0;
    SDL_GetRendererOutputSize(me->renderer, &me->pos.w, &me->pos.h);

    return me;
}

void Background_dtor(Background *me){
    SDL_assert(me);
    
    me->renderer = NULL;
    if(me->texture != NULL){
        SDL_DestroyTexture(me->texture);
        me->texture = NULL;
    }
    free(me);
}

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/

int Background_draw(const Background *me){
	SDL_assert(me);

    return SDL_RenderCopy(me->renderer, me->texture, NULL, &me->pos);
}

int Background_set_draw_mode(Background *me, BG_DrawMode mode, float *scale, SDL_Rect *custom){
    SDL_Rect new_pos = {0, 0, 0, 0};
    int w=0, h=0;
    int result = 0;

    SDL_assert(me);
    switch(mode){
        case BG_DrawMode_Full:
            result = SDL_GetRendererOutputSize(me->renderer, &new_pos.w, &new_pos.h);
            if(result){
                return result;
            }
            break;
        case BG_DrawMode_Scaled_Centered:
            SDL_assert(scale);
            SDL_assert(*scale > 0);

            if(SDL_QueryTexture(me->texture, NULL, NULL, &new_pos.w, &new_pos.h)){
                return -1;
            }
            result = SDL_GetRendererOutputSize(me->renderer, &w, &h);
            if(result){
                return result;
            }
            new_pos.w *= *scale;
            new_pos.h *= *scale;
            new_pos.x = (float)w/2 - (float)new_pos.w/2;
            new_pos.y = (float)h/2 - (float)new_pos.h/2;
            break;
        case BG_DrawMode_Custom:
            SDL_assert(custom);
            SDL_assert(custom->w<=0 || custom->h<=0);
            
            new_pos = *custom;
            break;
        default:
            SDL_SetError("In Background_set_draw_mode(), unknown mode selected: %d !", mode);
            return -1;
    }
    me->mode = mode;
    me->pos = new_pos;
    return 0;
}