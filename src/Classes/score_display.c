#include "Classes/score_display.h"

/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/

ScoreDisplay* ScoreDisplay_ctor(SDL_Renderer *r, const char *folder){

/* --------------------------------- Locals --------------------------------- */

    char digit_names[10][25] = {
        SD_BASE_NAME "0" SD_EXT,
        SD_BASE_NAME "1" SD_EXT,
        SD_BASE_NAME "2" SD_EXT,
        SD_BASE_NAME "3" SD_EXT,
        SD_BASE_NAME "4" SD_EXT,
        SD_BASE_NAME "5" SD_EXT,
        SD_BASE_NAME "6" SD_EXT,
        SD_BASE_NAME "7" SD_EXT,
        SD_BASE_NAME "8" SD_EXT,
        SD_BASE_NAME "9" SD_EXT
    };
    char special_names[SD_NB_SPE_CHAR][30] = {
        SD_BASE_NAME "dot" SD_EXT,
        SD_BASE_NAME "comma" SD_EXT,
        SD_BASE_NAME "percent" SD_EXT
    };
    char full_path[75];
    int folder_length = strlen(folder);
    strcpy(full_path, folder);

/* --------------------------------- Assert --------------------------------- */

    SDL_assert(r);
    SDL_assert(folder);

/* ----------------------------- Instance Malloc ---------------------------- */
    
    ScoreDisplay *me = (ScoreDisplay*) malloc(sizeof(ScoreDisplay));
    if(me == NULL){
        SDL_SetError("In ScoreDisplay_ctor(): 'me' instance malloc failed\n");
        return NULL;
    }

/* ----------------------------- Init Attributes ---------------------------- */
    
    me->renderer = r;
    me->digit_to_draw = NULL;
    me->pos_to_draw = NULL;
    me->justify = SD_Justify_Right;
    me->update_mode = SD_UpdateMode_Instant;
    me->thousand_sep = SDL_FALSE;
    me->score = NULL;
    me->prev_score = NULL;
    me->min_digit = 0;
    me->max_digit = 0;
    me->actual_digit = 0;
    me->pixel_w = 0;
    me->digit_w = 0;
    me->digit_h = 0;
    me->spe_w = 0;
    me->spe_h = 0;
    me->origin_x = 0;
    me->origin_y = 0;

/* ---------------------------- Attributes Malloc --------------------------- */

// Nothing

/* ------------------------------ Set min / max ----------------------------- */

    if(ScoreDisplay_set_min_max(me, 6, 10)){
        ScoreDisplay_dtor(me);
        return NULL;
    }

/* ----------------------- Textures Loading + Set NULL ---------------------- */
   
    for(int i=0 ; i<10 ; i++){
        strcpy(full_path+folder_length, digit_names[i]);
        me->digit_textures[i] = NULL;
        me->digit_textures[i] = loadImageStatic(full_path, PNG, r);
        if(me->digit_textures[i] == NULL){
            ScoreDisplay_dtor(me);
            return NULL;
        }
    }

    for(int i=0 ; i<SD_NB_SPE_CHAR ; i++){
        strcpy(full_path+folder_length, special_names[i]);
        me->spe_textures[i] = NULL;
        me->spe_textures[i] = loadImageStatic(full_path, PNG, r);
        if(me->spe_textures[i] == NULL){
            ScoreDisplay_dtor(me);
            return NULL;
        }
    }

    if(SDL_QueryTexture(me->digit_textures[0], NULL, NULL, &me->digit_w, &me->digit_h)){
        ScoreDisplay_dtor(me);
        return NULL;
    }
    if(SDL_QueryTexture(me->spe_textures[0], NULL, NULL, &me->spe_w, &me->spe_h)){
        ScoreDisplay_dtor(me);
        return NULL;
    }

    return me;
}

void ScoreDisplay_dtor(ScoreDisplay *me){
    SDL_assert(me);
    
    me->renderer = NULL;
    for(int i=0 ; i<10 ; i++){
        if(me->digit_textures[i] != NULL){
            SDL_DestroyTexture(me->digit_textures[i]);
            me->digit_textures[i] = NULL;
        }
    }
    for(int i=0 ; i<SD_NB_SPE_CHAR ; i++){
        if(me->spe_textures[i] != NULL){
            SDL_DestroyTexture(me->spe_textures[i]);
            me->spe_textures[i] = NULL;
        }
    }
    if(me->pos_to_draw != NULL){
        free(me->pos_to_draw);
    }
    if(me->digit_to_draw != NULL){
        free(me->digit_to_draw);
    }
    if(me->prev_score != NULL){
        free(me->prev_score);
    }
    if(me->score != NULL){
        free(me->score);
    }
    free(me);
}

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/

