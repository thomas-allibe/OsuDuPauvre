#include "my_events.h"

/*-------------------------------------------------------------------------*/
/**
  @brief    get event from user
  @param    usr_input     struct pointer containing the input to process
  @return   the next state for the state machine

 */
/*--------------------------------------------------------------------------*/
State getUserEvent(Input *usr_input){
    SDL_Event event;
    State next_state = GetUserEvent;

    while(SDL_PollEvent(&event)){
    //###  Mouse Motion
        if(event.type == SDL_MOUSEMOTION){
            usr_input->mouse_x = event.motion.x;
            usr_input->mouse_y = event.motion.y;
            next_state = DoSomething;
        }//End Mouse Motion

    //###  Key Down
        if(event.type == SDL_KEYDOWN){
            //hit1
            if(event.key.keysym.sym == GAME_SETTINGS.controls_hit1){
                usr_input->hit1 = SDL_TRUE;
                
                if(event.key.repeat == 0){
                    usr_input->hit1_r = SDL_FALSE;
                }
                else{
                    usr_input->hit1_r = SDL_TRUE;
                }
                next_state = DoSomething;
            }
            //hit2
            if(event.key.keysym.sym == GAME_SETTINGS.controls_hit2){
                usr_input->hit2 = SDL_TRUE;
                
                if(event.key.repeat == 0){
                    usr_input->hit2_r = SDL_FALSE;
                }
                else{
                    usr_input->hit2_r = SDL_TRUE;
                }
                next_state = DoSomething;
            }
            //escape
            if(event.key.keysym.sym == SDLK_ESCAPE){
                // Temporary, waiting for pause menu
                usr_input->quit = SDL_TRUE;
                next_state = Quit;
                break; //Doesn't need to get remaining events
            }
        }//End Key Down

    //###  Mouse Button Down
        if(event.type == SDL_MOUSEBUTTONDOWN){
            usr_input->hit1 = SDL_TRUE;
            next_state = DoSomething;
        }//End Mouse Button Down
        
    //###  Quit
        if(event.type == SDL_QUIT){
            usr_input->quit = SDL_TRUE;
            next_state = Quit;
            break; //Doesn't need to get remaining events
        }
    }//End While

    return next_state;
}