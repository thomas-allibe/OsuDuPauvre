initest:
	@gcc -Wall -std=c11 -L../bin -I../include initest.c -o initest -liniparser_so

getsettingstest:
	@gcc -Wall -std=c11 -L../lib -I../include get_settings_test.c ../src/initialize_functions.c -o get_settings_test -lmingw32 -lSDL2main -lSDL2 -liniparser_so

sdltestmsgbox:
	gcc -Wall -Wextra -L ../lib -I ../include sdl_test_msgbox.c -o sdl_test_msgbox -lmingw32 -lSDL2main -lSDL2

sdltest:
	gcc -Wall -Wextra -L ../lib -I ../include sdl_test.c -o sdl_test -lmingw32 -lSDL2main -lSDL2
