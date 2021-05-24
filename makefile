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

HEADER = imgui-1.73/ SDL2/

IMGUI  = imgui-1.73/backend/imgui_impl_sdl.cpp imgui-1.73/*.cpp
SRC = cpu6502.cpp mmu.cpp nes.cpp main.cpp

SDL = -lSDL2

all : obj
	@echo "... Link ..."
	$(CXX) obj/* $(SDL) -o main

obj : $(SRC) $(IMGUI)
	@echo "... Compile ..."
	$(CXX) $(CFLAGS) -I$(HEADER) -c $?
	@mkdir -p obj/
	@$(MOVE) *.o obj/

clean : 
	@$(CLEAN)