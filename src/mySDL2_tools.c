#include "mySDL2_tools.h"

/*-------------------------------------------------------------------------*/
/**
  @brief    Create a target texture from an image file
  @param    path   path to the BMP file
  @param    f      format of the image file
  @param    renderer    pointer to the renderer to link with the new texture
  @return   0 if succeeded, less than 0 if failed.

  SDL_image must be initialized with the flag corresponding to f
  eg: IMG_Init(IMG_INIT_PNG)
 */
/*--------------------------------------------------------------------------*/
SDL_Texture* loadImageTarget(char path[], ImageFormat f, SDL_Renderer *renderer){
    /*Texture modifiable + rendu*/
    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL, *tmp = NULL;

    if(f == BMP)
        surface = SDL_LoadBMP(path);
    if(f == PNG)
        surface = IMG_Load(path);

    if(surface == NULL){
        #ifndef MT_ERR_PRINT
        fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
        #endif
        return NULL;
    }

    tmp = SDL_CreateTextureFromSurface(renderer, surface);
    if(tmp == NULL){
        #ifndef MT_ERR_PRINT
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        #endif
        return NULL;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
                            SDL_TEXTUREACCESS_TARGET, surface->w, surface->h);
    if(texture == NULL){
        #ifndef MT_ERR_PRINT
        fprintf(stderr, "Erreur SDL_CreateTexture : %s", SDL_GetError());
        #endif
        return NULL;
    }

    SDL_SetRenderTarget(renderer, texture);
    SDL_RenderCopy(renderer, tmp, NULL, NULL);
    SDL_DestroyTexture(tmp);
    SDL_FreeSurface(surface);
    SDL_SetRenderTarget(renderer, NULL);

    return texture;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Create a static texture from an image file
  @param    path   path to the BMP file
  @param    f      format of the image file
  @param    renderer    pointer to the renderer to link with the new texture
  @return   0 if succeeded, less than 0 if failed.

  SDL_image must be initialized with the flag corresponding to f
  eg: IMG_Init(IMG_INIT_PNG)
 */
/*--------------------------------------------------------------------------*/
SDL_Texture* loadImageStatic(char path[], ImageFormat f, SDL_Renderer *renderer){
    /*Texture non modifiable*/
    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;

    if(f == BMP)
        surface = SDL_LoadBMP(path);
    if(f == PNG)
        surface = IMG_Load(path);

    if(surface == NULL){
        #ifndef MT_ERR_PRINT
        fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
        #endif
        return NULL;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(texture == NULL){
        #ifndef MT_ERR_PRINT
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        #endif
        return NULL;
    }

    SDL_FreeSurface(surface);

    return texture;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Create a stream texture from an image file
  @param    path   path to the BMP file
  @param    f      format of the image file
  @param    renderer    pointer to the renderer to link with the new texture
  @return   0 if succeeded, less than 0 if failed.

  SDL_image must be initialized with the flag corresponding to f
  eg: IMG_Init(IMG_INIT_PNG)
 */
/*--------------------------------------------------------------------------*/
SDL_Texture* loadImageStream(char path[], ImageFormat f, SDL_Renderer *renderer){
    /*Texture modifiable*/

    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;
    size_t size;
    Uint32 *texturePixels, *surfacePixels;

    if(f == BMP)
        surface = SDL_LoadBMP(path);
    if(f == PNG)
        surface = IMG_Load(path);

    if(surface == NULL){
        #ifndef MT_ERR_PRINT
        fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
        #endif
        return NULL;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
                            SDL_TEXTUREACCESS_STREAMING, surface->w, surface->h);
    if(texture == NULL){
        #ifndef MT_ERR_PRINT
        fprintf(stderr, "Erreur SDL_CreateTexture : %s", SDL_GetError());
        #endif
        return NULL;
    }
    size = surface->w * surface->h;
    SDL_LockSurface(surface);
    surfacePixels = surface->pixels;
    SDL_LockTexture(texture, NULL, (void**)&texturePixels, NULL);
    for(size_t i=0 ; i<size ; i++){
        texturePixels[i] = surfacePixels[i];
    }
    SDL_UnlockTexture(texture);    
    SDL_LockSurface(surface);
    SDL_FreeSurface(surface);

    return texture;
}