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

typedef enum{
	Texture,
	Surface
}TextureFormat;

/**
  @brief    Create a target texture from an image file
  @param    path   path to the BMP file
  @param    f      format of the image file
  @param    renderer    pointer to the renderer to link with the new texture
  @return   0 if succeeded, less than 0 if failed.

  SDL_image must be initialized with the flag corresponding to f
  eg: IMG_Init(IMG_INIT_PNG)
 */
SDL_Texture* loadImageTarget(const char path[], ImageFormat f, SDL_Renderer *renderer);

/**
  @brief    Create a static texture from an image file
  @param    path   path to the BMP file
  @param    f      format of the image file
  @param    renderer    pointer to the renderer to link with the new texture
  @return   0 if succeeded, less than 0 if failed.

  SDL_image must be initialized with the flag corresponding to f
  eg: IMG_Init(IMG_INIT_PNG)
 */
SDL_Texture* loadImageStatic(const char path[], ImageFormat f, SDL_Renderer *renderer);

/**
  @brief    Create a stream texture from an image file
  @param    path   path to the BMP file
  @param    f      format of the image file
  @param    renderer    pointer to the renderer to link with the new texture
  @return   0 if succeeded, less than 0 if failed.

  SDL_image must be initialized with the flag corresponding to f
  eg: IMG_Init(IMG_INIT_PNG)
 */
SDL_Texture* loadImageStream(const char path[], ImageFormat f, SDL_Renderer *renderer);

/**
  @brief	Export pixels from SDL_Texture or SDL_Surface to a file.
  @param	sdl_object	SDL_Texture or SDL_Surface
  @param	path		path for the file
  @return	0 if succeeded, less if failed.
 */
int pixelsToFile(void *sdl_object, TextureFormat format, const char path[]);

#endif /* MYSDL2_TOOLS_H */