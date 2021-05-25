.SUFFIX:

CXX = g++
CFLAGS = -std=c++2a -W

ifeq ($(OS), Windows_NT)
	CLEAN = del *.o
	LIB = "C:\Program Files\CodeBlocks\MinGW\lib"
	HEADER = "C:\Program Files\CodeBlocks\MinGW\include"
	SDL = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
else
	CXX = g++-10
	CLEAN = rm -f *.o
	LIB = .
	HEADER = /usr/include/SDL2/
	SDL = -lSDL2 -lSDL2_ttf
endif


SRC = cpu6502.cpp opcode.cpp mmu.cpp nes.cpp main.cpp debugger.cpp text.cpp box.cpp button.cpp


all : obj
	@echo "... Link ..."
	$(CXX) *.o -L$(LIB) $(SDL) -o main
	@$(CLEAN)

obj : $(SRC)
	@echo "... Compile ..."
	$(CXX) $(CFLAGS) -I$(HEADER) -c $^

clean : 
	@$(CLEAN)