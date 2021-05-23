#pragma once

#include "mmu.h"
#include "cpu6502.h"

class Nes
{
private:
    Cpu6502 *cpu;
    Mmu *mmu;
    bool running;
    
public:
    Nes();
    ~Nes();
    void run();
};
