#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "iniparser/iniparser.h"
#include "initialize_functions.h"

int main(int argc, char *argv[]){
    dictionary *settings = NULL;
    char *file = "test.ini";

    SDL_Init(0);
    settings = getSettings(file);

    SDL_SetError("Test Error");
    fprintf(stderr, "SDL_GetError : %s\n", SDL_GetError());

    iniparser_dump(settings, stderr);



    iniparser_freedict(settings);
    SDL_Quit();
    return 0;
}