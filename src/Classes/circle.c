#include "Classes/circle.h"


/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/
//Constructor prototypes
/*-------------------------------------------------------------------------*/
/**
  @brief    Create an instance of Circle class
  @param    me     pointer for the new Circle
  @param    r      pointer to the renderer
  @param    t      pointer to the texture for this class
  @return   0 if succeeded, less than 0 if failed.

 */
/*--------------------------------------------------------------------------*/
int Circle_ctor(Circle * const me, SDL_Renderer *r, SDL_Texture *t, int radius){
    //SuperClass Constructor
    if(GameComponent_ctor(&me->super, r, t) != 0){
        return -1;
    }

    //Subclass constructor
    me->x_center = me->super.position.w / 2;
    me->y_center = me->super.position.h / 2;

    return 0;
}

//Destructor prototype
/*-------------------------------------------------------------------------*/
/**
  @brief    Destroy a Circle instance
  @param    me     pointer to the Circle to free
  @return   void

 */
/*--------------------------------------------------------------------------*/
void Circle_dtor(Circle * const me){
    //SuperClass Destructor
    GameComponent_dtor(&me->super);
}

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/

/*-------------------------------------------------------------------------*/
/**
  @brief    Return whether or not the click is on the circle
  @param    me     pointer to the Circle
  @param    x     x position of the mouse
  @param    y     y position of the mouse
  @return   return 1 if the mouse is on the circle, else return 0

 */
/*--------------------------------------------------------------------------*/
int Circle_is_click_on_circle(Circle * const me, int x, int y);