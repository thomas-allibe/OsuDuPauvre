#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <stdlib.h>
#include "SDL2/SDL.h"
#include "mySDL2_tools.h"

/****************************************************************************
 *    PUBLIC TYPES
 ***************************************************************************/
//Background Attributes
typedef enum{
    BG_DrawMode_Full,
    BG_DrawMode_Scaled_Centered,
    BG_DrawMode_Custom
}BG_DrawMode;

typedef struct{
    SDL_Texture *texture;
    SDL_Renderer *renderer;
    SDL_Rect pos;
    BG_DrawMode mode;
}Background;

/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/

/**
  @brief    Create an instance of Background class, using img for texture
            Default DrawMode is BG_DrawMode_Full
  @param    r      pointer to the renderer
  @param    path   path to the texutre file
  @return   Pointer to the BackGround instance. NULL if failed.
			Use BackGround_dtor to destroy it.
 */
Background* Background_ctor_file(SDL_Renderer *r, const char *path);

/**
  @brief    Create an instance of Background class, using a w*h filled rectangle for texture
            Default DrawMode is BG_DrawMode_Full
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
  @brief    Draw the background on the associated renderer
  @param	me     pointer to the Background to free
  @return	void
 */
int Background_draw(const Background *me);

/**
  @brief    Set the draw mode for Background instance.
            Full: Render at full scale (fit to renderer),
            Scaled_Centered: Render scaled BG, centered on the render,
            Custom: Render using custom SDL_Rect position
  @param	me      pointer to the Background to free
  @param    scale   pointer to scale, to be used with Scaled_Centered mode
                    Uses 1 if NULL, ignored if another mode is used.
  @param    custom  SDL_Rect position to be used with Custom mode
                    Uses x,y = 0 with BG dimensions if NULL.
                    Ignored if another mode is used.
  @return	0 on success, -1 on faillure (keep previous settings in that case)
 */
int Background_set_draw_mode(Background *me, BG_DrawMode mode, float *scale, SDL_Rect *custom);

void Background_set_scale(Background *me);

#endif /* BACKGROUND_H */