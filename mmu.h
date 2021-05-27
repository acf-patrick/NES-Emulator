#pragma once

#include "defs.h"
#include <fstream>

class Mmu
{
    private:
        Byte *RAM;    
    public:
        Mmu();
        ~Mmu();
        Byte readByte(Word address);                        //reading a byte (8bit unsigned integer) from RAM at given address 
        Word readWord(Word address);                        //reading a word (16bit unsigned integer in little endian) from RAM at given address+1 first then address (because of little endianess)
        void  writeByte(Byte value, Word address);          //write a byte to given address in RAM
        void loadCartridge(const std::string filename);     //loading the cartridge into memory
};