.SUFFIX:

CXX = g++-10
CFLAGS = -std=c++2a -W

ifeq ($(OS), Windows_NT)
	MOVE = move
	CLEAN = del /Q *.o /S obj/
	LIB = ./SDL2/lib/
else
	MOVE = mv
	CLEAN = rm -f test *.o -r obj/
endif

HEADER = ./SDL2/include/

SRC = cpu6502.cpp mmu.cpp nes.cpp main.cpp debugger.cpp text.cpp box.cpp

SDL = -lSDL2 -lSDL2_ttf

all : obj
	@echo "... Link ..."
	$(CXX) obj/* $(LIB) $(SDL) -o main

obj : $(SRC)
	@echo "... Compile ..."
	$(CXX) $(CFLAGS) -I$(HEADER) -c $?
	@mkdir -p obj/
	@$(MOVE) *.o obj/

clean : 
	@$(CLEAN)