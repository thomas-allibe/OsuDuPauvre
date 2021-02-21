#include "mySDL2_tools.h"

SDL_Texture* loadImageTarget(const char path[], ImageFormat f, SDL_Renderer *renderer){
    /*Texture modifiable + rendu*/
    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL, *tmp = NULL;

    switch(f){
        case BMP:
            surface = SDL_LoadBMP(path);
            break;
        case PNG:
            surface = IMG_Load(path);
            break;
        default:
            SDL_SetError("Bad ImageFormat (=%d) in loadImageStream()\n", f);
            return NULL;
    }

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

SDL_Texture* loadImageStatic(const char path[], ImageFormat f, SDL_Renderer *renderer){
    /*Texture non modifiable*/
    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;

    switch(f){
        case BMP:
            surface = SDL_LoadBMP(path);
            break;
        case PNG:
            surface = IMG_Load(path);
            break;
        default:
            SDL_SetError("Bad ImageFormat (=%d) in loadImageStream()\n", f);
            return NULL;
    }

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

SDL_Texture* loadImageStream(const char path[], ImageFormat f, SDL_Renderer *renderer){
    /*Texture modifiable*/

    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;
    size_t size;
    Uint32 *texturePixels, *surfacePixels;
    int pitch;

    switch(f){
        case BMP:
            surface = SDL_LoadBMP(path);
            break;
        case PNG:
            surface = IMG_Load(path);
            break;
        default:
            SDL_SetError("Bad ImageFormat (=%d) in loadImageStream()\n", f);
            return NULL;
    }

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
    if(SDL_LockSurface(surface) != 0){
        #ifndef MT_ERR_PRINT
        fprintf(stderr, "Erreur SDL_ : %s", SDL_GetError());
        #endif
        return NULL;
    }
    surfacePixels = surface->pixels;
    if(SDL_LockTexture(texture, NULL, (void**)&texturePixels, &pitch) != 0){
        #ifndef MT_ERR_PRINT
        fprintf(stderr, "Erreur SDL_LockTexture : %s", SDL_GetError());
        #endif
        return NULL;
    }
    for(size_t i=0 ; i<size ; i++){
        texturePixels[i] = surfacePixels[i];
    }
    SDL_UnlockTexture(texture);    
    SDL_UnlockSurface(surface);
    SDL_FreeSurface(surface);

    return texture;
}

int pixelsToFile(void *sdl_object, TextureFormat format, const char path[]){
    SDL_Texture *texture = NULL;
    SDL_Surface *surface = NULL;
    Uint32 *pixels = NULL;
    int w, h, pitch;

    if(sdl_object == NULL){
        return -1;
    }
    
/* ---------------------------------- Lock ---------------------------------- */

    switch(format){
        case Texture:
            texture = (SDL_Texture*)sdl_object;
            Uint32 pixelsFormat = SDL_PIXELFORMAT_RGBA8888;
            int textureAccess;
            SDL_QueryTexture(texture, &pixelsFormat, &textureAccess, &w, &h);
            
            if(textureAccess != SDL_TEXTUREACCESS_STREAMING){
                #ifndef MT_ERR_PRINT
                    fprintf(stderr, "Mauvais type de SDL_Texture dans pixelsToFile()\n");
                #endif
                return -1;
            }

            SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);
            break;
        case Surface:
            surface = (SDL_Surface*)sdl_object;
            SDL_LockSurface(surface);
            w = surface->w;
            h = surface->h;
            break;
        default:
            return -1;
    }

/* -------------------------- Write Pixels in file -------------------------- */

    FILE *f = NULL;
    f = fopen(path, "w+");
    if(f == NULL){
        #ifndef MT_ERR_PRINT
            fprintf(stderr, "Erreur ouverture du fichier : %s\n", path);
        #endif
        SDL_SetError("Erreur ouverture du fichier : %s\n", path);
        return -1;
    }

    fwrite(pixels, 4, w*h, f);
    
    fclose(f);
    f = NULL;

/* --------------------------------- Unlock --------------------------------- */

    switch(format){
        case Texture:
            SDL_UnlockTexture(texture);
            break;
        case Surface:
            SDL_UnlockSurface(surface);
            break;
        default:
            return -1;
    }
    
    return 0;
}