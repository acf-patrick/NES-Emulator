#include "cpu6502.h"

Cpu6502::Cpu6502(Mmu *_mmu)
{
    A = X = Y = 0;
    PC = 0xFFFC;    //program usually start at 0xFFFC 
    SP = 0xFF;      //Satck grow bottom top so 0x1FF to 0x100 (in the first page memory)
    S = 0;
    mmu = _mmu;
    cycles = 0;
}

Cpu6502::~Cpu6502()
{

}

void Cpu6502::reset()            //Reset members to default state
{
    A = X = Y = 0;
    PC = 0xFFFC;     
    SP = 0xFF;      
    S = 0;
}

void Cpu6502::step()
{
    /* starting from here, its just for testing LDA_IX, must delete after understanding LDA_IX*/
    X = 0x02;
    mmu->writeByte(0xA1, 0xFFFC);
    mmu->writeByte(0x34, 0xFFFD);
    mmu->writeByte(0x12, 0x36);
    mmu->writeByte(0x11, 0x12);
    /* std::cout << std::hex << (int)mmu->readByte(0xFFFC) << std::endl;
    std::cout << std::hex << (int)mmu->readByte(0xFFFD) << std::endl; */
    /*end here*/
    currentOpcode = mmu->readByte(PC);      //getting the current opcode at RAM[PC] (aka: FETCHING ;-) )
    std::cout << "CurrentOpcode: " << std::hex << (int)currentOpcode << ", at PC = " << std::hex << (int)PC << std::endl;
    switch (currentOpcode)                  //finding what the current opcode is and do the appropriate function (aka: DECODING and EXECUTING ;-) )      
    {
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


        default:
            std::cout << "Unknown Opcode : " << std::hex << currentOpcode << std::endl;
        break;
    }
    /* starting from here, its just for testing LDA , must delete after understanding LDA*/
    std::cout << "A = " << std::hex << (int)A << std::endl;
    std::cout << "X = " << std::hex << (int)X << std::endl;
    std::cout << "Y = " << std::hex << (int)Y << std::endl;
    std::cout << "PC = " << std::hex << (int)PC << std::endl;
    std::cout << "SP = " << std::hex << (int)SP << std::endl;
    std::cout << "S : NVB*DIZC" << std::endl;
    std::cout << "    " << std::bitset<8>((int)S) << std::endl << std::endl;
    /*end here*/
}


void Cpu6502::SetFlag(bool value, char FlagName)        //set specific Flag status accordingly to value
{
    switch (FlagName)
    {
        case 'N':
            if (value)
                S |= 0b10000000;
            else S &= ~0b10000000;
        break;
        case 'V':
            if (value)
                S |= 0b01000000;
            else S &= ~0b01000000;
        break;
        case 'B':
            if (value)
                S |= 0b00100000;
            else S &= ~0b00100000;
        break;
        case 'D':
            if (value)
                S |= 0b00001000;
            else S &= ~0b00001000;
        break;
        case 'I':
            if (value)
                S |= 0b00000100;
            else S &= ~0b00000100;
        break;
        case 'Z':
            if (value)
                S |= 0b00000010;
            else S &= ~0b00000010;
        break;
        case 'C':
            if (value)
                S |= 0b00000001;
            else S &= ~0b00000001;
        break;
        default:
            std::cout << "Unknow FlagName " << FlagName << ", exiting program..." << std::endl;
            exit(-1);
        break;
    }
}


bool Cpu6502::GetFlag(char FlagName)                    //get specific Flag status
{
    switch (FlagName)
    {
        case 'N':
            return (S & 0b10000000);
        break;
        case 'V':
            return (S & 0b01000000);
        break;
        case 'B':
            return (S & 0b00100000);
        break;
        case 'D':
            return (S & 0b00001000);
        break;
        case 'I':
            return (S & 0b00000100);
        break;
        case 'Z':
            return (S & 0b00000010);
        break;
        case 'C':
            return (S & 0b00000001);
        break;
        default:
            std::cout << "Unknow FlagName " << FlagName << ", exiting program..." << std::endl;
            exit(-1);
        break;
    }
}

void Cpu6502::setLDAFlags()              //set specific Flag status accordingly to value
{
    SetFlag(false, 'C');
    SetFlag(false, 'I');
    SetFlag(false, 'D');
    SetFlag(false, 'B');
    SetFlag(false, 'V');
    SetFlag(((A == 0) ? true : false), 'Z');                        //set if A = 0
    SetFlag(((A & (1 << 7)) ? true : false), 'C');                  //set if bit 7 of A is set
}


