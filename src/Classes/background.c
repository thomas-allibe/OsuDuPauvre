#include "Classes/background.h"

/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/

/* Constructor implementation */
/*-------------------------------------------------------------------------*/
/**
  @brief    Create an instance of Background class
  @param    me     pointer for the new Background
  @param    r      pointer to the renderer
  @param    path   path to the texutre file
  @return   0 if succeeded, less than 0 if failed.

 */
/*--------------------------------------------------------------------------*/
int Background_ctor(Background * const me, SDL_Renderer *r, const char *path){
    //Subclass attributes
    SDL_Texture *texture = NULL;
    texture = loadImageStatic(path, PNG, r);
    if(texture == NULL){
        #ifndef BG_ERR_PRINT
        fprintf(stderr, "Erreur loadImageStatic : %s", SDL_GetError());
        #endif
        return -1;
    }
    
    //Superclass constructor
    if(GameComponent_ctor(&me->super, r, texture) != 0){
      return -1;
    }

    return 0;
}

//Destructor prototype
/*-------------------------------------------------------------------------*/
/**
  @brief    Destroy a Background instance
  @param    me     pointer to the Background to free
  @return   void

    Detect if it's a source GC
 */
/*--------------------------------------------------------------------------*/
void Background_dtor(Background * const me){
    //Subclass destructor
    SDL_DestroyTexture(me->super.texture);

    //Superclass destructor
    GameComponent_dtor(&me->super);
}

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/

