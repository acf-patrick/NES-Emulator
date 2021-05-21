#pragma once

#include "defs.h"

class Mmu
{
    private:
        uint8_t *RAM;    
    public:
        Mmu();
        ~Mmu();
        uint8_t readByte(uint16_t address);                     //reading a byte (8bit unsigned integer) from RAM at given address 
        uint16_t readWord(uint16_t address);                    //reading a word (16bit unsigned integer in little endian) from RAM at given address+1 first then address (because of little endianess)
        void  writeByte(uint8_t value, uint16_t address);       //write a byte to given address in RAM
};