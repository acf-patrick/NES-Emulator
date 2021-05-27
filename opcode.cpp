#include "cpu6502.h"

/*****LDA INSTRUCTIONS******/

void Cpu6502::LDA_IMM()                  //loading A register with immediate addressing mode
{
    A = mmu->readByte(PC+1);            //A is loaded with RAM[PC+1]
    PC += 2;                            //PC is incremented by 2 for next instruction
    cycles += 2;                        //instruction take 2 cycles
    S[Z] = A == 0;      
    S[C] = A & (1<<7);  
}

void Cpu6502::LDA_ZP()                  //loading A register with zero page addressing mode
{
    A = mmu->readByte(mmu->readByte(PC+1));            //A is loaded with the next 8bit value in memory aka RAM[RAM[PC+1]]
    PC += 2;                            //PC is incremented by 2 for next instruction
    cycles += 3;                        //instruction take 3 cycles
    S[Z] = A == 0;      
    S[C] = A & (1<<7); 
}

void Cpu6502::LDA_ZPX()                  //loading A register with zero page + X addressing mode
{
    A = mmu->readByte(mmu->readByte(PC+1) + X);        //A is loaded with the next 8bit value in memory + X aka RAM[RAM[PC+1 + X]]
    PC += 2;                            //PC is incremented by 2 for next instruction
    cycles += 4;                        //instruction take 4 cycles
    S[Z] = A == 0;      
    S[C] = A & (1<<7); 
}

void Cpu6502::LDA_ABS()                //loading A register with absolute addressing mode
{
    A = mmu->readByte(mmu->readWord(PC+1));
    PC += 3;
    cycles += 4;
    S[Z] = A == 0;      
    S[C] = A & (1<<7); 
}

void Cpu6502::LDA_ABSX()                                    //loading A register with absolute + X addressing mode
{
    A = mmu->readByte(mmu->readWord(PC+1) + X);             //reading the next 2 bytes in little endian mode then adding X to it, then load it into A
    cycles += ((PC+3 & 0xFF00) > (PC & 0xFF00)) ? 5 : 4;      //if page is crossed (ex: PC=0x00FF and after the instruction PC=0x0101), cycles is incremented by 5, 4 otherwise
    PC += 3;                                                //PC is incremented by 3 for next instruction
    S[Z] = A == 0;      
    S[C] = A & (1<<7); 
}

void Cpu6502::LDA_ABSY()                //loading A register with absolute + X addressing mode
{
    A = mmu->readByte(mmu->readWord(PC+1) + Y);              //reading the next 2 bytes in little endian mode then adding Y to it, then load it into A
    cycles += ((PC+3 & 0xFF00) > (PC & 0xFF00)) ? 5 : 4;       //if page is crossed (ex: PC=0x00FF and after the instruction PC=0x0101), cycles is incremented by 5, 4 otherwise
    PC += 3;                                                 //PC is incremented by 3 for next instruction
    S[Z] = A == 0;      
    S[C] = A & (1<<7); 
}
                                                                  
void Cpu6502::LDA_IX()                                                //loading A register with (indirect + X) addressing mode
{
    A = mmu->readByte(mmu->readWord(mmu->readByte(PC+1)+ X));         //read a byte at PC+1 then add X to it, the read the next 2 bytes in little endian mode from that and lastly read a byte from the 2 bytes 
    cycles += 6;                                                      //if page is crossed (ex: PC=0x00FF and after the instruction PC=0x0101), cycles is incremented by 6, 5 otherwise
    PC += 2;                                                          //PC is incremented by 2 for next instruction
    S[Z] = A == 0;      
    S[C] = A & (1<<7); 
    /* A little example here, may delete this after knowing how it works
        X = 0x04;
        RAM[PC+1] = 0x02;
        Word value = RAM[0x02 + 0x04 + 1] | RAM[0x02 + 0x04];
        A = RAM[value];
     */
}

void Cpu6502::LDA_IY()                                                //loading A register with (indirect) + Y addressing mode
{
    A = mmu->readByte(mmu->readWord(mmu->readByte(PC+1)) + Y);        //read a byte at PC+1 then read the next 2 bytes in little endian mode from that, add Y to it,  and lastly read a byte from the 2 bytes 
    cycles += ((PC+2 & 0xFF00) > (PC & 0xFF00)) ? 6 : 5;              //if page is crossed (ex: PC=0x00FF and after the instruction PC=0x0101), cycles is incremented by 6, 5 otherwise
    PC += 2;                                                          //PC is incremented by 2 for next instruction
    S[Z] = A == 0;      
    S[C] = A & (1<<7); 
    /* A little example here, may delete this after knowing how it works
        Y = 0x04;
        RAM[PC+1] = 0x02;
        Word value = RAM[0x02 + 1] | RAM[0x02];
        value += Y;
        A = RAM[value];
     */
}


/******LDX INSTRUCTIONS*****/

void Cpu6502::LDX_IMM()                  //loading X register with immediate addressing mode
{
    X = mmu->readByte(PC+1);            //X is loaded with RAM[PC+1]
    PC += 2;                            //PC is incremented by 2 for next instruction
    cycles += 2;                        //instruction take 2 cycles
    S[Z] = X == 0;      
    S[C] = X & (1<<7);  
}

void Cpu6502::LDX_ZP()                                   //loading X register with zero page addressing mode
{
    X = mmu->readByte(mmu->readByte(PC+1));              //X is loaded with the next 8bit value in memory aka RAM[RAM[PC+1]]
    PC += 2;                                             //PC is incremented by 2 for next instruction
    cycles += 3;                                         //instruction take 3 cycles
    S[Z] = X == 0;      
    S[C] = X & (1<<7);  
}

void Cpu6502::LDX_ZPY()                                  //loading X register with zero page + Y addressing mode
{
    X = mmu->readByte(mmu->readByte(PC+1) + X);          //X is loaded with the next 8bit value in memory + Y aka RAM[RAM[PC+1 + Y]]
    PC += 2;                                             //PC is incremented by 2 for next instruction
    cycles += 4;                                         //instruction take 4 cycles
    S[Z] = X == 0;      
    S[C] = X & (1<<7);  
}

void Cpu6502::LDX_ABS()                //loading X register with absolute addressing mode
{
    X = mmu->readByte(mmu->readWord(PC+1));
    PC += 3;
    cycles += 4;
    S[Z] = X == 0;      
    S[C] = X & (1<<7);  
}

void Cpu6502::LDX_ABSY()                //loading X register with absolute + Y addressing mode
{
    X = mmu->readByte(mmu->readWord(PC+1) + Y);              //reading the next 2 bytes in little endian mode then adding Y to it, then load it into X
    cycles += ((PC+3 & 0xFF00) > (PC & 0xFF00)) ? 5 : 4;       //if page is crossed (ex: PC=0x00FF and after the instruction PC=0x0101), cycles is incremented by 5, 4 otherwise
    PC += 3;                                                 //PC is incremented by 3 for next instruction
    S[Z] = X == 0;      
    S[C] = X & (1<<7);  
}
                                                                  
