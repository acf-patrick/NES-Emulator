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
    /* starting from here, its just for testing LDA_IMM(), must delete after understanding LDA_IMM() */
    mmu->writeByte(0xA9 ,0xFFFC);
    mmu->writeByte(0x42, 0xFFFD);
    std::cout << std::hex << (int)mmu->readByte(0xFFFC) << std::endl;
    std::cout << std::hex << (int)mmu->readByte(0xFFFD) << std::endl;
    /*end here*/
    currentOpcode = mmu->readByte(PC);      //getting the current opcode at RAM[PC] (aka: FETCHING ;-) )
    switch (currentOpcode)                  //finding what the current opcode is and do the appropriate function (aka: DECODING and EXECUTING ;-) )      
    {
        case 0xA9:
            LDA_IMM();
        break;

        default:
            std::cout << "Unknown Opcode : " << std::hex << currentOpcode << std::endl;
        break;
    }
    /* starting from here, its just for testing LDA_IMM() , must delete after understanding LDA_IMM()*/
    std::cout << std::hex << (int)A;
    /*end here*/
}

void Cpu6502::LDA_IMM()                  //loading A register with immediate addressing mode
{
    A = mmu->readByte(PC+1);            //A is loaded with RAM[PC+1]
    PC += 2;                            //PC is incremented by 2 for next instruction
    cycles += 2;                        //instruction take 2 cycles
}