#pragma once

#include "defs.h"

class Cpu6502;
class Mmu;

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
