#include "Classes/gameboard.h"

/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/
GameBoard* GameBoard_ctor(SDL_Renderer *renderer){
    
/* -------------------------------- Variables ------------------------------- */

    Uint32 *pixels = NULL;
    int pitch = 0;
    Uint8 r = 0, g = 0, b = 0, a = 0;
    SDL_Rect item_pos = {0, 0, 0, 0};
    int radius = 0;

/* ---------------------------- Memory Allocation --------------------------- */

    GameBoard *me = NULL;
    me = (GameBoard*) malloc(sizeof(GameBoard));
    if(me == NULL){
        return NULL;
    }

    me->pixel_format = NULL;
    me->pixel_format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    if(me->pixel_format == NULL){
        GameBoard_dtor(me);
        return NULL;
    }

/* -------------------------------- Set NULL -------------------------------- */

    me->background = NULL;
    me->circle_textures = (Circle_Textures){NULL, NULL, NULL};
    me->items = NULL;
    me->test_circle = NULL;

/* -------------------------------- SDL_video ------------------------------- */

    me->renderer = renderer;

/* --------------------------- Objects / Textures --------------------------- */

// ### Background
    // me->background = Background_ctor_file(me->renderer, "../images/bg_1.png");
    me->background = Background_ctor_color(me->renderer,
                                                        me->pixel_format,
                                                        (SDL_Color){10, 10, 10, 255},
                                                        GAME_SETTINGS.window_w,
                                                        GAME_SETTINGS.window_h);
    if(me->background == NULL){
        GameBoard_dtor(me);
        return NULL;
    }

// ### Circle Texture
    me->circle_textures.circle = loadImageStream("../images/hitcircle_2.png", PNG, renderer);
    if(me->circle_textures.circle == NULL){
        GameBoard_dtor(me);
        return NULL;
    }
    if(SDL_SetTextureBlendMode(me->circle_textures.circle, SDL_BLENDMODE_BLEND)){
        GameBoard_dtor(me);
        return NULL;
    }
    if(SDL_QueryTexture(me->circle_textures.circle, NULL, NULL, &item_pos.w, &item_pos.h)){
        GameBoard_dtor(me);
        return NULL;
    }
    if(SDL_LockTexture(me->circle_textures.circle, NULL, (void**)&pixels, &pitch)){
        GameBoard_dtor(me);
        return NULL;
    }
    for(int i=0 ; i<(item_pos.w*item_pos.h) ; i++){
        SDL_GetRGBA(pixels[i], me->pixel_format, &r, &g, &b, &a);
        if(a != 0){
            pixels[i] = SDL_MapRGBA(me->pixel_format, 164, 100, 240, a);
        }//164, 100, 240
    }
    
    //Get radius by measuring opaque texture from center to center-right
    a = 255;
    for(int i=item_pos.w/2 ; i<item_pos.w && a>20 ; i++){
        SDL_GetRGBA(pixels[i + item_pos.w*item_pos.h/2], me->pixel_format, &r, &g, &b, &a);
        radius++;
    }
    // fprintf(stdout, "circle radius = %d\n", radius);
    SDL_UnlockTexture(me->circle_textures.circle);

// ### Circle Overlay Texture
    me->circle_textures.overlay = loadImageStatic("../images/hitcircleoverlay.png", PNG, renderer);
    if(me->circle_textures.overlay == NULL){
        GameBoard_dtor(me);
        return NULL;
    }
    if(SDL_SetTextureBlendMode(me->circle_textures.circle, SDL_BLENDMODE_BLEND)){
        GameBoard_dtor(me);
        return NULL;
    }
// ### Approach circle
    me->circle_textures.approach = loadImageStatic("../images/approachcircle.png", PNG, renderer);
    if(me->circle_textures.approach == NULL){
        GameBoard_dtor(me);
        return NULL;
    }
    if(SDL_SetTextureBlendMode(me->circle_textures.approach, SDL_BLENDMODE_BLEND)){
        GameBoard_dtor(me);
        return NULL;
    }

// ### Test Circle
    item_pos.x = 50;
    item_pos.y = 50;
    me->test_circle = Circle_ctor(me->renderer, &me->circle_textures, radius, &item_pos);
    if(me->test_circle == NULL){
        GameBoard_dtor(me);
        return NULL;
    }

    // fprintf(stdout, "Print Pixels result : %d\n",
    //         pixelsToFile(me->circle_textures.circle, Texture, "../test/hitcircle_pixels.txt"));

    me->nb_items = 1;

/* --------------------------------- Player --------------------------------- */

    me->score = 0;
    me->combo = 0;

/* ---------------------------------- Game ---------------------------------- */

    me->GameState = Starting;

    return me;
}

void GameBoard_dtor(GameBoard *me){
    if(me->test_circle != NULL){
        Circle_dtor(me->test_circle);
        me->test_circle = NULL;
    }
    //Circles / Sliders / Spiner
    // for(int i=0 ; i<me->nb_items ; i++){
    //     if(me->items[i].type == 1)
    //         Circle_dtor(&me->items[i].circle);
    // }
    if(me->circle_textures.approach != NULL){
        SDL_DestroyTexture(me->circle_textures.approach);
        me->circle_textures.approach = NULL;
    }
    if(me->circle_textures.overlay != NULL){
        SDL_DestroyTexture(me->circle_textures.overlay);
        me->circle_textures.overlay = NULL;
    }
    if(me->circle_textures.circle != NULL){
        SDL_DestroyTexture(me->circle_textures.circle);
        me->circle_textures.circle = NULL;
    }
    if(me->background != NULL){
        Background_dtor(me->background);
        me->background = NULL;
    }
    if(me->pixel_format != NULL){
        SDL_FreeFormat(me->pixel_format);
        me->pixel_format = NULL;
    }
    if(me != NULL)
        free(me);
}

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/

int GameBoard_processEvent(GameBoard *me, Input *user_input){
    //Hit processing
    // if(user_input->hit1 == 1 || user_input->hit2 == 1){
    //     switch(me->GameState){
    //         case Pause:
    //             me->GameState = Running;
    //             break;

    //         case Running:
    //             //Check for item
    //             if(me->nb_items > 0){
    //                 if(me->items[me->next_item_index].type == 1){
                        
    //                 }
    //             }
    //             //Check for menu
    //                 //No menu for now
    //             break;
    //         default:;
    //     }
    // }

    fprintf(stdout, "x= %4d, y= %4d, h1= %1d:%1d, h2= %1d:%1d\r",
                    user_input->mouse_x, user_input->mouse_y,
                    user_input->hit1, user_input->hit1_r,
                    user_input->hit2, user_input->hit2_r);
            user_input->hit1 = SDL_FALSE;
            user_input->hit1_r = SDL_FALSE;
            user_input->hit2 = SDL_FALSE;
            user_input->hit2_r = SDL_FALSE;

    return 0;
}

int GameBoard_update(GameBoard *me){

    return 0;
}

int GameBoard_render(GameBoard *me, Uint32 frame_fraction){
    if(Background_draw(me->background) != 0){
        return -1;
    }
    if(Circle_draw(me->test_circle) != 0){
        return -1;
    }
    SDL_RenderPresent(me->renderer);
    return 0;
}