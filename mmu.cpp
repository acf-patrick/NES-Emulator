#include "mmu.h"

Mmu::Mmu()
{
    RAM = new uint8_t[0xFFFF];            //max memory size is 0xFFFF because the NES have a 16bit cpu?  
    for (int i=0; i<0xFFFF; i++)
        RAM[i] = 0;
}

Mmu::~Mmu()
{
    delete(RAM);
}