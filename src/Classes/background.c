#include "Classes/background.h"

/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/

Background* Background_ctor_file(SDL_Renderer *r, const char *path){

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
    
    return me;
}

Background* Background_ctor_color(SDL_Renderer *r, SDL_PixelFormat *pf, SDL_Color c, size_t w, size_t h){

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

    return me;
}

void Background_dtor(Background *me){
    me->renderer = NULL;
    if(me->texture != NULL){
        SDL_DestroyTexture(me->texture);
        me->texture = NULL;
    }
    if(me != NULL)
        free(me);
}

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/

int Background_draw(const Background *me){
	return SDL_RenderCopy(me->renderer, me->texture, NULL, NULL);
}