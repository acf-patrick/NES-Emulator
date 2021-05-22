#pragma once

#include "defs.h"

class Mmu;

class Cpu6502
{
    private:
        uint8_t A, X , Y;       //Registers
        uint16_t PC;            //ProgramCounter
        uint8_t SP;             //StackPointer
        uint8_t S;              //Status Register -  NVB*DIZC  (* means not used)
        Mmu *mmu;
        int cycles;             //for counting the number of current cycles
        uint8_t currentOpcode;  //for storing curret opcode where the PC is
    public:
        Cpu6502(Mmu *_mmu);
        ~Cpu6502();
        void reset();
        void step();

        //Flags Useful Function
        void SetFlag(bool value, char FlagName);        //set specific Flag status accordingly to value
        bool GetFlag(char FlagName);                    //get specific Flag status


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
};