/******LDY INSTRUCTIONS*****/

void Cpu6502::LDY_IMM()                  //loading Y register with immediate addressing mode
{
    Y = mmu->readByte(PC+1);            //Y is loaded with RAM[PC+1]
    PC += 2;                            //PC is incremented by 2 for next instruction
    cycles += 2;                        //instruction take 2 cycles
    S[Z] = Y == 0;      
    S[C] = Y & (1<<7);  
}

void Cpu6502::LDY_ZP()                                   //loading Y register with zero page addressing mode
{
    Y = mmu->readByte(mmu->readByte(PC+1));              //Y is loaded with the next 8bit value in memory aka RAM[RAM[PC+1]]
    PC += 2;                                             //PC is incremented by 2 for next instruction
    cycles += 3;                                         //instruction take 3 cycles
    S[Z] = Y == 0;      
    S[C] = Y & (1<<7); 
}

void Cpu6502::LDY_ZPX()                                  //loading Y register with zero page + X addressing mode
{
    Y = mmu->readByte(mmu->readByte(PC+1) + X);          //Y is loaded with the next 8bit value in memory + Y aka RAM[RAM[PC+1 + Y]]
    PC += 2;                                             //PC is incremented by 2 for next instruction
    cycles += 4;                                         //instruction take 4 cycles
    S[Z] = Y == 0;      
    S[C] = Y & (1<<7); 
}

void Cpu6502::LDY_ABS()                //loading Y register with absolute addressing mode
{
    Y = mmu->readByte(mmu->readWord(PC+1));
    PC += 3;
    cycles += 4;
    S[Z] = Y == 0;      
    S[C] = Y & (1<<7); 
}

void Cpu6502::LDY_ABSX()                //loading Y register with absolute + X addressing mode
{
    Y =mmu->readByte(mmu->readWord(PC+1) + Y);              //reading the next 2 bytes in little endian mode then adding Y to it, then load it into X
    cycles += ((PC+3 & 0xFF00) > (PC & 0xFF00)) ? 5 : 4;       //if page is crossed (ex: PC=0x00FF and after the instruction PC=0x0101), cycles is incremented by 5, 4 otherwise
    PC += 3;                                                 //PC is incremented by 3 for next instruction
    S[Z] = Y == 0;      
    S[C] = Y & (1<<7); 
}

//STA Instructions
void Cpu6502::STA_ZP()
{
    mmu->writeByte(A, mmu->readByte(PC+1));         //writing A value into RAM[RAM[PC+1]] at zero page
    cycles += 3;
    PC += 2;
}

void Cpu6502::STA_ZPX()
{
    mmu->writeByte(A, mmu->readByte(PC+1) + X);         //writing A value into RAM[RAM[PC+1] + X] at zero page 
    cycles += 4;
    PC += 2;
}

void Cpu6502::STA_ABS()
{
    mmu->writeByte(A, mmu->readWord(PC+1));         //writing A value into RAM[next 2 bytes in little endianess]
    cycles += 4;
    PC += 3;
}

void Cpu6502::STA_ABSX()
{
    mmu->writeByte(A, mmu->readWord(PC+1) + X);         //writing A value into RAM[next 2 bytes in little endianess + X]
    cycles += 5;
    PC += 3;
}

void Cpu6502::STA_ABSY()
{
    mmu->writeByte(A, mmu->readWord(PC+1) + Y);         //writing A value into RAM[next 2 bytes in little endianess + Y]
    cycles += 5;
    PC += 3;
}

void Cpu6502::STA_IX()
{
    mmu->writeByte(A, mmu->readWord(mmu->readByte(PC+1) + X));      //read next byte, add x to it. then write A at next 2 bytes staring from last addition
    cycles += 6;
    PC += 2;
}

void Cpu6502::STA_IY()
{
    mmu->writeByte(A, mmu->readWord(mmu->readByte(PC+1)) + Y);      //read next byte. then write A at next 2 bytes staring from last addition + Y
    cycles += 6;
    PC += 2;
}


//STX Instructions
void Cpu6502::STX_ZP()
{
    mmu->writeByte(X, mmu->readByte(PC+1));         //writing X value into RAM[RAM[PC+1]] at zero page
    cycles += 3;
    PC += 2;
}

void Cpu6502::STX_ZPY()
{
    mmu->writeByte(X, mmu->readByte(PC+1) + Y);         //writing X value into RAM[RAM[PC+1] + Y] at zero page 
    cycles += 4;
    PC += 2;
}

void Cpu6502::STX_ABS()
{
    mmu->writeByte(X, mmu->readWord(PC+1));         //writing X value into RAM[next 2 bytes in little endianess]
    cycles += 4;
    PC += 3;
}

//STY Instructions
void Cpu6502::STY_ZP()
{
    mmu->writeByte(Y, mmu->readByte(PC+1));         //writing Y value into RAM[RAM[PC+1]] at zero page
    cycles += 3;
    PC += 2;
}

void Cpu6502::STY_ZPX()
{
    mmu->writeByte(Y, mmu->readByte(PC+1) + X);         //writing X value into RAM[RAM[PC+1] + Y] at zero page 
    cycles += 4;
    PC += 2;
}

void Cpu6502::STY_ABS()
{
    mmu->writeByte(Y, mmu->readWord(PC+1));         //writing X value into RAM[next 2 bytes in little endianess]
    cycles += 4;
    PC += 3;
}


//JSR Instruction
void Cpu6502::JSR()
{
    PushPCtoStack();            //push next instruction to stack
    PC = mmu->readWord(PC+1);   //set the PC to next 2 bytes in little endianess
    cycles += 6;
}

//RST Instruction
void Cpu6502::RTS()                      //Return from subroutine
{
    PopPCfromStack();           //pop previous instruction to be executed from stack
    cycles += 6;
}

//JMP Instruction
/*
    TODO: An original 6502 has does not correctly fetch the target address if the indirect vector falls on a page boundary 
    (e.g. $xxFF where xx is any value from $00 to $FF). 
    In this case fetches the LSB from $xxFF as expected but takes the MSB from $xx00. 
    This is fixed in some later chips like the 65SC02 so for compatibility always ensure the indirect vector is not at the end of the page.
    */

void Cpu6502::JMP_ABS()
{
   PC = mmu->readWord(PC+1);                //set PC to next 2 bytes in little endianess
   cycles += 3;
   PC += 3;
}

void Cpu6502::JMP_I()
{
    PC = mmu->readWord(mmu->readWord(PC+1));    //get next 2 bytes in litle endian, then get next 2 bytes from that and set PC to it
    cycles += 5;
    PC += 3;
}

//TAX Instruction
void Cpu6502::TAX()
{
    X = A;
    S[Z] = X == 0;
    S[N] = X & (1 << 7);
    cycles += 2;
    PC ++;
}

