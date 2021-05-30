.SUFFIX:

CXX = g++
CFLAGS = -std=c++2a -W

ifeq ($(OS), Windows_NT)
	CLEAN = del *.o
	OBJ = *.o
	LIB = "C:\Program Files\CodeBlocks\MinGW\lib"
	HEADER = "C:\Program Files\CodeBlocks\MinGW\include"
	SDL = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
	CLEAN_SCREEN = cls
else
	CXX = g++-10
	OBJ = obj/*
	CLEAN = rm *.o -rf obj/
	LIB = .
	HEADER = /usr/include/SDL2/
	SDL = -lSDL2 -lSDL2_ttf
	MOVE = mkdir -p obj/ && mv *.o obj/
	CLEAN_SCREEN = clear
endif


SRC = opcode.cpp cpu6502.cpp mmu.cpp nes.cpp main.cpp debugger.cpp text.cpp box.cpp button.cpp


all : obj
	@echo "... Link ..."
	$(CXX) $(OBJ) -L$(LIB) $(SDL) -o main

obj : $(SRC)
	@$(CLEAN_SCREEN)
	@echo "... Compile ..."
	$(CXX) $(CFLAGS) -I$(HEADER) -c $?
	@$(MOVE)

clean : 
	@$(CLEAN)
