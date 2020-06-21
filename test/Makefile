initest:
	@gcc -Wall -std=c11 -L../bin -I../include initest.c -o initest -liniparser_so

getsettingstest:
	@gcc -Wall -std=c11 -L../lib -I../include get_settings_test.c ../src/initialize_functions.c -o get_settings_test -lmingw32 -lSDL2main -lSDL2 -liniparser_so