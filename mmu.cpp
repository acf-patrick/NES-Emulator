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

uint8_t Mmu::readByte(uint16_t address)
{
    return RAM[address];                     //just getting the value at RAM[address]
}

uint16_t Mmu::readWord(uint16_t address)
{
    uint16_t value = (uint16_t)RAM[address+1];         //getting value at RAM[address+1] (little andian)    
    value <<= 8;
    return (value |= RAM[address]);   //shifting the value by 8bit then OR it with the value at RAM[address]
    /*
    ex:      RAM[0xFFFC] = 0xFE  
               RAM[0xFFFD] = 0xFF
               shifting RAM[0xFFFD] by 8bit will give 0xFF00 and OR-ing it with RAM(0xFFFC) will give 0xFFFE
               readWord(0xFFFC) will return 0xFFFE
    */
}

void Mmu::writeByte(uint8_t value, uint16_t address)            //just write value to RAM[address]
{
    RAM[address] = value;
}