#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

//Global struct
typedef struct{
    //If you add parameters,
    //add them in initialize_functions.h #define section
    int window_w;
    int window_h;
}Settings;

typedef struct{
    int foo;
}Input;

//Global enum
typedef enum {
/* 0 */ Initialize,
/* 1 */ GetUserEvent,
/* 2 */ Wait,
/* 3 */ Error,
/* 4 */ Quit,
/* 5 */ Stop
}State;


//Global variables (GV)
extern const char *GAME_TITLE;
extern Settings GAME_SETTINGS;

#endif /* GLOBAL_VARIABLES_H */