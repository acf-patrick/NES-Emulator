#include "mmu.h"

Mmu::Mmu()
{
    RAM = new Byte[0xFFFF];            //max memory size is 0xFFFF because the NES have a 16bit cpu?  
    for (int i=0; i<0xFFFF; i++)
        RAM[i] = 0;
}

Mmu::~Mmu()
{
    delete(RAM);
}

Byte Mmu::readByte(Word address)
{
    return RAM[address];                     //just getting the value at RAM[address]
}

Word Mmu::readWord(Word address)
{
    Word value = (Word)RAM[address+1];         //getting value at RAM[address+1] (little andian)    
    value <<= 8;
    return (value |= RAM[address]);   //shifting the value by 8bit then OR it with the value at RAM[address]
    /*
    ex:      RAM[0xFFFC] = 0xFE  
               RAM[0xFFFD] = 0xFF
               shifting RAM[0xFFFD] by 8bit will give 0xFF00 and OR-ing it with RAM(0xFFFC) will give 0xFFFE
               readWord(0xFFFC) will return 0xFFFE
    */
}

void Mmu::writeByte(Byte value, Word address)            //just write value to RAM[address]
{
    RAM[address] = value;
}