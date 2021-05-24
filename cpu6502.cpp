#include "cpu6502.h"

Cpu6502::Cpu6502(Mmu *_mmu)
{
    A = X = Y = 0;
    PC = 0xFFEC;    //  program usually start at 0xFFFC 
    SP = 0xFF;      //  Satck grow bottom top so 0x1FF to 0x100 (in the first page memory)
    S.reset();      //  set all bits to zero
    mmu = _mmu;
    cycles = 0;
}

Cpu6502::~Cpu6502()
{

}

void Cpu6502::reset()            //Reset members to default state
{
    A = X = Y = 0;
    PC = 0xFFEC;     
    SP = 0xFF;      
    S.reset();
}

void Cpu6502::step()
{
    currentOpcode = mmu->readByte(PC);      //getting the current opcode at RAM[PC] (aka: FETCHING ;-) )
    //std::cout << "CurrentOpcode: " << std::hex << (int)currentOpcode << ", at PC = " << std::hex << (int)PC << std::endl;
    switch (currentOpcode)                  //finding what the current opcode is and do the appropriate function (aka: DECODING and EXECUTING ;-) )      
    {

        /*******************LOAD/STORE INSTRUCTIONS*******************/
        //LDA Instructions
        case 0xA9:
            LDA_IMM();
        break;
        case 0xA5:
            LDA_ZP();
        break;
        case 0xB5:
            LDA_ZPX();
        break;
        case 0xAD:
            LDA_ABS();
        break;
        case 0xBD:
            LDA_ABSX();
        break;
        case 0xB9:
            LDA_ABSY();
        break;
        case 0xA1:
            LDA_IX();
        break;
        case 0xB1:
            LDA_IY();
        break;

        //LDX Instructions
        case 0xA2:
            LDX_IMM();
        break;
        case 0xA6:
            LDX_ZP();
        break;
        case 0xB6:
            LDX_ZPY();
        break;
        case 0xAE:
            LDX_ABS();
        break;
        case 0xBE:
            LDX_ABSY();
        break;

        //LDY Instructions
        case 0xA0:
            LDY_IMM();
        break;
        case 0xA4:
            LDY_ZP();
        break;
        case 0xB4:
            LDY_ZPX();
        break;
        case 0xAC:
            LDY_ABS();
        break;
        case 0xBC:
            LDY_ABSX();
        break;

        //STA Instructions
        case 0x85:
            STA_ZP();
        break;
        case 0x95:
            STA_ZPX();
        break;
        case 0x8D:
            STA_ABS();
        break;
        case 0x9D:
            STA_ABSX();
        break;
        case 0x99:
            STA_ABSY();
        break;
        case 0x81:
            STA_IX();
        break;
        case 0x91:
            STA_IY();
        break;

        //STX Instructions
        case 0x86:
            STX_ZP();
        break;
        case 0x96:
            STX_ZPY();
        break;
        case 0x8E:
            STX_ABS();
        break;

        //STY Instructions
        case 0x84:
            STY_ZP();
        break;
        case 0x94:
            STY_ZPX();
        break;
        case 0x8C:
            STY_ABS();
        break;


        /*******************JUMP AND CALLS INSTRUCTIONS*******************/
        //JSR Instruction
        case 0x20:
            JSR();
        break;
        
        //RTS Instruction
        case 0x60:
            RTS();
        break;

        //JMP Instruction
        case 0x4C:
            JMP_ABS();
        break;
        case 0x6C:
            JMP_I();
        break;

        /********************REGISTER TRANSFERS Operations****************/
        //TAX Instruction
        case 0xAA:
            TAX();
        break;
        //TAY Instruction
        case 0xA8:
            TAY();
        break;
        //TXA Instruction
        case 0x8A:
            TXA();
        break;
        //TYA Instruction
        case 0x98:
            TYA();
        break;

        /**************************STACK Operations***********************/
        //TSX Instruction
        case 0xBA:
            TSX();
        break;
        //TXS Instruction
        case 0x9A:
            TXS();
        break;
        //PHA Instruction
        case 0x48:
            PHA();
        break;
        //PHP Instruction
        case 0x08:
            PHP();
        break;
        //PLA Instruction
        case 0x68:
            PLA();
        break;
        //PLP Instruction
        case 0x28:
            PLP();
        break;

        //AND Instructions
        case 0x29:
            AND_IMM();
        break;
        case 0x25:
            AND_ZP();
        break;
        case 0x35:
            AND_ZPX();
        break;
        case 0x2D:
            AND_ABS();
        break;
        case 0x3D:
            AND_ABSX();
        break;
        case 0x39:
            AND_ABSY();
        break;
        case 0x21:
            AND_IX();
        break;
        case 0x31:
            AND_IY();
        break;

        //EOR INstructions
        case 0x49:
            EOR_IMM();
        break;
        case 0x45:
            EOR_ZP();
        break;
        case 0x55:
            EOR_ZPX();
        break;
        case 0x4D:
            EOR_ABS();
        break;
        case 0x5D:
            EOR_ABSX();
        break;
        case 0x59:
            EOR_ABSY();
        break;
        case 0x41:
            EOR_IX();
        break;
        case 0x51:
            EOR_IY();
        break;

        //ORA INstructions
        case 0x09:
            ORA_IMM();
        break;
        case 0x05:
            ORA_ZP();
        break;
        case 0x15:
            ORA_ZPX();
        break;
        case 0x0D:
            ORA_ABS();
        break;
        case 0x1D:
            ORA_ABSX();
        break;
        case 0x19:
            ORA_ABSY();
        break;
        case 0x01:
            ORA_IX();
        break;
        case 0x11:
            ORA_IY();
        break;

        default:
            std::cout << "Unknown Opcode = 0x" << std::hex << (int)currentOpcode << std::endl;
        break;
    }
    /* starting from here, its just for testing LDA , must delete after understanding LDA*/
    std::cout << "A = " << std::hex << (int)A << std::endl;
    std::cout << "X = " << std::hex << (int)X << std::endl;
    std::cout << "Y = " << std::hex << (int)Y << std::endl;
    std::cout << "PC = " << std::hex << (int)PC << std::endl;
    for (int i=0; i<5; i++)
        std::cout << "[" << std::hex << (int)0x01FF - i << "] = " << std::hex << (int)mmu->readByte(0x01FF - i) << std::endl;
    std::cout << "S : NVB*DIZC" << std::endl;
    std::cout << "    ";
    for (int i=0; i<8; i++)
        std::cout << S[i];
    std::cout << std::endl;
    /*end here*/
}

