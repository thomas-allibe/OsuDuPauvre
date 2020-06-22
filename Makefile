#Makefile for c projects

#OS
OS=win

#Handle diffrences between OS for commands
ifeq ($(OS),win)
	DELDIR=rd /s /q
	DELFILE=del
	EXT=.exe
	MKDIR=if not exist "$(subst /,\,$(dir $@))" mkdir $(subst /,\,$(dir $@))
	MKBIN=if not exist "bin" mkdir bin
else ifeq ($(OS),unix)
	DELDIR=rm -rf
	DELFILE=rm -rf
	EXT=
	MKDIR=mkdir -p $(dir $@)
	MKBIN=mkdir -p bin
else
	#Default
	DELCMD=rm -rf
	DELFILE=rm -rf
	EXT=
endif

#Folders
#Do not use the same name for subdirectories eg: src/src/...
SRCDIR = src
OBJDIR = obj
INCDIR = include
BINDIR = bin

#Compilation options
CC = gcc
CFLAGS = -Wall -std=c11 -pedantic -MMD -MP
LDFLAGS = -L ./lib -lmingw32 -lSDL2main -lSDL2 -liniparser_so
INCFLAGS = -I ./$(INCDIR)

#Files
#Paths are relative to the Makefile
EXEC = OsuDuPauvre
SRC = src/main.c
SRC += src/initialize_functions.c src/ini_settings.c src/my_events.c
SRC += src/Classes/game_window.c
# SRC = $(wildcard *.c) can be used instead of manual completion

######################### DO NOT MODIFY #########################

#Create object & dependency names
OBJ = $(subst $(SRCDIR),$(OBJDIR),$(SRC:.c=.o))
DEP = $(OBJ:.o=.d)

#Compilation
all: $(EXEC)
	@echo -----Generating executable : $(EXEC)$(EXT)-----

$(EXEC): $(OBJ)
	@$(MKBIN)
	@$(CC) -o $(BINDIR)/$@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(MKDIR)
	@$(CC) $(CFLAGS) $(INCFLAGS) -o $@ -c $<

#Cleaning
.PHONY: clean mrproper

clean:
	@echo -----Deletion of object files-----
	@$(DELDIR) $(OBJDIR)

mrproper:
	@echo -----Deletion of executable : $(EXEC)$(EXT)-----
	@$(DELFILE) $(EXEC)$(EXT)

startexe:
	@./$(BINDIR)/$(EXEC)$(EXT)
	
# Files of dependencies for headers, generated by gcc
# with -MMD -MP options
-include $(DEP)

# USEFULL LINKS
#
#TUTO FR
#https://gl.developpez.com/tutoriel/outil/makefile/
#
#EN Manual for GNU Make :
#https://www.gnu.org/software/make/manual/make.html
#
#EN Manual for GCC options
#http://tigcc.ticalc.org/doc/comopts.html
#
#EN Stackoverflow post on header dependencies
#https://stackoverflow.com/questions/2394609/makefile-header-dependencies
#
#Template by Thomas ALLIBE
#Inspired by the link above & by Polytech microC project