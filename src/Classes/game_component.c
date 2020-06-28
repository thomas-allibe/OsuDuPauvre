#include "Classes/game_component.h"

/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/

/* Constructor implementation */
/*-------------------------------------------------------------------------*/
/**
  @brief    Create an instance of GameComponent class
  @param    me     pointer for the new GameComponent
  @param    r      pointer to the renderer
  @param    t      pointer to the texture for this class
  @return   0 if succeeded, less than 0 if failed.

 */
/*--------------------------------------------------------------------------*/
int GameComponent_ctor(GameComponent * const me, SDL_Renderer *r, SDL_Texture *t){
    //me->is_source = SDL_TRUE; //This GC has allocated a SDL_Texture
    if(r == NULL){
        SDL_SetError("SDL_Renderer pointer NULL");
        return -1;
    }
    if(t == NULL){
        SDL_SetError("SDL_Texture pointer NULL");
        return -1;
    }
    me->renderer = r;
    me->texture = t;

    //Set width & height into position attribute
    if(SDL_QueryTexture(me->texture, NULL, NULL,
                                     &me->position.w, &me->position.h) != 0){
        #if GC_ERR_PRINT
        fprintf(stderr, "Erreur SDL_QuerryTexture : %s", SDL_GetError());
        #endif
        return -1;
    }
    
    return 0;
}

//Destructor prototype
/*-------------------------------------------------------------------------*/
/**
  @brief    Destroy a GameComponent instance
  @param    me     pointer to the GameComponent to free
  @return   void

 */
/*--------------------------------------------------------------------------*/
void GameComponent_dtor(GameComponent * const me){
    me->renderer = NULL;
    me->texture = NULL;
}

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/

/*-------------------------------------------------------------------------*/
/**
  @brief    Copy the texture on the renderer.
  @param    me     pointer to GameComponent
  @return   0 if succeeded, less than 0 if failed.

 */
/*--------------------------------------------------------------------------*/
int GameComponent_renderCopy(GameComponent * const me){
    if(SDL_RenderCopy(me->renderer, me->texture, NULL, &me->position) != 0){
        #if GC_ERR_PRINT
        fprintf(stderr, "Erreur SDL_RenderCopy : %s", SDL_GetError());
        #endif
        return -1;
    }
    return 0;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Set new x & y position
  @param    me     pointer to GameComponent
  @param    x      x position int pixels
  @param    y      y position int pixels
  @return   void

 */
/*--------------------------------------------------------------------------*/
void GameComponent_setXY(GameComponent *me, int x, int y){
    me->position.x = x;
    me->position.y = y;
}