//TAY Instruction
void Cpu6502::TAY()
{
    Y = A;
    S[Z] = Y == 0;
    S[N] = Y & (1 << 7);
    cycles += 2;
    PC ++;
}

//TXA Instruction
void Cpu6502::TXA()
{
    A = X;
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 2;
    PC ++;
}

//TYA Instruction
void Cpu6502::TYA()
{
    A = Y;
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 2;
    PC ++;
}

//TSX Instruction
void Cpu6502::TSX()
{
    X = SP;
    S[Z] = X == 0;
    S[N] = X & (1 << 7);
    cycles += 2;
    PC++;
}

//TXS Instruction
void Cpu6502::TXS()
{
    SP = X;
    cycles += 2;
    PC++;
}

//PHP Inxtruction
void Cpu6502::PHP()
{
    mmu->writeByte(S.to_ulong(), 0x0100 | SP);
    SP --;
    cycles += 3;
    PC ++;
}

//PHA Instruction
void Cpu6502::PHA()
{
    mmu->writeByte(A, 0x0100 | SP);
    SP --;
    cycles += 3;
    PC ++;
}

//PLA Instruction
void Cpu6502::PLA()
{
    SP ++;
    A = mmu->readByte(0x0100 | SP);
    mmu->writeByte(0, 0x0100 | SP);
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 4;
    PC ++;
}

//PLP Instruction
void Cpu6502::PLP()
{
    SP ++;
    S = mmu->readByte(0x0100 | SP);
    mmu->writeByte(0, 0x0100 | SP);
    cycles += 4;
    PC ++;
}

//AND Instructions
void Cpu6502::AND_IMM()
{
    A &= mmu->readByte(PC+1);
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 2;
    PC += 2;
}

void Cpu6502::AND_ZP()
{
    A &= mmu->readByte(mmu->readByte(PC+1));
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 3;
    PC += 2;
}

void Cpu6502::AND_ZPX()
{
    A &= mmu->readByte(mmu->readByte(PC+1) + X);
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 4;
    PC += 2;
}

void Cpu6502::AND_ABS()
{
    A &= mmu->readByte(mmu->readWord(PC+1));
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 4;
    PC += 3;
}

void Cpu6502::AND_ABSX()
{
    A &= mmu->readByte(mmu->readWord(PC+1) + X);
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += ((PC+3 & 0xFF00) > (PC & 0xFF00)) ? 5 : 4;
    PC += 3;
}

void Cpu6502::AND_ABSY()
{
    A &= mmu->readByte(mmu->readWord(PC+1) + Y);
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += ((PC+3 & 0xFF00) > (PC & 0xFF00)) ? 5 : 4;
    PC += 3;
}

void Cpu6502::AND_IX()
{
    A &= mmu->readByte(mmu->readWord(mmu->readByte(PC+1)+ X));
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 6;
    PC += 2;
}

void Cpu6502::AND_IY()
{
    A &= mmu->readByte(mmu->readWord(mmu->readByte(PC+1)) + Y);
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += ((PC+2 & 0xFF00) > (PC & 0xFF00)) ? 6 : 5;
    PC += 2;
}

//EOR Instructions
void Cpu6502::EOR_IMM()
{
    A ^= mmu->readByte(PC+1);
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 2;
    PC += 2;
}

void Cpu6502::EOR_ZP()
{
    A ^= mmu->readByte(mmu->readByte(PC+1));
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 3;
    PC += 2;
}

void Cpu6502::EOR_ZPX()
{
    A ^= mmu->readByte(mmu->readByte(PC+1) + X);
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 4;
    PC += 2;
}

void Cpu6502::EOR_ABS()
{
    A ^= mmu->readByte(mmu->readWord(PC+1));
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 4;
    PC += 3;
}

void Cpu6502::EOR_ABSX()
{
    A ^= mmu->readByte(mmu->readWord(PC+1) + X);
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += ((PC+3 & 0xFF00) > (PC & 0xFF00)) ? 5 : 4;
    PC += 3;
}

void Cpu6502::EOR_ABSY()
{
    A ^= mmu->readByte(mmu->readWord(PC+1) + Y);
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += ((PC+3 & 0xFF00) > (PC & 0xFF00)) ? 5 : 4;
    PC += 3;
}

void Cpu6502::EOR_IX()
{
    A ^= mmu->readByte(mmu->readWord(mmu->readByte(PC+1)+ X));
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 6;
    PC += 2;
}

void Cpu6502::EOR_IY()
{
    A ^= mmu->readByte(mmu->readWord(mmu->readByte(PC+1)) + Y);
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += ((PC+2 & 0xFF00) > (PC & 0xFF00)) ? 6 : 5;
    PC += 2;
}

//ORA Instructions
void Cpu6502::ORA_IMM()
{
    A |= mmu->readByte(PC+1);
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 2;
    PC += 2;
}

void Cpu6502::ORA_ZP()
{
    A |= mmu->readByte(mmu->readByte(PC+1));
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 3;
    PC += 2;
}

void Cpu6502::ORA_ZPX()
{
    A |= mmu->readByte(mmu->readByte(PC+1) + X);
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 4;
    PC += 2;
}

void Cpu6502::ORA_ABS()
{
    A |= mmu->readByte(mmu->readWord(PC+1));
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 4;
    PC += 3;
}

void Cpu6502::ORA_ABSX()
{
    A |= mmu->readByte(mmu->readWord(PC+1) + X);
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += ((PC+3 & 0xFF00) > (PC & 0xFF00)) ? 5 : 4;
    PC += 3;
}

void Cpu6502::ORA_ABSY()
{
    A |= mmu->readByte(mmu->readWord(PC+1) + Y);
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += ((PC+3 & 0xFF00) > (PC & 0xFF00)) ? 5 : 4;
    PC += 3;
}

void Cpu6502::ORA_IX()
{
    A |= mmu->readByte(mmu->readWord(mmu->readByte(PC+1)+ X));
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 6;
    PC += 2;
}

void Cpu6502::ORA_IY()
{
    A |= mmu->readByte(mmu->readWord(mmu->readByte(PC+1)) + Y);
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += ((PC+2 & 0xFF00) > (PC & 0xFF00)) ? 6 : 5;
    PC += 2;
}

//BIT Instructions
void Cpu6502::BIT_ZP()
{
    Byte value = mmu->readByte(mmu->readByte(PC+1));
    S[Z] = A & value == 0 ? 1 : 0;
    S[V] = value & (1 << 6);
    S[N] = value & (1 << 7);
    cycles += 3;
    PC += 2;
}

void Cpu6502::BIT_ABS()
{
    Byte value = mmu->readByte(mmu->readWord(PC+1));
    S[Z] = A & value == 0 ? 1 : 0;
    S[V] = value & (1 << 6);
    S[N] = value & (1 << 7);
    cycles += 4;
    PC += 3;
}

