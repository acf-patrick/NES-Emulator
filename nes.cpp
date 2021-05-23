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
    /* starting from here, its just for testing , must delete after*/
        mmu->writeByte(0x20, 0xFFEC);
        mmu->writeByte(0x34, 0xFFED);
        mmu->writeByte(0x12, 0xFFEE);
        cpu->step();
        mmu->writeByte(0x60, 0x1234);
        cpu->step();
    /*end here*/  
}