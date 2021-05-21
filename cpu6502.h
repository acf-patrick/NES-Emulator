#pragma once

#include "defs.h"

class Mmu;

class Cpu6502
{
    private:
        uint8_t A, X , Y;       //Registers
        uint16_t PC;            //ProgramCounter
        uint8_t SP;             //StackPointer
        uint8_t S;              //Status Register
        Mmu *mmu;
    public:
        Cpu6502(Mmu *_mmu);
        ~Cpu6502();
        void reset();
        void step();
};