//ADC Instructions
void Cpu6502::ADC_IMM()
{
    Byte nextByte = mmu->readByte(PC+1);
    Word tmp = (Word)A + (Word)nextByte + (Word)S[C] ? 1 : 0;
    S[C] = (tmp > 255);
    S[Z] = (tmp & 0x00FF) == 0;
    S[V] = ( ((Word)A ^ (Word)nextByte) & ((Word)A ^ (Word)tmp) & 0x80 );
    S[N] = tmp & 0x80;
    A = tmp & 0x00FF;
    cycles += 2;
    PC += 2;
}

void Cpu6502::ADC_ZP()
{
    Byte nextByte = mmu->readByte(mmu->readByte(PC+1));
    Word tmp = (Word)A + (Word)nextByte + (Word)S[C] ? 1 : 0;
    S[C] = (tmp > 255);
    S[Z] = (tmp & 0x00FF) == 0;
    S[V] = ( ((Word)A ^ (Word)nextByte) & ((Word)A ^ (Word)tmp) & 0x80 );
    S[N] = tmp & 0x80;
    A = tmp & 0x00FF;
    cycles += 3;
    PC += 2;
}

void Cpu6502::ADC_ZPX()
{
    Byte nextByte = mmu->readByte(mmu->readByte(PC+1) + X);
    Word tmp = (Word)A + (Word)nextByte + (Word)S[C] ? 1 : 0;
    S[C] = (tmp > 255);
    S[Z] = (tmp & 0x00FF) == 0;
    S[V] = ( ((Word)A ^ (Word)nextByte) & ((Word)A ^ (Word)tmp) & 0x80 );
    S[N] = tmp & 0x80;
    A = tmp & 0x00FF;
    cycles += 4;
    PC += 2;
}

void Cpu6502::ADC_ABS()
{
    Byte nextByte = mmu->readByte(mmu->readWord(PC+1));
    Word tmp = (Word)A + (Word)nextByte + (Word)S[C] ? 1 : 0;
    S[C] = (tmp > 255);
    S[Z] = (tmp & 0x00FF) == 0;
    S[V] = ( ((Word)A ^ (Word)nextByte) & ((Word)A ^ (Word)tmp) & 0x80 );
    S[N] = tmp & 0x80;
    A = tmp & 0x00FF;
    cycles += 4;
    PC += 3;
}

void Cpu6502::ADC_ABSX()
{
    Byte nextByte = mmu->readByte(mmu->readWord(PC+1) + X);
    Word tmp = (Word)A + (Word)nextByte + (Word)S[C] ? 1 : 0;
    S[C] = (tmp > 255);
    S[Z] = (tmp & 0x00FF) == 0;
    S[V] = ( ((Word)A ^ (Word)nextByte) & ((Word)A ^ (Word)tmp) & 0x80 );
    S[N] = tmp & 0x80;
    A = tmp & 0x00FF;
    cycles += ((PC + 3) & 0xFF00 > PC & 0xFF00) ? 5 : 4;
    PC += 3;
}

void Cpu6502::ADC_ABSY()
{
    Byte nextByte = mmu->readByte(mmu->readWord(PC+1) + Y);
    Word tmp = (Word)A + (Word)nextByte + (Word)S[C] ? 1 : 0;
    S[C] = (tmp > 255);
    S[Z] = (tmp & 0x00FF) == 0;
    S[V] = ( ((Word)A ^ (Word)nextByte) & ((Word)A ^ (Word)tmp) & 0x80 );
    S[N] = tmp & 0x80;
    A = tmp & 0x00FF;
    cycles += ((PC + 3) & 0xFF00 > PC & 0xFF00) ? 5 : 4;
    PC += 3;
}

void Cpu6502::ADC_IX()
{
    Byte nextByte = mmu->readByte(mmu->readWord(mmu->readByte(PC+1) + X));
    Word tmp = (Word)A + (Word)nextByte + (Word)S[C] ? 1 : 0;
    S[C] = (tmp > 255);
    S[Z] = (tmp & 0x00FF) == 0;
    S[V] = ( ((Word)A ^ (Word)nextByte) & ((Word)A ^ (Word)tmp) & 0x80 );
    S[N] = tmp & 0x80;
    A = tmp & 0x00FF;
    cycles += 6;
    PC += 2;
}

void Cpu6502::ADC_IY()
{
    Byte nextByte = mmu->readByte(mmu->readWord(mmu->readByte(PC+1)) + Y);
    Word tmp = (Word)A + (Word)nextByte + (Word)S[C] ? 1 : 0;
    S[C] = (tmp > 255);
    S[Z] = (tmp & 0x00FF) == 0;
    S[V] = ( ((Word)A ^ (Word)nextByte) & ((Word)A ^ (Word)tmp) & 0x80 );
    S[N] = tmp & 0x80;
    A = tmp & 0x00FF;
    cycles += ((PC + 3) & 0xFF00 > PC & 0xFF00) ? 6 : 5;
    PC += 2;
}

//SBC Instructions
void Cpu6502::SBC_IMM()
{
    Byte nextByte = mmu->readByte(PC+1);
    Word value = (Word)nextByte ^ 0x00FF;
    Word tmp = (Word)A + value + (Word)S[C] ? 1 : 0;
    S[C] = (tmp > 255);
    S[Z] = (tmp & 0x00FF) == 0;
    S[V] = ( ((Word)A ^ (Word)nextByte) & ((Word)A ^ (Word)tmp) & 0x80 );
    S[N] = tmp & 0x80;
    A = tmp & 0x00FF;
    cycles += 2;
    PC += 2;
}

void Cpu6502::SBC_ZP()
{
    Byte nextByte = mmu->readByte(mmu->readByte(PC+1));
    Word value = (Word)nextByte ^ 0x00FF;
    Word tmp = (Word)A + value + (Word)S[C] ? 1 : 0;
    S[C] = (tmp > 255);
    S[Z] = (tmp & 0x00FF) == 0;
    S[V] = ( ((Word)A ^ (Word)nextByte) & ((Word)A ^ (Word)tmp) & 0x80 );
    S[N] = tmp & 0x80;
    A = tmp & 0x00FF;
    cycles += 3;
    PC += 2;
}

void Cpu6502::SBC_ZPX()
{
    Byte nextByte = mmu->readByte(mmu->readByte(PC+1) + X);
    Word value = (Word)nextByte ^ 0x00FF;
    Word tmp = (Word)A + value + (Word)S[C] ? 1 : 0;
    S[C] = (tmp > 255);
    S[Z] = (tmp & 0x00FF) == 0;
    S[V] = ( ((Word)A ^ (Word)nextByte) & ((Word)A ^ (Word)tmp) & 0x80 );
    S[N] = tmp & 0x80;
    A = tmp & 0x00FF;
    cycles += 4;
    PC += 2;
}

