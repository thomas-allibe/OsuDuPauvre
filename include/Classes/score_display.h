#ifndef SCORE_DISPLAY_H
#define SCORE_DISPLAY_H

#include <stdlib.h>
#include <string.h>
#include "SDL2/SDL.h"
#include "mySDL2_tools.h"

#define SD_BASE_NAME "ScoreDisplay_"
#define SD_EXT ".png"
#define SD_NB_SPE_CHAR 3
#define SD_REAL_MAX_NB_DIGIT 32

/****************************************************************************
 *    PUBLIC TYPES
 ***************************************************************************/
// Not used
typedef struct{
    char *Score_0;
    char *Score_1;
    char *Score_2;
    char *Score_3;
    char *Score_4;
    char *Score_6;
    char *Score_7;
    char *Score_8;
    char *Score_9;
    char *Score_dot;
    char *Score_comma;
}SD_Texture_Paths;

typedef enum{
    SD_Justify_Left,
    SD_Justify_Center,
    SD_Justify_Right
}SD_Justify;

typedef enum{
    SD_UpdateMode_Instant,
    SD_UpdateMode_Increment,
    SD_UpdateMode_Rotate
}SD_UpdateMode;

//ScoreDisplay Attributes
typedef struct{
    SDL_Renderer *renderer;
    SDL_Texture *digit_textures[10]; // 0-9
    SDL_Texture *spe_textures[SD_NB_SPE_CHAR]; // . , %
    SDL_Texture **digit_to_draw; //Always length = max_digit
    SDL_Rect *pos_to_draw; //Always length = max_digit
    SD_Justify justify;
    SD_UpdateMode update_mode;
    SDL_bool thousand_sep;
    char *score; // Buffer for score to display length = max_digit+1
    char *prev_score;
    int min_digit;
    int max_digit;
    int actual_digit;
    int pixel_w;
    int digit_w;
    int digit_h;
    int spe_w;
    int spe_h;
    int origin_x;
    int origin_y;
}ScoreDisplay;

/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/

/**
  @brief    Create an instance of ScoreDisplay class. (Max 32 digits)
  @param    r      pointer to the renderer
  @param    folder   folder where to find score digit textures.
            IMG name format must be SD_BASE_NAME + "0" for 0 texture.
            Same with 1-9 and "dot" and "comma"
  @return   Pointer to the ScoreDisplay instance. NULL if failed.
			Use ScoreDisplay_dtor to destroy it.
 */
ScoreDisplay* ScoreDisplay_ctor(SDL_Renderer *r, const char *folder);

/**
  @brief    Destroy a ScoreDisplay instance
  @param    me     pointer to the ScoreDisplay to free
  @return   void
 */
void ScoreDisplay_dtor(ScoreDisplay *me);

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/

/**
  @brief    Draw the ScoreDisplay on the associated renderer
  @param	me     pointer to the ScoreDisplay to free
  @return	void
 */
int ScoreDisplay_draw(const ScoreDisplay *me);

/**
  @brief    Update the ScoreDisplay timings
  @param	me     pointer to the ScoreDisplay to free
  @return	void
 */
int ScoreDisplay_update(ScoreDisplay *me);

/**
  @brief    Set the new score to display
  @param	me      pointer to the ScoreDisplay instance
  @param    score   string (numbers, comma & dot) to show
  @return	0 on success, -1 on error
 */
int ScoreDisplay_set_score(ScoreDisplay *me, const char *score);

/**
  @brief    Set the justify mode for ScoreDisplay
  @param	me      pointer to the ScoreDisplay instance
  @param    justify One between SD_Justify_Left, SD_Justify_Center & SD_Justify_Right
  @return	void
 */
void ScoreDisplay_set_justify(ScoreDisplay *me, SD_Justify justify);

/**
  @brief    Set the justify mode for ScoreDisplay
  @param	me      pointer to the ScoreDisplay instance
  @param    mode    One between SD_UpdateMode_Instant, SD_UpdateMode_Rotate,
                    SD_UpdateMode_Increment & SD_UpdateMode_Rot_Inc
  @return	void
 */
void ScoreDisplay_set_update_mode(ScoreDisplay *me, SD_UpdateMode mode);

/**
  @brief    Set the justify mode for ScoreDisplay
  @param	me      pointer to the ScoreDisplay instance
  @param    mode    One between SD_UpdateMode_Instant, SD_UpdateMode_Rotate,
                    SD_UpdateMode_Increment & SD_UpdateMode_Rot_Inc
  @return	void
 */
void ScoreDisplay_set_origin(ScoreDisplay *me, int x, int y);

/**
  @brief    Set the min & max sizes for ScoreDisplay
  @param	me  pointer to the ScoreDisplay instance
  @param    min min size, arg ignored if -1    
  @param    max max size, capped to SD_REAL_MAX_NB_DIGIT, arg ignored if -1    
  @return	0 on success, -1 on error
 */
int ScoreDisplay_set_min_max(ScoreDisplay *me, int min, int max);

#endif /* SCORE_DISPLAY_H */