#include "cpu6502.h"

Cpu6502::Cpu6502(Mmu *_mmu)
{
    A = X = Y = 0;
    PC = 0xFFFC;    //program usually start at 0xFFFC 
    SP = 0xFF;      //Satck grow bottom top so 0x1FF to 0x100 (in the first page memory)
    S = 0;
    mmu = _mmu;
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
    std::cout << "Yes, this cpu is running fine...";
}