void Cpu6502::SBC_ABS()
{
    Byte nextByte = mmu->readByte(mmu->readWord(PC+1));
    Word value = (Word)nextByte ^ 0x00FF;
    Word tmp = (Word)A + value + (Word)S[C] ? 1 : 0;
    S[C] = (tmp > 255);
    S[Z] = (tmp & 0x00FF) == 0;
    S[V] = ( ((Word)A ^ (Word)nextByte) & ((Word)A ^ (Word)tmp) & 0x80 );
    S[N] = tmp & 0x80;
    A = tmp & 0x00FF;
    cycles += 4;
    PC += 3;
}

void Cpu6502::SBC_ABSX()
{
    Byte nextByte = mmu->readByte(mmu->readWord(PC+1) + X);
    Word value = (Word)nextByte ^ 0x00FF;
    Word tmp = (Word)A + value + (Word)S[C] ? 1 : 0;
    S[C] = (tmp > 255);
    S[Z] = (tmp & 0x00FF) == 0;
    S[V] = ( ((Word)A ^ (Word)nextByte) & ((Word)A ^ (Word)tmp) & 0x80 );
    S[N] = tmp & 0x80;
    A = tmp & 0x00FF;
    cycles += ((PC + 3) & 0xFF00 > PC & 0xFF00) ? 5 : 4;
    PC += 3;
}

void Cpu6502::SBC_ABSY()
{
    Byte nextByte = mmu->readByte(mmu->readWord(PC+1) + Y);
    Word value = (Word)nextByte ^ 0x00FF;
    Word tmp = (Word)A + value + (Word)S[C] ? 1 : 0;
    S[C] = (tmp > 255);
    S[Z] = (tmp & 0x00FF) == 0;
    S[V] = ( ((Word)A ^ (Word)nextByte) & ((Word)A ^ (Word)tmp) & 0x80 );
    S[N] = tmp & 0x80;
    A = tmp & 0x00FF;
    cycles += ((PC + 3) & 0xFF00 > PC & 0xFF00) ? 5 : 4;
    PC += 3;
}

void Cpu6502::SBC_IX()
{
    Byte nextByte = mmu->readByte(mmu->readWord(mmu->readByte(PC+1) + X));
    Word value = (Word)nextByte ^ 0x00FF;
    Word tmp = (Word)A + value + (Word)S[C] ? 1 : 0;
    S[C] = (tmp > 255);
    S[Z] = (tmp & 0x00FF) == 0;
    S[V] = ( ((Word)A ^ (Word)nextByte) & ((Word)A ^ (Word)tmp) & 0x80 );
    S[N] = tmp & 0x80;
    A = tmp & 0x00FF;
    cycles += 6;
    PC += 2;
}

void Cpu6502::SBC_IY()
{
    Byte nextByte = mmu->readByte(mmu->readWord(mmu->readByte(PC+1)) + Y);
    Word value = (Word)nextByte ^ 0x00FF;
    Word tmp = (Word)A + value + (Word)S[C] ? 1 : 0;
    S[C] = (tmp > 255);
    S[Z] = (tmp & 0x00FF) == 0;
    S[V] = ( ((Word)A ^ (Word)nextByte) & ((Word)A ^ (Word)tmp) & 0x80 );
    S[N] = tmp & 0x80;
    A = tmp & 0x00FF;
    cycles += ((PC + 3) & 0xFF00 > PC & 0xFF00) ? 6 : 5;
    PC += 2;
}



//CMP Instructions
void Cpu6502::CMP_IMM()
{
    Byte nextByte = mmu->readByte(PC+1);
    Byte tmp = A - nextByte;
    S[C] = A >= nextByte;
    S[Z] = tmp == 0;
    S[N] = tmp & 0x80;
    cycles += 2;
    PC += 2;
}

void Cpu6502::CMP_ZP()
{
    Byte nextByte = mmu->readByte(mmu->readByte(PC+1));
    Byte tmp = A - nextByte;
    S[C] = A >= nextByte;
    S[Z] = tmp == 0;
    S[N] = tmp & 0x80;
    cycles += 3;
    PC += 2;
}

void Cpu6502::CMP_ZPX()
{
    Byte nextByte = mmu->readByte(mmu->readByte(PC+1) + X);
    Byte tmp = A - nextByte;
    S[C] = A >= nextByte;
    S[Z] = tmp == 0;
    S[N] = tmp & 0x80;
    cycles += 4;
    PC += 2;
}

void Cpu6502::CMP_ABS()
{
    Byte nextByte = mmu->readByte(mmu->readWord(PC+1));
    Byte tmp = A - nextByte;
    S[C] = A >= nextByte;
    S[Z] = tmp == 0;
    S[N] = tmp & 0x80;
    cycles += 4;
    PC += 3;
}

void Cpu6502::CMP_ABSX()
{
    Byte nextByte = mmu->readByte(mmu->readWord(PC+1) + X);
    Byte tmp = A - nextByte;
    S[C] = A >= nextByte;
    S[Z] = tmp == 0;
    S[N] = tmp & 0x80;
    cycles += (((PC + 3) & 0xFF00) > PC & 0xFF00) ? 5 : 4;
    PC += 3;
}

void Cpu6502::CMP_ABSY()
{
    Byte nextByte = mmu->readByte(mmu->readWord(PC+1) + Y);
    Byte tmp = A - nextByte;
    S[C] = A >= nextByte;
    S[Z] = tmp == 0;
    S[N] = tmp & 0x80;
    cycles += (((PC + 3) & 0xFF00) > PC & 0xFF00) ? 5 : 4;
    PC += 3;
}

void Cpu6502::CMP_IX()
{
    Byte nextByte = mmu->readByte(mmu->readWord(mmu->readByte(PC+1) + X));
    Byte tmp = A - nextByte;
    S[C] = A >= nextByte;
    S[Z] = tmp == 0;
    S[N] = tmp & 0x80;
    cycles += 6;
    PC += 2;
}

void Cpu6502::CMP_IY()
{
    Byte nextByte =  mmu->readByte(mmu->readWord(mmu->readByte(PC+1)) + Y);
    Byte tmp = A - nextByte;
    S[C] = A >= nextByte;
    S[Z] = tmp == 0;
    S[N] = tmp & 0x80;
    cycles += (((PC + 3) & 0xFF00) > PC & 0xFF00) ? 6 : 5;
    PC += 2;
}

//CPX Instructions
void Cpu6502::CPX_IMM()
{
    Byte nextByte =  mmu->readByte(PC+1);
    Byte tmp = X - nextByte;
    S[C] = X >= nextByte;
    S[Z] = tmp == 0;
    S[N] = tmp & 0x80;
    cycles += 2;
    PC += 2;
}

void Cpu6502::CPX_ZP()
{
    Byte nextByte =  mmu->readByte(mmu->readByte(PC+1));
    Byte tmp = X - nextByte;
    S[C] = X >= nextByte;
    S[Z] = tmp == 0;
    S[N] = tmp & 0x80;
    cycles += 3;
    PC += 2;
}

