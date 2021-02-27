#include "Classes/gameboard.h"

/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/
GameBoard* GameBoard_ctor(SDL_Renderer *renderer){
    
/* -------------------------------- Variables ------------------------------- */

    Uint32 *pixels = NULL;
    int pitch = 0;
    Uint8 r = 0, g = 0, b = 0, a = 0;
    Circle *tmp_circle = NULL;

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
    me->pause_bg = NULL;
    me->pause_bg = NULL;
    me->circle_textures = (Circle_Textures){NULL, NULL, NULL};
    me->item_list = NULL;
    me->item_list_end = NULL;

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

// ### Pause Background
    me->pause_bg = Background_ctor_file(me->renderer, "../images/pause-overlay.png");
    if(me->pause_bg == NULL){
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
    me->circle_pos = (SDL_Rect){0, 0, 0, 0};
    if(SDL_QueryTexture(me->circle_textures.circle, NULL, NULL, &me->circle_pos.w, &me->circle_pos.h)){
        GameBoard_dtor(me);
        return NULL;
    }
    if(SDL_LockTexture(me->circle_textures.circle, NULL, (void**)&pixels, &pitch)){
        GameBoard_dtor(me);
        return NULL;
    }
    for(int i=0 ; i<(me->circle_pos.w*me->circle_pos.h) ; i++){
        SDL_GetRGBA(pixels[i], me->pixel_format, &r, &g, &b, &a);
        if(a != 0){
            pixels[i] = SDL_MapRGBA(me->pixel_format, 164, 100, 240, a);
        }//164, 100, 240
    }
    
    //Get radius by measuring opaque texture from center to center-right
    me->circle_radius = 0;
    a = 255;
    for(int i=me->circle_pos.w/2 ; i<me->circle_pos.w && a>20 ; i++){
        SDL_GetRGBA(pixels[i + me->circle_pos.w*me->circle_pos.h/2], me->pixel_format, &r, &g, &b, &a);
        me->circle_radius++;
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

// ### Compute Spawn Region
    // Get a 4:3 window region
    if(GAME_SETTINGS.window_w > GAME_SETTINGS.window_h){
        me->spawn_region.h = GAME_SETTINGS.window_h - me->circle_pos.h; // Do not spawn out of bounds
        me->spawn_region.w = (GAME_SETTINGS.window_h * 4)/3 - me->circle_pos.w; // Do not spawn out of bounds
        me->spawn_region.y = 0;
        me->spawn_region.x = (GAME_SETTINGS.window_w - me->spawn_region.w)/2;
    }
    else if(GAME_SETTINGS.window_w < GAME_SETTINGS.window_h){
        me->spawn_region.w = GAME_SETTINGS.window_w - me->circle_pos.w; // Do not spawn out of bounds
        me->spawn_region.h = (GAME_SETTINGS.window_w * 4)/3 - me->circle_pos.h; // Do not spawn out of bounds
        me->spawn_region.x = 0;
        me->spawn_region.y = (GAME_SETTINGS.window_h - me->spawn_region.h)/2;
    }
    else{// ==
        me->spawn_region.w = GAME_SETTINGS.window_w - me->circle_pos.w; // Do not spawn out of bounds
        me->spawn_region.h = GAME_SETTINGS.window_h - me->circle_pos.h; // Do not spawn out of bounds
        me->spawn_region.x = 0;
        me->spawn_region.y = 0;
    }

    me->nb_items = 1;
    me->spawn_time = SPAWN_PERIOD;
    
    srand(SDL_GetTicks());
    
    me->circle_pos.x = Random(me->spawn_region.x, me->spawn_region.x + me->spawn_region.w);
    me->circle_pos.y = Random(me->spawn_region.y, me->spawn_region.y + me->spawn_region.h);
    tmp_circle = Circle_ctor(me->renderer, &me->circle_textures, me->circle_radius, &me->circle_pos);
    if(tmp_circle == NULL){
        GameBoard_dtor(me);
        return NULL;
    }
    me->item_list = ItemList_init(tmp_circle, 1);

/* --------------------------------- Player --------------------------------- */

    me->score = 0;
    me->combo = 0;
    me->health = 20;

/* ---------------------------------- Game ---------------------------------- */

    me->GameState = Starting;
    me->start_time = START_TIME;

    return me;
}

void GameBoard_dtor(GameBoard *me){
    // No need to set pointers to NULL since GameBoard structure will be free
    if(me->item_list != NULL){
        ItemList_free_list_and_items(me->item_list);
    }
    if(me->circle_textures.approach != NULL){
        SDL_DestroyTexture(me->circle_textures.approach);
    }
    if(me->circle_textures.overlay != NULL){
        SDL_DestroyTexture(me->circle_textures.overlay);
    }
    if(me->circle_textures.circle != NULL){
        SDL_DestroyTexture(me->circle_textures.circle);
    }
    if(me->pause_bg != NULL){
        Background_dtor(me->pause_bg);
    }
    if(me->background != NULL){
        Background_dtor(me->background);
    }
    if(me->pixel_format != NULL){
        SDL_FreeFormat(me->pixel_format);
    }
    if(me != NULL)
        free(me);
}

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/

int GameBoard_processEvent(GameBoard *me, Input *user_input){
    switch(me->GameState){
        case Starting:
            break;
        case Pause:
            if(user_input->pause){
                me->GameState = Running;
            }

            break;
        case Running:
            if(user_input->hit1 || user_input->hit2){
                
            }
            else if(user_input->pause){
                me->GameState = Pause;
            }

            break;
        default:
            ;
    }
    ResetUserInput(user_input);
    
    // fprintf(stdout, "x= %4d, y= %4d, h1= %1d:%1d, h2= %1d:%1d\r",
    //                 user_input->mouse_x, user_input->mouse_y,
    //                 user_input->hit1, user_input->hit1_r,
    //                 user_input->hit2, user_input->hit2_r);
    //         user_input->hit1 = SDL_FALSE;
    //         user_input->hit1_r = SDL_FALSE;
    //         user_input->hit2 = SDL_FALSE;
    //         user_input->hit2_r = SDL_FALSE;

    return 0;
}

int GameBoard_update(GameBoard *me, Uint32 dt){
    ItemList *tmp_list = me->item_list;
    Circle *tmp_circle = NULL;
    SDL_bool first_item = SDL_TRUE;
    switch(me->GameState){
        case Starting:
            me->start_time -= dt;
            if(me->start_time <= 0){
                me->GameState = Running;
                me->start_time = START_TIME;
            }
            break;
        case Pause:
        
            break;
        case Running:
            while(tmp_list != NULL){
                switch(tmp_list->type){
                case 1:
                    Circle_update(tmp_list->circle, dt);
                    // if(Circle_no_time_left(tmp_list->circle)){
                    //     tmp_list->circle->time_left = tmp_list->circle->duration;
                    // }
                    if(first_item){
                        if(Circle_no_time_left(tmp_list->circle)){
                            tmp_list = ItemList_free_first(tmp_list);
                            me->item_list = tmp_list; //Update the new list pointer
                            me->health--;
                            me->combo = 0;
                            first_item = SDL_FALSE;
                        }
                    }
                    break;
                case 2:

                    break;
                case 3:
                    
                    break;
                default:
                    ;
                }
                tmp_list = tmp_list->next;
            }
            me->spawn_time -= dt;
            if(me->spawn_time <= 0){
                me->circle_pos.x = Random(me->spawn_region.x, me->spawn_region.x + me->spawn_region.w);
                me->circle_pos.y = Random(me->spawn_region.y, me->spawn_region.y + me->spawn_region.h);
                tmp_circle = Circle_ctor(me->renderer, &me->circle_textures, me->circle_radius, &me->circle_pos);
                if(tmp_circle == NULL){
                    return -1;
                }
                me->item_list_end = ItemList_add_item(me->item_list, tmp_circle, 1);
                me->spawn_time += SPAWN_PERIOD;
            }
            break;
        default:
            ;
    }
    return 0;
}

int GameBoard_render(GameBoard *me, double prediction){
    // SDL_RenderClear(me->renderer);
    switch(me->GameState){
        case Starting:
            break;
        case Pause:
            Background_draw(me->pause_bg);
            break;
        case Running:
            if(Background_draw(me->background) != 0){
                return -1;
            }
            // Send the end of list to browse backwards
            if(ItemList_render_items(me->item_list_end, prediction) != 0){
                return -1;
            }
            break;
        default:
        ;
    }
    SDL_RenderPresent(me->renderer);
    return 0;
}

/* -------------------------------------------------------------------------- */
/*                                ItemsList API                               */
/* -------------------------------------------------------------------------- */

ItemList *ItemList_init(void *item, int type){
    ItemList *list = NULL;
    list = malloc(sizeof(ItemList));
    if(list == NULL){
        SDL_SetError("Erreur malloc ItemList_init()\n");
        return NULL;
    }

    list->next = NULL;
    list->prev = NULL;
    list->type = type;
    switch(type){
        case 1:
            list->circle = (Circle*)item;
            break;
        case 2:
            list->SliderFoo = (int*)item;
            break;
        case 3:
            list->SpinerFoo = (int*)item;
            break;
        default:
            ;
    }
    return list;
}

ItemList *ItemList_add_item(ItemList *list, void *item, int type){
    ItemList *prev = NULL;
    while(list->next != NULL){
        list = list->next;
    }
    prev = list;
    list->next = malloc(sizeof(ItemList));
    if(list->next == NULL){
        SDL_SetError("Erreur malloc ItemList_init()\n");
        return NULL;
    }
    list = list->next;
    list->next = NULL;
    list->prev = prev;
    list->type = type;
    switch(type){
        case 1:
            list->circle = (Circle*)item;
            break;
        case 2:
            list->SliderFoo = (int*)item;
            break;
        case 3:
            list->SpinerFoo = (int*)item;
            break;
        default:
            ;
    }
    return list;
}

ItemList *ItemList_remove_first(ItemList *list_in, void **item_out, int *type_out){
    ItemList *to_free = list_in;
    *type_out = list_in->type;
    switch(list_in->type){
        case 1:
            *item_out = (Circle*)list_in->circle;
            break;
        case 2:
            *item_out = (int*)list_in->SliderFoo;
            break;
        case 3:
            *item_out = (int*)list_in->SpinerFoo;
            break;
        default:
            ;
    }
    if(list_in->next == NULL){
        list_in = NULL;
    }
    else{
        list_in = list_in->next;
        list_in->prev = NULL;
    }
    free(to_free);
    return list_in;
}

ItemList *ItemList_free_first(ItemList *list_in){
    ItemList *to_free = list_in;
    switch(list_in->type){
        case 1:
            Circle_dtor(list_in->circle);
            break;
        case 2:
            
            break;
        case 3:
            
            break;
        default:
            ;
    }
    if(list_in->next == NULL){
        list_in = NULL;
    }
    else{
        list_in = list_in->next;
        list_in->prev = NULL;
    }
    free(to_free);
    return list_in;
}

void ItemList_free_list_and_items(ItemList *list){
    ItemList *tmp = NULL;
    while(list != NULL){
        tmp = list;
        list = list->next;
        switch(tmp->type){
        case 1:
            if(tmp->circle != NULL){
                Circle_dtor(tmp->circle);
            }
            break;
        case 2:

            break;
        case 3:
            
            break;
        default:
            ;
        }
        free(tmp);
    }
}

int ItemList_render_items(ItemList *list, double prediction){
    while(list != NULL){
        switch(list->type){
        case 1:
            if(Circle_draw(list->circle, prediction) != 0){
                return -1;
            }
            break;
        case 2:

            break;
        case 3:
            
            break;
        default:
            ;
        }
        list = list->prev;
    }
    return 0;
}

// int ItemList_update_items(ItemList *list, Uint32 dt){

// }

/* -------------------------------------------------------------------------- */
/*                                    OTHER                                   */
/* -------------------------------------------------------------------------- */

int Random(int min, int max){
   return (rand() % (min - max + 1)) + min; 
}