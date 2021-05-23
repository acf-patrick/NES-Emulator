#pragma once

#include "defs.h"
#include "mmu.h"

class Cpu6502
{
private:
    enum Flags { N, V, UNUSED, B, D, I, Z, C };

    Byte A, X , Y;      // Registers
    Word PC;            // ProgramCounter
    Byte SP;            // StackPointer
    
    std::bitset<8> S;   // Status Register -  NVB*DIZC  (* means not used)

    Mmu *mmu;
    int cycles;         //for counting the number of current cycles
    Byte currentOpcode; //for storing curret opcode where the PC is

public:
    Cpu6502(Mmu *_mmu);
    ~Cpu6502();
    void reset();
    void step();

    //Stack Useful Functions
    void PushPCtoStack();       //push the next instrcutions into stack before jumping to next 2 bytes (little endian)
    void PopPCfromStack();  //pop previous instructions to be executed stocked into stack after doing routines
    

    /***********************LOAD/STORE Operations*********************/
    
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

    //LDX Instructions
    void setLDXFlags();      //setting flags accordingly
    void LDX_IMM();          //loading X register with immediate addressing mode
    void LDX_ZP();           //loading X register with zero page addressing mode
    void LDX_ZPY();          //loading X register with zero page + Y addressing mode
    void LDX_ABS();          //loading X register with absolute addressing mode
    void LDX_ABSY();         //loading X register with absolute + Y addressing mode

    //LDY Instructions
    void setLDYFlags();      //setting flags accordingly
    void LDY_IMM();          //loading Y register with immediate addressing mode
    void LDY_ZP();           //loading Y register with zero page addressing mode
    void LDY_ZPX();          //loading Y register with zero page + X addressing mode
    void LDY_ABS();          //loading Y register with absolute addressing mode
    void LDY_ABSX();         //loading Y register with absolute + X addressing mode

    //STA Instructions
    void STA_ZP();          //storing A register value with zero page addressing mode
    void STA_ZPX();         //storing A register value with zero page + X addressing mode
    void STA_ABS();         //storing A register value with absolute addressing mode
    void STA_ABSX();        //storing A register value with absolute + X addressing mode
    void STA_ABSY();        //storing A register value with absolute + Y addressing mode
    void STA_IX();          //storing A register value with (indirect + X) addressing mode
    void STA_IY();          //storing A register value with (indirect) + Y addressing mode

    //STX Instructions
    void STX_ZP();          //storing X register value with zero page addressing mode
    void STX_ZPY();         //storing X register value with zero page + Y addressing mode
    void STX_ABS();         //storing X register value with absolute addressing mode

    //STY Instructions
    void STY_ZP();          //storing Y register value with zero page addressing mode
    void STY_ZPX();         //storing Y register value with zero page + X addressing mode
    void STY_ABS();         //storing Y register value with absolute addressing mode


    /***************************************************************/


    /**********************JUMP AND CALLS Operations****************/
    
    //JSR Instruction
    void JSR();              //Jump to subroutine

    //RTS Instruction
    void RTS();             //Return from subroutine

    //JMP Instruction
    void JMP_ABS();              //Jump to a specific location in memory in absolute addressing mode
    void JMP_I();                //Jump to a specific location in memory in indirect addressing mode

     /*************************************************************/

};