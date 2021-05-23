.SUFFIX:

CXX = g++-10
CFLAGS = -std=c++2a -W

SRC = cpu6502.cpp mmu.cpp nes.cpp main.cpp

SDL = -lSDL2

all : obj
	@echo "... Link ..."
	$(CXX) obj/* $(SDL) -o main

obj : $(SRC)
	@echo "... Compile ..."
	$(CXX) $(CFLAGS) -c $?
	@mkdir -p obj/
	@mv *.o obj/

clean : 
	@rm -f test *.o -r obj/