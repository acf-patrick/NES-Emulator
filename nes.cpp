#include "nes.h"

Nes::Nes()
{
    mmu = new Mmu();
    cpu = new Cpu6502(mmu);
    running = true;
}

Nes::~Nes()
{

}

void Nes::run()
{
    //while (running)               //this loop is for the future main loop
        cpu->step();
}