void Cpu6502::CPX_ABS()
{
    Byte nextByte =  mmu->readByte(mmu->readWord(PC+1));
    Byte tmp = X - nextByte;
    S[C] = X >= nextByte;
    S[Z] = tmp == 0;
    S[N] = tmp & 0x80;
    cycles += 4;
    PC += 3;
}

//CPY Instructions
void Cpu6502::CPY_IMM()
{
    Byte nextByte =  mmu->readByte(PC+1);
    Byte tmp = Y - nextByte;
    S[C] = Y >= nextByte;
    S[Z] = tmp == 0;
    S[N] = tmp & 0x80;
    cycles += 2;
    PC += 2;
}

void Cpu6502::CPY_ZP()
{
    Byte nextByte =  mmu->readByte(mmu->readByte(PC+1));
    Byte tmp = Y - nextByte;
    S[C] = Y >= nextByte;
    S[Z] = tmp == 0;
    S[N] = tmp & 0x80;
    cycles += 3;
    PC += 2;
}

void Cpu6502::CPY_ABS()
{
    Byte nextByte =  mmu->readByte(mmu->readWord(PC+1));
    Byte tmp = Y - nextByte;
    S[C] = Y >= nextByte;
    S[Z] = tmp == 0;
    S[N] = tmp & 0x80;
    cycles += 4;
    PC += 3;
}

//INC Instructions
void Cpu6502::INC_ZP()
{
    Byte index = mmu->readByte(PC+1);
    Byte value = mmu->readByte(index);
    mmu->writeByte(value + 1, index);
    S[Z] = (value + 1) == 0;
    S[N] = (value + 1) & (1 << 7);
    cycles += 5;
    PC += 2;
}

void Cpu6502::INC_ZPX()
{
    Byte index = mmu->readByte(PC+1) + X;
    Byte value = mmu->readByte(index);
    mmu->writeByte(value + 1, index);
    S[Z] = (value + 1) == 0;
    S[N] = (value + 1) & (1 << 7);
    cycles += 6;
    PC += 2;
}

void Cpu6502::INC_ABS()
{
    Word index = mmu->readWord(PC+1);
    Byte value = mmu->readByte(index);
    mmu->writeByte(value + 1, index);
    S[Z] = (value + 1) == 0;
    S[N] = (value + 1) & (1 << 7);
    cycles += 6;
    PC += 3;
}

void Cpu6502::INC_ABSX()
{
    Word index = mmu->readWord(PC+1) + X;
    Byte value = mmu->readByte(index);
    mmu->writeByte(value + 1, index);
    S[Z] = (value + 1) == 0;
    S[N] = (value + 1) & (1 << 7);
    cycles += 7;
    PC += 3;
}

//INX Instruction
void Cpu6502::INX()
{
    X ++;
    S[Z] = X == 0;
    S[N] = X & (1 << 7);
    cycles += 2;
    PC ++;
}

//INY Instruction
void Cpu6502::INY()
{
    Y ++;
    S[Z] = Y == 0;
    S[N] = Y & (1 << 7);
    cycles += 2;
    PC ++;
}

//DEC Instructions
void Cpu6502::DEC_ZP()
{
    Byte index = mmu->readByte(PC+1);
    Byte value = mmu->readByte(index);
    mmu->writeByte(value - 1, index);
    S[Z] = (value - 1) == 0;
    S[N] = (value - 1) & (1 << 7);
    cycles += 5;
    PC += 2;
}

void Cpu6502::DEC_ZPX()
{
    Byte index = mmu->readByte(PC+1) + X;
    Byte value = mmu->readByte(index);
    mmu->writeByte(value - 1, index);
    S[Z] = (value - 1) == 0;
    S[N] = (value - 1) & (1 << 7);
    cycles += 6;
    PC += 2;
}

void Cpu6502::DEC_ABS()
{
    Word index = mmu->readWord(PC+1);
    Byte value = mmu->readByte(index);
    mmu->writeByte(value - 1, index);
    S[Z] = (value - 1) == 0;
    S[N] = (value - 1) & (1 << 7);
    cycles += 6;
    PC += 3;
}

void Cpu6502::DEC_ABSX()
{
    Word index = mmu->readWord(PC+1) + X;
    Byte value = mmu->readByte(index);
    mmu->writeByte(value - 1, index);
    S[Z] = (value - 1) == 0;
    S[N] = (value - 1) & (1 << 7);
    cycles += 7;
    PC += 3;
}

//DEX Instruction
void Cpu6502::DEX()
{
    X --;
    S[Z] = X == 0;
    S[N] = X & (1 << 7);
    cycles += 2;
    PC ++;
}

//DEY Instruction
void Cpu6502::DEY()
{
    Y --;
    S[Z] = Y == 0;
    S[N] = Y & (1 << 7);
    cycles += 2;
    PC ++;
}

//ASL Instructions
void Cpu6502::ASL()
{
    S[C] = A & (1 << 7);
    std::bitset<8> tmp; 
    for (int i=7; i>=0; i--)      
        tmp[i] = A & (1 << (7-i));
    for (int i=7; i>0; i--)
        tmp[i] = tmp[i-1];
    tmp[0] = 0;
    A = (Byte)(tmp.to_ulong());
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 2;
    PC ++;
}

void Cpu6502::ASL_ZP()
{
    Byte value = mmu->readByte(mmu->readByte(PC+1));
    S[C] = value & (1 << 7);
    std::bitset<8> tmp; 
    for (int i=7; i>=0; i--)      
        tmp[i] = value & (1 << (7-i));
    for (int i=7; i>0; i--)
        tmp[i] = tmp[i-1];
    tmp[0] = 0;
    value = (Byte)(tmp.to_ulong());
    S[Z] = value == 0;
    S[N] = value & (1 << 7);
    cycles += 5;
    PC += 2;
}

void Cpu6502::ASL_ZPX()
{
    Byte value = mmu->readByte(mmu->readByte(PC+1) + X);
    S[C] = value & (1 << 7);
    std::bitset<8> tmp; 
    for (int i=7; i>=0; i--)      
        tmp[i] = value & (1 << (7-i));
    for (int i=7; i>0; i--)
        tmp[i] = tmp[i-1];
    tmp[0] = 0;
    value = (Byte)(tmp.to_ulong());
    S[Z] = value == 0;
    S[N] = value & (1 << 7);
    cycles += 6;
    PC += 2;
}

void Cpu6502::ASL_ABS()
{
    Byte value = mmu->readByte(mmu->readWord(PC+1));
    S[C] = value & (1 << 7);
    std::bitset<8> tmp; 
    for (int i=7; i>=0; i--)      
        tmp[i] = value & (1 << (7-i));
    for (int i=7; i>0; i--)
        tmp[i] = tmp[i-1];
    tmp[0] = 0;
    value = (Byte)(tmp.to_ulong());
    S[Z] = value == 0;
    S[N] = value & (1 << 7);
    cycles += 6;
    PC += 3;
}

