#ifndef MYSDL2_TOOLS_H
#define MYSDL2_TOOLS_H

#define MT_ERR_PRINT 0 //0: no fprintf of SDL_GetError()

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

typedef enum{
  BMP,
  PNG
}ImageFormat;

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
SDL_Texture* loadImageTarget(char path[], ImageFormat f, SDL_Renderer *renderer);

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
SDL_Texture* loadImageStatic(char path[], ImageFormat f, SDL_Renderer *renderer);

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
SDL_Texture* loadImageStream(char path[], ImageFormat f, SDL_Renderer *renderer);

#endif /* MYSDL2_TOOLS_H */