//Status Flag Usefuls Conversions
Byte Cpu6502::StoByte()
{
    int sum = 0;        
    for (int i=7; i>=0; i--)                        //usual power of 2 calculation from binary to decimal
        sum += (S[i] ? 1 : 0) * pow(2 , 7-i);       
    return (Byte)sum;                               //cast to byte for unsigned char (byte)
}

void Cpu6502::ByteToS(Byte byte)
{
    for (int i=7; i>=0; i--)      
        S[i] = byte & (1 << (7-i));
}

//Stack Useful Functions
void Cpu6502::PushPCtoStack()
{
    mmu->writeByte((PC + 3) & 0xFF, 0x0100 | SP);           //writing next instruction lower byte into first page at SP (0x0100 | SP)
    SP--;                                                   //decrementing SP because Stack grows downward
    mmu->writeByte((PC + 3) >> 8, 0x0100 | SP);             //writing next instruction lower byte into first page at SP (0x0100 | SP)
    SP--;                                                   //decrementing SP because Stack grows downward
}

void Cpu6502::PopPCfromStack()
{
    Word value = (mmu->readByte(0x0100 | (SP+1)) << 8) | mmu->readByte(0x0100 | SP+2);      //getting 2 bytes from first page at SP in little endianess
    mmu->writeByte(0, 0x0100 | (SP+2));         //zero-ing previous value of previous stack
    mmu->writeByte(0, 0x0100 | (SP+1));
    SP += 2;                                    //incrementing SP by 2 because we're poping (Stack grows downard)
    PC = value;                                 //setting PC to previous instruction to be executed before jumping 
}


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
    mmu->writeByte(StoByte(), 0x0100 | SP);
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
    ByteToS(mmu->readByte(0x0100 | SP));
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