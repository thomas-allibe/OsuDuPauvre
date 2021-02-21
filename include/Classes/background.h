#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <stdlib.h>
#include "SDL2/SDL.h"
#include "mySDL2_tools.h"

/****************************************************************************
 *    PUBLIC TYPES
 ***************************************************************************/
//Background Attributes
typedef struct{
    SDL_Texture *texture;
    SDL_Renderer *renderer;
}Background;

/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/

/**
  @brief    Create an instance of Background class, using img for texture
  @param    r      pointer to the renderer
  @param    path   path to the texutre file
  @return   Pointer to the GameWindow instance. NULL if failed.
			Use BackGround_dtor to destroy it.
 */
Background* Background_ctor_file(SDL_Renderer *r, const char *path);

/**
  @brief    Create an instance of Background class, using a w*h filled rectangle for texture
  @param    r   pointer to the renderer
  @param	pf	pointer to SDL_PixelFormat
  @param    c   color to fill the texture with
  @param    w   width of background texture
  @param    h   height of background texture
  @return   Pointer to the GameWindow instance. NULL if failed.
			Use BackGround_dtor to destroy it.
 */
Background* Background_ctor_color(SDL_Renderer *r, SDL_PixelFormat *pf, SDL_Color c, size_t w, size_t h);

/**
  @brief    Destroy a Background instance
  @param    me     pointer to the Background to free
  @return   void
 */
void Background_dtor(Background *me);

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/

/**
  @brief    Destroy a Background instance
  @param	me     pointer to the Background to free
  @return	void
 */
int Background_draw(const Background *me);

#endif /* BACKGROUND_H */