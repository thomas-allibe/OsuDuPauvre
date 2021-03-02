#include "Classes/circle.h"


/****************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ***************************************************************************/
Circle* Circle_ctor(SDL_Renderer *r, Circle_Textures *ct, int radius, SDL_Rect *pos){

/* ---------------------------- Memory Allocation --------------------------- */
    
    Circle *me = NULL;
    me = (Circle*) malloc(sizeof(Circle));
    if(me == NULL){
        SDL_SetError("Erreur malloc : Circle_ctor()");
        return NULL;
    }
    
    me->renderer = r;
    me->textures = ct;
    me->pos = *pos;
    me->radius = radius;
    me->x_center = pos->x + pos->w/2;
    me->y_center = pos->y + pos->h/2;

// ### Approach circle pos & size
    if(GAME_SETTINGS.game_AR > 5){
        me->ac_hit = 1200 + (600*(5 - GAME_SETTINGS.game_AR))/5;
        me->fade_in = 800 - (500*(GAME_SETTINGS.game_AR - 5))/5;
    }
    else if(GAME_SETTINGS.game_AR < 5){
        me->ac_hit = 1200 - (750*(GAME_SETTINGS.game_AR - 5))/5;
        me->fade_in = 800 + (400*(5 - GAME_SETTINGS.game_AR))/5;
    }
    else{
        me->ac_hit = 1200;
        me->fade_in = 800;
    }
    me->life_time = 0;
    SDL_QueryTexture(me->textures->approach, NULL, NULL, &me->ac_w_end, &me->ac_h_end);
    me->ac_w_end *= 0.5;
    me->ac_h_end *= 0.5;
	me->ac_w_start = me->ac_w_end * 4;
    me->ac_h_start = me->ac_h_end * 4;

    me->ac_pos.w = me->ac_w_start;
    me->ac_pos.h = me->ac_h_start;
    me->ac_pos.x = me->x_center - me->ac_pos.w/2;
    me->ac_pos.y = me->y_center - me->ac_pos.h/2;

    me->hit_flag = 0;
    
    return me;
}

void Circle_dtor(Circle *me){
    if(me != NULL)
        free(me);
}

/****************************************************************************
 *    PUBLIC METHODS
 ***************************************************************************/

void Circle_update(Circle *me, int dt){
    me->life_time += dt;
    if(!me->hit_flag && me->life_time >= me->ac_hit+EXTRA_LIFE_SPAN){
        me->hit_flag = HIT_FLAG_MISS;
        me->life_time = 0;
    }
}

int Circle_draw(Circle *me, double prediction){
    if(!me->hit_flag){
    // Draw circle & stuff
    // if(me->life_time < me->ac_hit + EXTRA_LIFE_SPAN){
        //Fading in
	    Uint8 alpha = 0;
        if(me->life_time <= me->fade_in){
			alpha = (me->life_time*255)/me->fade_in;
        }
		else{
			alpha = 255;
		}
		if(SDL_SetTextureAlphaMod(me->textures->circle, alpha)){
			return -1;
		}
		if(SDL_SetTextureAlphaMod(me->textures->overlay, alpha)){
			return -1;
		}
		if(SDL_SetTextureAlphaMod(me->textures->approach, alpha)){
			return -1;
		}
        if(SDL_RenderCopy(me->renderer, me->textures->circle, NULL, &(me->pos))){
            return -1;
        }
        if(SDL_RenderCopy(me->renderer, me->textures->overlay, NULL, &(me->pos))){
            return -1;
        }
        if(me->life_time+prediction*MS_PER_UPDATE <= me->ac_hit){
            //Compute new approach circle dimensions w, h then x, y position
            me->ac_pos.w = ((me->ac_w_end-me->ac_w_start)*(me->life_time+prediction*MS_PER_UPDATE))
                            /me->ac_hit + me->ac_w_start;
            me->ac_pos.h = ((me->ac_h_end-me->ac_h_start)*(me->life_time+prediction*MS_PER_UPDATE))
                            /me->ac_hit + me->ac_h_start;
            me->ac_pos.x = me->x_center - me->ac_pos.w/2;
            me->ac_pos.y = me->y_center - me->ac_pos.h/2;
            if(SDL_RenderCopy(me->renderer, me->textures->approach, NULL, &(me->ac_pos))){
                return -1;
            }
        }
    }
    else{
        // Draw points or miss
        SDL_Texture *tmp_texture = NULL;
        if(me->hit_flag == -1)
            tmp_texture = me->textures->miss;
        if(me->hit_flag == 1)
            tmp_texture = me->textures->_50;
        if(me->hit_flag == 2)
            tmp_texture = me->textures->_100;
        if(me->hit_flag == 3)
            tmp_texture = me->textures->_300;
        SDL_Rect tmp = {me->pos.x, me->pos.y, 0, 0};
        SDL_QueryTexture(tmp_texture, NULL, NULL, &tmp.w, &tmp.h);
        if(SDL_RenderCopy(me->renderer, tmp_texture, NULL, &tmp)){
            return -1;
        }
    }
    return 0;
}

int Circle_is_pos_on_circle(Circle *me, int x, int y){
    if(me->hit_flag)
        return 0;
    float distance = sqrt((x-me->x_center)*(x-me->x_center)
                        +(y-me->y_center)*(y-me->y_center));
    return distance <= me->radius;
}

int Circle_set_hit(Circle *me){
	if(me->hit_flag)
        return 0; // Circle not hitable
    if(me->life_time <= me->ac_hit+EXTRA_LIFE_SPAN/2 && 
		me->life_time >= me->ac_hit-EXTRA_LIFE_SPAN/2){
		// 300 pts
		me->hit_flag = HIT_FLAG_300;
        me->life_time = 0;
		return 300;
	}
	else if(me->life_time <= me->ac_hit+EXTRA_LIFE_SPAN &&
			me->life_time >= me->ac_hit-EXTRA_LIFE_SPAN){
		// 100 pts
		me->hit_flag = HIT_FLAG_100;
        me->life_time = 0;
		return 100;
	}
	else if(me->life_time <= me->ac_hit+EXTRA_LIFE_SPAN &&
			me->life_time >= me->ac_hit-1.5*EXTRA_LIFE_SPAN){
		// 50 pts
		me->hit_flag = HIT_FLAG_50;
        me->life_time = 0;
		return 50;
	}
	return 0;
}

int Circle_has_time_left(Circle *me){
    return !me->hit_flag && me->life_time < me->ac_hit+EXTRA_LIFE_SPAN;
}

int Circle_is_missed(Circle *me){
    return me->hit_flag;
}

int Circle_to_destroy(Circle *me){
    return me->hit_flag && me->life_time>=SHOW_PTS_DURATION;
}