void Cpu6502::ASL_ABSX()
{
    Byte value = mmu->readByte(mmu->readWord(PC+1) + X);
    S[C] = value & (1 << 7);
    std::bitset<8> tmp; 
    for (int i=7; i>=0; i--)      
        tmp[i] = value & (1 << (7-i));
    for (int i=7; i>0; i--)
        tmp[i] = tmp[i-1];
    tmp[0] = 0;
    value = (Byte)(tmp.to_ulong());
    S[Z] = value == 0;
    S[N] = value & (1 << 7);
    cycles += 7;
    PC += 3;
}

//LSR Instructions
void Cpu6502::LSR()
{
    S[C] = A & 1;
    std::bitset<8> tmp; 
    for (int i=7; i>=0; i--)      
        tmp[i] = A & (1 << (7-i));
    for (int i=0; i<7; i++)
        tmp[i] = tmp[i+1];
    tmp[7] = 0;
    A = (Byte)(tmp.to_ulong());
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 2;
    PC ++;
}

void Cpu6502::LSR_ZP()
{
    Byte value = mmu->readByte(mmu->readByte(PC+1));
    S[C] = value & 1;
    std::bitset<8> tmp; 
    for (int i=7; i>=0; i--)      
        tmp[i] = value & (1 << (7-i));
    for (int i=0; i<7; i++)
        tmp[i] = tmp[i+1];
    tmp[7] = 0;
    value = (Byte)(tmp.to_ulong());
    S[Z] = value == 0;
    S[N] = value & (1 << 7);
    cycles += 5;
    PC += 2;
}

void Cpu6502::LSR_ZPX()
{
    Byte value = mmu->readByte(mmu->readByte(PC+1) + X);
    S[C] = value & 1;
    std::bitset<8> tmp; 
    for (int i=7; i>=0; i--)      
        tmp[i] = value & (1 << (7-i));
    for (int i=0; i<7; i++)
        tmp[i] = tmp[i+1];
    tmp[7] = 0;
    value = (Byte)(tmp.to_ulong());
    S[Z] = value == 0;
    S[N] = value & (1 << 7);
    cycles += 6;
    PC += 2;
}

void Cpu6502::LSR_ABS()
{
    Byte value = mmu->readByte(mmu->readWord(PC+1));
    S[C] = value & 1;
    std::bitset<8> tmp; 
    for (int i=7; i>=0; i--)      
        tmp[i] = value & (1 << (7-i));
    for (int i=0; i<7; i++)
        tmp[i] = tmp[i+1];
    tmp[7] = 0;
    value = (Byte)(tmp.to_ulong());
    S[Z] = value == 0;
    S[N] = value & (1 << 7);
    cycles += 6;
    PC += 3;
}

void Cpu6502::LSR_ABSX()
{
    Byte value = mmu->readByte(mmu->readWord(PC+1) + X);
    S[C] = value & 1;
    std::bitset<8> tmp; 
    for (int i=7; i>=0; i--)      
        tmp[i] = value & (1 << (7-i));
    for (int i=0; i<7; i++)
        tmp[i] = tmp[i+1];
    tmp[7] = 0;
    value = (Byte)(tmp.to_ulong());
    S[Z] = value == 0;
    S[N] = value & (1 << 7);
    cycles += 7;
    PC += 3;
}

//ROL Instructions
void Cpu6502::ROL()
{
    int oldBit7 = A & (1 << 7);
    std::bitset<8> tmp; 
    for (int i=7; i>=0; i--)      
        tmp[i] = A & (1 << (7-i));
    for (int i=7; i>0; i--)
        tmp[i] = tmp[i-1];
    tmp[0] = S[C];
    S[C] = oldBit7;
    A = (Byte)(tmp.to_ulong());
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 2;
    PC ++;
}

void Cpu6502::ROL_ZP()
{
    Byte value = mmu->readByte(mmu->readByte(PC+1));
    int oldBit7 = value & (1 << 7);
    std::bitset<8> tmp; 
    for (int i=7; i>=0; i--)      
        tmp[i] = value & (1 << (7-i));
    for (int i=7; i>0; i--)
        tmp[i] = tmp[i-1];
    tmp[0] = S[C];
    S[C] = oldBit7;
    value = (Byte)(tmp.to_ulong());
    S[Z] = value == 0;
    S[N] = value & (1 << 7);
    cycles += 5;
    PC += 2;
}

void Cpu6502::ROL_ZPX()
{
    Byte value = mmu->readByte(mmu->readByte(PC+1) + X);
    int oldBit7 = value & (1 << 7);
    std::bitset<8> tmp; 
    for (int i=7; i>=0; i--)      
        tmp[i] = value & (1 << (7-i));
    for (int i=7; i>0; i--)
        tmp[i] = tmp[i-1];
    tmp[0] = S[C];
    S[C] = oldBit7;
    value = (Byte)(tmp.to_ulong());
    S[Z] = value == 0;
    S[N] = value & (1 << 7);
    cycles += 6;
    PC += 2;
}

void Cpu6502::ROL_ABS()
{
    Byte value = mmu->readByte(mmu->readWord(PC+1));
    int oldBit7 = value & (1 << 7);
    std::bitset<8> tmp; 
    for (int i=7; i>=0; i--)      
        tmp[i] = value & (1 << (7-i));
    for (int i=7; i>0; i--)
        tmp[i] = tmp[i-1];
    tmp[0] = S[C];
    S[C] = oldBit7;
    value = (Byte)(tmp.to_ulong());
    S[Z] = value == 0;
    S[N] = value & (1 << 7);
    cycles += 6;
    PC += 3;
}

void Cpu6502::ROL_ABSX()
{
    Byte value = mmu->readByte(mmu->readWord(PC+1) + X);
    int oldBit7 = value & (1 << 7);
    std::bitset<8> tmp; 
    for (int i=7; i>=0; i--)      
        tmp[i] = value & (1 << (7-i));
    for (int i=7; i>0; i--)
        tmp[i] = tmp[i-1];
    tmp[0] = S[C];
    S[C] = oldBit7;
    value = (Byte)(tmp.to_ulong());
    S[Z] = value == 0;
    S[N] = value & (1 << 7);
    cycles += 7;
    PC += 3;
}

//ROR Instructions
void Cpu6502::ROR()
{
    int oldBit0 = A & 1;
    std::bitset<8> tmp; 
    for (int i=7; i>=0; i--)      
        tmp[i] = A & (1 << (7-i));
    for (int i=0; i<7; i++)
        tmp[i] = tmp[i+1];
    tmp[7] = S[C];
    S[C] = oldBit0;
    A = (Byte)(tmp.to_ulong());
    S[Z] = A == 0;
    S[N] = A & (1 << 7);
    cycles += 2;
    PC ++;
}

