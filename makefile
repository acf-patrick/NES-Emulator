.SUFFIX:

CXX = g++-10
CFLAGS = -std=c++2a -W

ifeq ($(OS), Windows_NT)
	MOVE = move
	CLEAN = del /Q *.o /S obj/
else
	MOVE = mv
	CLEAN = rm -f test *.o -r obj/
endif

HEADER = SDL2/

SRC = cpu6502.cpp mmu.cpp nes.cpp main.cpp

SDL = -lSDL2 -lSDL2-ttf

all : obj
	@echo "... Link ..."
	$(CXX) obj/* $(SDL) -o main

obj : $(SRC)
	@echo "... Compile ..."
	$(CXX) $(CFLAGS) -I$(HEADER) -c $?
	@mkdir -p obj/
	@$(MOVE) *.o obj/

clean : 
	@$(CLEAN)