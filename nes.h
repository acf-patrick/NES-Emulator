#pragma once

#include "mmu.h"
#include "cpu6502.h"
#include "debugger.h"

#include <SDL2/SDL.h>
#include <map>

class Nes
{
private:
    std::map<SDL_Scancode, bool> keys;
    bool running;
    
    Cpu6502 *cpu;
    Mmu *mmu;

    Debugger* debugger = nullptr;

    void drawGame();

public:
    // we only need one of them
    static SDL_Window* window; 
    static SDL_Renderer* renderer;

    Nes();
    ~Nes();

    void run();
};