void Cpu6502::ROR_ZP()
{
    Byte value = mmu->readByte(mmu->readByte(PC+1));
    int oldBit0 = value & 1;
    std::bitset<8> tmp; 
    for (int i=7; i>=0; i--)      
        tmp[i] = value & (1 << (7-i));
    for (int i=0; i<7; i++)
        tmp[i] = tmp[i+1];
    tmp[7] = S[C];
    S[C] = oldBit0;
    value = (Byte)(tmp.to_ulong());
    S[Z] = value == 0;
    S[N] = value & (1 << 7);
    cycles += 5;
    PC += 2;
}

void Cpu6502::ROR_ZPX()
{
    Byte value = mmu->readByte(mmu->readByte(PC+1) + X);
    int oldBit0 = value & 1;
    std::bitset<8> tmp; 
    for (int i=7; i>=0; i--)      
        tmp[i] = value & (1 << (7-i));
    for (int i=0; i<7; i++)
        tmp[i] = tmp[i+1];
    tmp[7] = S[C];
    S[C] = oldBit0;
    value = (Byte)(tmp.to_ulong());
    S[Z] = value == 0;
    S[N] = value & (1 << 7);
    cycles += 6;
    PC += 2;
}

void Cpu6502::ROR_ABS()
{
    Byte value = mmu->readByte(mmu->readWord(PC+1));
    int oldBit0 = value & 1;
    std::bitset<8> tmp; 
    for (int i=7; i>=0; i--)      
        tmp[i] = value & (1 << (7-i));
    for (int i=0; i<7; i++)
        tmp[i] = tmp[i+1];
    tmp[7] = S[C];
    S[C] = oldBit0;
    value = (Byte)(tmp.to_ulong());
    S[Z] = value == 0;
    S[N] = value & (1 << 7);
    cycles += 6;
    PC += 3;
}

void Cpu6502::ROR_ABSX()
{
    Byte value = mmu->readByte(mmu->readWord(PC+1) + X);
    int oldBit0 = value & 1;
    std::bitset<8> tmp; 
    for (int i=7; i>=0; i--)      
        tmp[i] = value & (1 << (7-i));
    for (int i=0; i<7; i++)
        tmp[i] = tmp[i+1];
    tmp[7] = S[C];
    S[C] = oldBit0;
    value = (Byte)(tmp.to_ulong());
    S[Z] = value == 0;
    S[N] = value & (1 << 7);
    cycles += 7;
    PC += 3;
}

//BRK Instruction
void Cpu6502::BRK()
{
    PushPCtoStack();
    PushStoSTack();
    /*TODO: check here if interrupt vector is correct (0xFFFE/F)*/
    PC = mmu->readWord(0xFFFE);
    S[B] = 1;
    cycles += 7;
}

//NOP Instruction
void Cpu6502::NOP()
{
    cycles += 2;
    PC++;
}

//RTI Instruction
void Cpu6502::RTI()
{
    PopSFromStack();
    PopPCfromStack();
    cycles += 6;
}

//BCC Instruction
void Cpu6502::BCC()
{
    Byte oldHiByteOfPC = PC & 0xFF00;
    int nextByte = (int)mmu->readByte(PC+1);       //(int) because relative displacement after add
    PC += S[C] ? 2 : nextByte;
    cycles += 2 + (S[C] ? 0 : 1) + (((PC & 0xFF00) > oldHiByteOfPC) ? 2 : 0);
}

//BCS Instruction
void Cpu6502::BCS()
{
    Byte oldHiByteOfPC = PC & 0xFF00;
    int nextByte = (int)mmu->readByte(PC+1);       //(int) because relative displacement after add
    PC += S[C] ? nextByte : 2;
    cycles += 2 + (S[C] ? 1 : 0) + (((PC & 0xFF00) > oldHiByteOfPC) ? 2 : 0);
}

//BNE Instruction
void Cpu6502::BNE()
{
    Byte oldHiByteOfPC = PC & 0xFF00;
    int nextByte = (int)mmu->readByte(PC+1);       //(int) because relative displacement after add
    PC += S[Z] ? 2 : nextByte;
    cycles += 2 + (S[Z] ? 0 : 1) + (((PC & 0xFF00) > oldHiByteOfPC) ? 2 : 0);
}

//BEQ Instruction
void Cpu6502::BEQ()
{
    Byte oldHiByteOfPC = PC & 0xFF00;
    int nextByte = (int)mmu->readByte(PC+1);       //(int) because relative displacement after add
    PC += S[Z] ? nextByte : 2;
    cycles += 2 + (S[C] ? 1 : 0) + (((PC & 0xFF00) > oldHiByteOfPC) ? 2 : 0);
}

//BPL Instruction
void Cpu6502::BPL()
{
    Byte oldHiByteOfPC = PC & 0xFF00;
    int nextByte = (int)mmu->readByte(PC+1);       //(int) because relative displacement after add
    PC += S[N] ? 2 : nextByte;
    cycles += 2 + (S[N] ? 0 : 1) + (((PC & 0xFF00) > oldHiByteOfPC) ? 2 : 0);
}

//BMI Instruction
void Cpu6502::BMI()
{
    Byte oldHiByteOfPC = PC & 0xFF00;
    int nextByte = (int)mmu->readByte(PC+1);       //(int) because relative displacement after add
    PC += S[N] ? nextByte : 2;
    cycles += 2 + (S[N] ? 1 : 0) + (((PC & 0xFF00) > oldHiByteOfPC) ? 2 : 0);
}

//BVC Instruction
void Cpu6502::BVC()
{
    Byte oldHiByteOfPC = PC & 0xFF00;
    int nextByte = (int)mmu->readByte(PC+1);       //(int) because relative displacement after add
    PC += S[V] ? 2 : nextByte;
    cycles += 2 + (S[V] ? 0 : 1) + (((PC & 0xFF00) > oldHiByteOfPC) ? 2 : 0);
}

//BVS Instruction
void Cpu6502::BVS()
{
    Byte oldHiByteOfPC = PC & 0xFF00;
    int nextByte = (int)mmu->readByte(PC+1);       //(int) because relative displacement after add
    PC += S[V] ? nextByte : 2;
    cycles += 2 + (S[V] ? 1 : 0) + (((PC & 0xFF00) > oldHiByteOfPC) ? 2 : 0);
}

//CLC Instruction
void Cpu6502::CLC()
{
    S[C] = 0;
    cycles += 2;
    PC ++;
}

//CLD Instruction
void Cpu6502::CLD()
{
    S[D] = 0;
    cycles += 2;
    PC ++;
}

//CLC Instruction
void Cpu6502::CLI()
{
    S[I] = 0;
    cycles += 2;
    PC ++;
}

//CLV Instruction
void Cpu6502::CLV()
{
    S[V] = 0;
    cycles += 2;
    PC ++;
}

//SEC Instruction
void Cpu6502::SEC()
{
    S[C] = 1;
    cycles += 2;
    PC ++;
}

//SED Instruction
void Cpu6502::SED()
{
    S[D] = 1;
    cycles += 2;
    PC ++;
}

//SEI Instruction
void Cpu6502::SEI()
{
    S[I] = 1;
    cycles += 2;
    PC ++;
}