void Cpu6502::LDA_IMM()                  //loading A register with immediate addressing mode
{
    A = mmu->readByte(PC+1);            //A is loaded with RAM[PC+1]
    PC += 2;                            //PC is incremented by 2 for next instruction
    cycles += 2;                        //instruction take 2 cycles
    setLDAFlags();
}

void Cpu6502::LDA_ZP()                  //loading A register with zero page addressing mode
{
    A = mmu->readByte(mmu->readByte(PC+1));            //A is loaded with the next 8bit value in memory aka RAM[RAM[PC+1]]
    PC += 2;                            //PC is incremented by 2 for next instruction
    cycles += 3;                        //instruction take 3 cycles
    setLDAFlags();
}

void Cpu6502::LDA_ZPX()                  //loading A register with zero page + X addressing mode
{
    A = mmu->readByte(mmu->readByte(PC+1) + X);        //A is loaded with the next 8bit value in memory + X aka RAM[RAM[PC+1 + X]]
    PC += 2;                            //PC is incremented by 2 for next instruction
    cycles += 4;                        //instruction take 4 cycles
    setLDAFlags();
}

void Cpu6502::LDA_ABS()                //loading A register with absolute addressing mode
{
    A = mmu->readByte(mmu->readWord(PC+1));
    PC += 3;
    cycles += 4;
    setLDAFlags();
}

void Cpu6502::LDA_ABSX()                                    //loading A register with absolute + X addressing mode
{
    A = mmu->readByte(mmu->readWord(PC+1) + X);             //reading the next 2 bytes in little endian mode then adding X to it, then load it into A
    cycles += ((PC+3 & 0xFF00) > (PC & 0xFF00)) ? 5 : 4;      //if page is crossed (ex: PC=0x00FF and after the instruction PC=0x0101), cycles is incremented by 5, 4 otherwise
    PC += 3;                                                //PC is incremented by 3 for next instruction
    setLDAFlags();
}

void Cpu6502::LDA_ABSY()                //loading A register with absolute + X addressing mode
{
    A = mmu->readByte(mmu->readWord(PC+1) + Y);              //reading the next 2 bytes in little endian mode then adding Y to it, then load it into A
    cycles += ((PC+3 & 0xFF00) > (PC & 0xFF00)) ? 5 : 4;       //if page is crossed (ex: PC=0x00FF and after the instruction PC=0x0101), cycles is incremented by 5, 4 otherwise
    PC += 3;                                                 //PC is incremented by 3 for next instruction
    setLDAFlags();
}
                                                                  
void Cpu6502::LDA_IX()                                                //loading A register with (indirect + X) addressing mode
{
    A = mmu->readByte(mmu->readWord(mmu->readByte(PC+1)+ X));         //read a byte at PC+1 then add X to it, the read the next 2 bytes in little endian mode from that and lastly read a byte from the 2 bytes 
    cycles += 6;                                                      //if page is crossed (ex: PC=0x00FF and after the instruction PC=0x0101), cycles is incremented by 6, 5 otherwise
    PC += 2;                                                          //PC is incremented by 2 for next instruction
    setLDAFlags();
    /* A little example here, may delete this after knowing how it works
        X = 0x04;
        RAM[PC+1] = 0x02;
        uint16_t value = RAM[0x02 + 0x04 + 1] | RAM[0x02 + 0x04];
        A = RAM[value];
     */
}

void Cpu6502::LDA_IY()                                                //loading A register with (indirect) + Y addressing mode
{
    A = mmu->readByte(mmu->readWord(mmu->readByte(PC+1)) + Y);        //read a byte at PC+1 then read the next 2 bytes in little endian mode from that, add Y to it,  and lastly read a byte from the 2 bytes 
    cycles += ((PC+2 & 0xFF00) > (PC & 0xFF00)) ? 6 : 5;              //if page is crossed (ex: PC=0x00FF and after the instruction PC=0x0101), cycles is incremented by 6, 5 otherwise
    PC += 2;                                                          //PC is incremented by 2 for next instruction
    setLDAFlags();
    /* A little example here, may delete this after knowing how it works
        Y = 0x04;
        RAM[PC+1] = 0x02;
        uint16_t value = RAM[0x02 + 1] | RAM[0x02];
        value += Y;
        A = RAM[value];
     */
}
  