int ScoreDisplay_draw(const ScoreDisplay *me){
	SDL_assert(me);

    switch(me->update_mode){
        case SD_UpdateMode_Instant:
            for(int i=0 ; i<me->actual_digit ; i++){
                if(SDL_RenderCopy(me->renderer, me->digit_to_draw[i], NULL, &me->pos_to_draw[i])){
                    return -1;
                }
            }
            break;
        case SD_UpdateMode_Increment:
            for(int i=0 ; i<me->actual_digit ; i++){
                if(SDL_RenderCopy(me->renderer, me->digit_to_draw[i], NULL, &me->pos_to_draw[i])){
                    return -1;
                }
            }
            break;
        case SD_UpdateMode_Rotate:
            break;

        default:
            return -1;
    }
    
    return 0;
}

int ScoreDisplay_set_score(ScoreDisplay *me, const char *score){
    SDL_assert(me);
    SDL_assert(score);

    int prev_x = me->origin_x;
    int offset = 0;
    int len = strlen(score);
    char tmp_score[SD_REAL_MAX_NB_DIGIT+1];

    // Upper limit
    if(len > me->max_digit){
        score = score + len - me->max_digit; // Keep max_digit number of char from end of score, ignore left chars
        len = me->max_digit;
    }

    // Lower limit, pad with '0'
    if(len < me->min_digit){
        offset = me->min_digit - len;
        memset(tmp_score, '0', offset);
        len = me->min_digit;
    }

    // Cpy score in tmp_score after padded zeros
    strncpy(tmp_score + offset, score, len);
    
    me->actual_digit = len;

    //Browse tmp_score
    for(int i=0 ; i<len ; i++){
        switch(tmp_score[i]){
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '.':
            case ',':
            case '%':
                me->score[i] = score[i];
                me->pos_to_draw[i].x = prev_x;
                me->pos_to_draw[i].y = me->origin_y;
                switch(tmp_score[i]){
                    case '.':
                    case ',':
                    case '%':
                        switch(tmp_score[i]){
                            case '.':
                                me->digit_to_draw[i] = me->spe_textures[0];
                                break;
                            case ',':
                                me->digit_to_draw[i] = me->spe_textures[1];
                                break;
                            case '%':
                                me->digit_to_draw[i] = me->spe_textures[2];
                                break;
                            default:;
                        }
                        me->pos_to_draw[i].w = me->spe_w;
                        me->pos_to_draw[i].h = me->spe_h;
                        prev_x += me->spe_w;
                        break;
                    default:
                    // 0-9
                        me->digit_to_draw[i] = me->digit_textures[tmp_score[i]-'0'];
                        me->pos_to_draw[i].w = me->digit_w;
                        me->pos_to_draw[i].h = me->digit_h;
                        prev_x += me->digit_w;
                }
                break;
            default:;
                // Ignore non supported char
        }
    }
    me->score[me->max_digit] = '\0';

    offset =  prev_x - me->origin_x; // ScoreDisplay width in pixels
    me->pixel_w = offset;
    switch(me->justify){
        case SD_Justify_Left:
            for(int i=0 ; i<me->actual_digit ; i++){
                me->pos_to_draw[i].x -= offset;
            }
            break;
        case SD_Justify_Center:
            offset /= 2;
            for(int i=0 ; i<me->actual_digit ; i++){
                me->pos_to_draw[i].x -= offset;
            }
            break;
        default:;
            // Do Nothing, already Right
    }

    return 0;
}

void ScoreDisplay_set_justify(ScoreDisplay *me, SD_Justify justify){
    SDL_assert(me);
    SDL_assert(0 <= justify && justify <= 2);
    
    me->justify = justify;
}

void ScoreDisplay_set_update_mode(ScoreDisplay *me, SD_UpdateMode mode){
	SDL_assert(me);
    SDL_assert(0 <= mode && mode <= 2);

    me->update_mode = mode;
}

void ScoreDisplay_set_origin(ScoreDisplay *me, int x, int y){
	SDL_assert(me);
    
    me->origin_x = x;
    me->origin_y = y;
}

int ScoreDisplay_set_min_max(ScoreDisplay *me, int min, int max){
    SDL_assert(me);

    if(min >= 0)
        me->min_digit = min;
    
    if(max >= 0){
        // Capped
        if(max > SD_REAL_MAX_NB_DIGIT)
            max = SD_REAL_MAX_NB_DIGIT;

        if(max > me->max_digit){
            //Realloc
                // me->score
                // me->digit_to_draw
                // me->pos_to_draw
            me->score = realloc(me->score, sizeof(char)*(max+1));
            if(me->score == NULL){
                SDL_SetError("In ScoreDisplay_set_min_max(): 'score' realloc failed\n");
                return -1;
            }
            me->digit_to_draw = realloc(me->digit_to_draw, sizeof(SDL_Texture*)*max);
            if(me->digit_to_draw == NULL){
                SDL_SetError("In ScoreDisplay_set_min_max(): 'digit_to_draw' realloc failed\n");
                return -1;
            }
            me->pos_to_draw = realloc(me->pos_to_draw, sizeof(SDL_Rect)*max);
            if(me->pos_to_draw == NULL){
                SDL_SetError("In ScoreDisplay_set_min_max(): 'pos_to_draw' realloc failed\n");
                return -1;
            }
        }
        me->max_digit = max;
    }
    return 0;
}