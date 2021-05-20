#pragma once

#include "mmu.h"
#include "cpu6502.h"

#include <SDL2/SDL.h>
#include <map>

class Nes
{
private:
    SDL_Window*     window;
    SDL_Renderer* renderer;

    std::map<SDL_Scancode, bool> keys;

    bool running;
    bool debug;
    
private:
    Cpu6502 *cpu;
    Mmu *mmu;

    void drawGame();
    void drawDebugger();

public:
    Nes();
    ~Nes();

    void run();
};
