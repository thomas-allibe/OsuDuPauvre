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
        if(event.type == SDL_MOUSEMOTION){
            fprintf(stderr, "x=%4d / y=%4d\r", event.motion.x, event.motion.y);
            next_state = GetUserEvent;
        }
        else if(event.type == SDL_KEYDOWN){
            fprintf(stderr, "Keydown\n");
            next_state = GetUserEvent;
        }
        else if(event.type == SDL_QUIT){
            next_state = Quit;
            //Doesn't need to get remaining events
            break;
        }
    }

    return next_state;
}