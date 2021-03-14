#include "Classes/gameboard.h"

/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/
GameBoard* GameBoard_ctor(SDL_Renderer *renderer){
    
/* -------------------------------- Variables ------------------------------- */

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
    me->count_1 = NULL;
    me->count_2 = NULL;
    me->count_3 = NULL;
    me->circle_textures = (Circle_Textures){NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    me->circle_sounds = (Circle_Sounds){NULL, NULL};
    me->item_list = NULL;
    me->item_list_end = NULL;
    me->music = NULL;
    me->count_sound = NULL;

/* -------------------------------- SDL_video ------------------------------- */

    me->renderer = renderer;

/* --------------------------- Objects / Textures --------------------------- */

    if(GameBoard_load_assets(me)){
        GameBoard_dtor(me);
        return NULL;
    }

// ### Compute Spawn Region
    GameBoard_compute_spawn_region(me);

    me->spawn_time = SPAWN_PERIOD;
    srand(SDL_GetTicks());
    
    me->circle_pos.x = Random(me->spawn_region.x, me->spawn_region.x + me->spawn_region.w);
    me->circle_pos.y = Random(me->spawn_region.y, me->spawn_region.y + me->spawn_region.h);
    tmp_circle = Circle_ctor(me->renderer, &me->circle_textures, &me->circle_sounds, me->circle_radius, &me->circle_pos);
    if(tmp_circle == NULL){
        GameBoard_dtor(me);
        return NULL;
    }
    me->item_list = ItemList_init(tmp_circle, 1);
    me->item_list_end = me->item_list;

/* --------------------------------- Sounds --------------------------------- */

    Mix_AllocateChannels(NB_CHANNELS); //For hit and combo break sounds
    Mix_Volume(-1, MIX_MAX_VOLUME/10);
    me->count_flag = 0xFF;

/* --------------------------------- Player --------------------------------- */

    me->score = 0;
    me->score_update = SDL_TRUE;
    me->combo = 0;
    me->health = 20;

/* ---------------------------------- Game ---------------------------------- */

    me->GameState = Starting;
    me->start_time = START_TIME;

    return me;
}

void GameBoard_dtor(GameBoard *me){
    SDL_assert(me);
    // No need to set pointers to NULL since GameBoard structure will be free
    //Music
    if(me->count_sound != NULL){
        Mix_FreeChunk(me->count_sound);
    }
    if(me->circle_sounds.combo_break != NULL){
        Mix_FreeChunk(me->circle_sounds.combo_break);
    }
    if(me->circle_sounds.hit_sound != NULL){
        Mix_FreeChunk(me->circle_sounds.hit_sound);
    }
    if(me->music != NULL){
        if(Mix_PlayingMusic())
            Mix_HaltMusic();
        Mix_FreeMusic(me->music);
    }

    if(me->item_list != NULL){
        ItemList_free_list_and_items(me->item_list);
    }
    //Textures
    if(me->circle_textures.miss != NULL){
        SDL_DestroyTexture(me->circle_textures.miss);
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
    //Objects
    if(me->score_display != NULL){
        ScoreDisplay_dtor(me->score_display);
    }
    if(me->count_3 != NULL){
        Background_dtor(me->count_3);
    }
    if(me->count_2 != NULL){
        Background_dtor(me->count_2);
    }
    if(me->count_1 != NULL){
        Background_dtor(me->count_1);
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
    free(me);
}

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/

int GameBoard_processEvent(GameBoard *me, Input *user_input){
    switch(me->GameState){
/* ---------------------------- SWITCH: Starting ---------------------------- */
        case Starting:
            break;

/* ------------------------------ SWITCH: Pause ----------------------------- */
        case Pause:
            if(user_input->pause){
                me->GameState = Starting;
            }

            break;

/* ----------------------------- SWITCH: Running ---------------------------- */
        case Running:
            if(user_input->hit1 || user_input->hit2){
                int hit_points = 0;
                Uint8 check_next = 1;
                ItemList *tmp = me->item_list;
                while(check_next && tmp!=NULL){
                    switch(tmp->type){
                        case 1:
                            hit_points = Circle_hit(tmp->circle, user_input->mouse_x, user_input->mouse_y);
                            if(hit_points > 0){                               
                                me->combo++;
                                me->score += hit_points*me->combo;
                                me->score_update = SDL_TRUE;
                                check_next = 0; //Don't check next item
                            }
                            else if(hit_points == 0){ // Bad timing
                                check_next = 0; //Don't check next item
                            }
                            //  if(hit_points == -1) => is disapearing => check_next = 1
                            break;
                        case 2:

                            break;
                        case 3:
                            
                            break;
                        default:
                            ;
                    }
                    tmp = tmp->next;
                }
            }
            else if(user_input->pause){
                me->GameState = Pause;
            }

            break;
        default:
            ;
    }
    ResetUserInput(user_input);

    return 0;
}

int GameBoard_update(GameBoard *me, Uint32 dt){
    ItemList *tmp_list = me->item_list;
    Circle *tmp_circle = NULL;
    SDL_bool first_item = SDL_TRUE;
    char score[SCORE_NB_DIGIT+1];
    switch(me->GameState){
/* ---------------------------- SWITCH: Starting ---------------------------- */
        case Starting:
            me->start_time -= dt;
            
            if(me->count_flag&1 && 3000 >= me->start_time){
                me->count_flag &= ~1;
                Mix_PlayChannel(-1, me->count_sound, 0);
            }
            if(me->count_flag&(1<<1) && 2000 >= me->start_time){
                me->count_flag &= ~(1<<1);
                Mix_PlayChannel(-1, me->count_sound, 0);
            }
            if(me->count_flag&(1<<2) && 1000 >= me->start_time){
                me->count_flag &= ~(1<<2);
                Mix_PlayChannel(-1, me->count_sound, 0);
            }
            if(me->count_flag&(1<<3) && 0 >= me->start_time){
                me->count_flag &= ~(1<<3);
                Mix_PlayChannel(-1, me->count_sound, 0);
            }


            if(me->start_time <= 0){
                me->count_flag = 0xFF;
                me->GameState = Running;
                me->start_time = START_TIME;
                if(!Mix_PlayingMusic()){
                    if(Mix_PlayMusic(me->music, -1)){
                        return -1;
                    }
                    Mix_VolumeMusic(MIX_MAX_VOLUME/10);
                }
                if(Mix_PausedMusic()){
                    Mix_ResumeMusic();
                }
            }
            break;

/* ------------------------------ SWITCH: Pause ----------------------------- */
        case Pause:
            if(Mix_PlayingMusic() && !Mix_PausedMusic()){
                Mix_PauseMusic();
            }
            break;

/* ----------------------------- SWITCH: Running ---------------------------- */
        case Running:
        // ### Item Update
            while(tmp_list != NULL){
                switch(tmp_list->type){
                case 1:
                    Circle_update(tmp_list->circle, dt, me->combo);
                    if(first_item){
                        //Only check for the first item since next it's the oldest
                        if(Circle_to_destroy(tmp_list->circle)){
                            if(Circle_is_missed(tmp_list->circle)){
                                me->health--;
                                me->combo = 0;
                            }
                            tmp_list = ItemList_free_first(tmp_list);
                            me->item_list = tmp_list; //Update the new list pointer
                            if(tmp_list == NULL)// No item left, set list end pointer to NULL
                                me->item_list_end = NULL;
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
        // ### Item Spawn
            me->spawn_time -= dt;
            if(me->spawn_time <= 0){
                me->circle_pos.x = Random(me->spawn_region.x, me->spawn_region.x + me->spawn_region.w);
                me->circle_pos.y = Random(me->spawn_region.y, me->spawn_region.y + me->spawn_region.h);
                tmp_circle = Circle_ctor(me->renderer, &me->circle_textures, &me->circle_sounds, me->circle_radius, &me->circle_pos);
                if(tmp_circle == NULL){
                    return -1;
                }
                me->item_list_end = ItemList_add_item(me->item_list, tmp_circle, 1);
                me->spawn_time += SPAWN_PERIOD;
            }
        // ### Score Display
            if(me->score_update){
                sprintf(score, "%08d", me->score);
                ScoreDisplay_set_score(me->score_display, score);
                me->score_update = SDL_FALSE;
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
            if(Background_draw(me->background) != 0){
                return -1;
            }
            if(ScoreDisplay_draw(me->score_display) != 0){
                return -1;
            }
            // Use the end of list to browse backwards
            if(ItemList_render_items(me->item_list_end, prediction) != 0){
                return -1;
            }
            if(3000 >= me->start_time && me->start_time > 2000)
                Background_draw(me->count_3);
            if(2000 >= me->start_time && me->start_time > 1000)
                Background_draw(me->count_2);
            if(1000 >= me->start_time && me->start_time >= 0)
                Background_draw(me->count_1);
            break;
        case Pause:
            Background_draw(me->pause_bg);
            break;
        case Running:
            if(Background_draw(me->background) != 0){
                return -1;
            }
            if(ScoreDisplay_draw(me->score_display) != 0){
                return -1;
            }
            // Use the end of list to browse backwards
            if(ItemList_render_items(me->item_list_end, prediction) != 0){
                return -1;
            }
            break;
        default:
        ;
    }
    SDL_RenderPresent(me->renderer);
    // fprintf(stdout, "\rSCORE=%08d, COMBO=%04d, HEALTH=%02d", me->score, me->combo, me->health);

    return 0;
}

int GameBoard_load_assets(GameBoard *me){
    Uint32 *pixels = NULL;
    int pitch = 0;
    Uint8 r = 0, g = 0, b = 0, a = 0;
    float scale = 1;

/* ------------------------------- Background ------------------------------- */

    me->background = Background_ctor_color(me->renderer,
                                                        me->pixel_format,
                                                        (SDL_Color){10, 10, 10, 255},
                                                        GAME_SETTINGS.window_w,
                                                        GAME_SETTINGS.window_h);
    if(me->background == NULL){
        return -1;
    }

/* ---------------------------- Pause Background ---------------------------- */

    me->pause_bg = Background_ctor_file(me->renderer, ASSET_TEXTURE_PAUSE_BG);
    if(me->pause_bg == NULL){
        return -1;
    }

/* -------------------------------- Countdown ------------------------------- */

    me->count_1 = Background_ctor_file(me->renderer, ASSET_TEXTURE_COUNT_1);
    if(me->count_1 == NULL){
        return -1;
    }
    if(Background_set_draw_mode(me->count_1, BG_DrawMode_Scaled_Centered, &scale, NULL)){
        return -1;
    }

    me->count_2 = Background_ctor_file(me->renderer, ASSET_TEXTURE_COUNT_2);
    if(me->count_2 == NULL){
        return -1;
    }
    if(Background_set_draw_mode(me->count_2, BG_DrawMode_Scaled_Centered, &scale, NULL)){
        return -1;
    }

    me->count_3 = Background_ctor_file(me->renderer, ASSET_TEXTURE_COUNT_3);
    if(me->count_3 == NULL){
        return -1;
    }
    if(Background_set_draw_mode(me->count_3, BG_DrawMode_Scaled_Centered, &scale, NULL)){
        return -1;
    }

/* ---------------------------------- Score --------------------------------- */

    me->score_display = ScoreDisplay_ctor(me->renderer, ASSET_TEXTURE_FOLDER);
    if(me->score_display == NULL){
        return -1;
    }
    ScoreDisplay_set_justify(me->score_display, SD_Justify_Right);
    ScoreDisplay_set_update_mode(me->score_display, SD_UpdateMode_Instant);

/* ------------------------------------ - ----------------------------------- */
/* -------------------------------- Textures -------------------------------- */
/* ------------------------------------ - ----------------------------------- */

/* ----------------------------- Circle Texture ----------------------------- */

    me->circle_textures.circle = loadImageStream(ASSET_TEXTURE_CIRCLE, PNG, me->renderer);
    if(me->circle_textures.circle == NULL){
        return -1;
    }
    if(SDL_SetTextureBlendMode(me->circle_textures.circle, SDL_BLENDMODE_BLEND)){
        return -1;
    }
    me->circle_pos = (SDL_Rect){0, 0, 0, 0};
    if(SDL_QueryTexture(me->circle_textures.circle, NULL, NULL, &me->circle_pos.w, &me->circle_pos.h)){
        return -1;
    }
    if(SDL_LockTexture(me->circle_textures.circle, NULL, (void**)&pixels, &pitch)){
        return -1;
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

/* ------------------------- Circle Overlay Texture ------------------------- */

    me->circle_textures.overlay = loadImageStatic(ASSET_TEXTURE_CIRCLE_HOVERLAY, PNG, me->renderer);
    if(me->circle_textures.overlay == NULL){
        return -1;
    }
    if(SDL_SetTextureBlendMode(me->circle_textures.circle, SDL_BLENDMODE_BLEND)){
        return -1;
    }

/* ----------------------------- Approach circle ---------------------------- */

    me->circle_textures.approach = loadImageStatic(ASSET_TEXTURE_CIRCLE_APPROACH, PNG, me->renderer);
    if(me->circle_textures.approach == NULL){
        return -1;
    }
    if(SDL_SetTextureBlendMode(me->circle_textures.approach, SDL_BLENDMODE_BLEND)){
        return -1;
    }

/* ---------------------------------- Miss ---------------------------------- */

    me->circle_textures.miss = loadImageStatic(ASSET_TEXTURE_CIRCLE_MISS, PNG, me->renderer);
    if(me->circle_textures.miss == NULL){
        return -1;
    }
    if(SDL_SetTextureBlendMode(me->circle_textures.miss, SDL_BLENDMODE_BLEND)){
        return -1;
    }

/* --------------------------------- Hit 50 --------------------------------- */

    me->circle_textures._50 = loadImageStatic(ASSET_TEXTURE_CIRCLE_50, PNG, me->renderer);
    if(me->circle_textures._50 == NULL){
        return -1;
    }
    if(SDL_SetTextureBlendMode(me->circle_textures._50, SDL_BLENDMODE_BLEND)){
        return -1;
    }

/* --------------------------------- Hit 100 -------------------------------- */

    me->circle_textures._100 = loadImageStatic(ASSET_TEXTURE_CIRCLE_100, PNG, me->renderer);
    if(me->circle_textures._100 == NULL){
        return -1;
    }
    if(SDL_SetTextureBlendMode(me->circle_textures._100, SDL_BLENDMODE_BLEND)){
        return -1;
    }

/* --------------------------------- Hit 300 -------------------------------- */

    me->circle_textures._300 = loadImageStatic(ASSET_TEXTURE_CIRCLE_300, PNG, me->renderer);
    if(me->circle_textures._300 == NULL){
        return -1;
    }
    if(SDL_SetTextureBlendMode(me->circle_textures._300, SDL_BLENDMODE_BLEND)){
        return -1;
    }

/* ------------------------------------ - ----------------------------------- */
/* ---------------------------------- Music --------------------------------- */
/* ------------------------------------ - ----------------------------------- */

    me->music = Mix_LoadMUS(ASSET_MUSIC_DIR "Gintama_End_29_Full.mp3");
    if(me->music == NULL){
        return -1;
    }

    me->circle_sounds.hit_sound = Mix_LoadWAV(ASSET_SOUND_HIT);
    if(me->circle_sounds.hit_sound == NULL){
        return -1;
    }
    me->circle_sounds.combo_break = Mix_LoadWAV(ASSET_SOUND_BREAK);
    if(me->circle_sounds.combo_break == NULL){
        return -1;
    }

    me->count_sound = Mix_LoadWAV(ASSET_SOUND_COUNT);
    if(me->count_sound == NULL){
        return -1;
    }

    return 0;
}

void GameBoard_compute_spawn_region(GameBoard *me){
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
    if(list == NULL){
        list = ItemList_init(item, type);
    }
    else{
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
    }
    return list;
}

ItemList *ItemList_remove_first(ItemList *list_in, void **item_out, int *type_out){
    ItemList *to_free = list_in;
    if(list_in == NULL)
        return NULL;
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
    if(list_in == NULL)
        return NULL;
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