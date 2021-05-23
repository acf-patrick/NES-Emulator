#pragma once

#include "defs.h"
#include "mmu.h"

class Debugger;

class Cpu6502
{
private:
    enum Flags { N, V, UNUSED, B, D, I, Z, C };

    Byte A, X , Y;      // Registers
    Word PC;            // ProgramCounter
    Byte SP;            // StackPointer
    
    // SetFlag(value, flagName) becomes S[flagName] = value;
    // GetFlag(flagName) becomes S[flagName];
    std::bitset<8> S;   // Status Register -  NVB*DIZC  (* means not used)

    Mmu *mmu;
    int cycles;         //for counting the number of current cycles
    Byte currentOpcode; //for storing curret opcode where the PC is

public:
    Cpu6502(Mmu *_mmu);
    ~Cpu6502();
    void reset();
    void step();

    //LDA Instructions
    void setLDAFlags();      //setting flags accordingly
    void LDA_IMM();          //loading A register with immediate addressing mode
    void LDA_ZP();           //loading A register with zero page addressing mode
    void LDA_ZPX();          //loading A register with zero page + X addressing mode
    void LDA_ABS();          //loading A register with absolute addressing mode
    void LDA_ABSX();         //loading A register with absolute + x addressing mode
    void LDA_ABSY();         //loading A register with absolute + y addressing mode
    void LDA_IX();           //loading A register with (indirect + X) addressing mode
    void LDA_IY();           //loading A register with (indirect) + Y addressing mode

friend class Debugger;
};