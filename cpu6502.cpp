#include "cpu6502.h"

Cpu6502::Cpu6502(Mmu *_mmu)
{
    A = X = Y = 0;
    PC = 0xC184;    //  program usually start at 0xFFFC 
    SP = 0xFF;      //  Satck grow bottom top so 0x1FF to 0x100 (in the first page memory)
    S.reset();      //  set all bits to zero
    mmu = _mmu;
    cycles = 0;

    using c = Cpu6502;
    // used when the program attempt to use an illegal opcode
    Instruction illegal = Instruction{ "Unknown Opcode", "", [this](){ NONE(); } };
    instructions = {        // we then initialize the list
        Instruction{ "BRK", "impl", [this](){ BRK(); } }, Instruction{ "ORA", "X, ind", [this](){ ORA_IX(); } }, illegal, illegal, illegal, Instruction{ "ORA", "zpg", [this](){ ORA_ZP(); } }, Instruction{ "ASL", "zpg", [this](){ ASL_ZP(); } }, illegal, Instruction{ "PHP", "impl", [this](){ PHP(); } }, Instruction{ "ORA", "immediate", [this](){ ORA_IMM(); } }, Instruction{ "ASL", "A", [this](){ ASL(); } }, illegal, illegal, Instruction{ "ORA", "abs", [this](){ ORA_ABS(); } }, Instruction{ "ASL", "abs", [this](){ ASL_ABS(); } }, illegal,
        Instruction{ "BPL", "rel", [this](){ BPL(); } },  Instruction{ "ORA", "ind, Y", [this](){ ORA_IY(); } }, illegal, illegal, illegal, Instruction{ "ORA", "zpg, X", [this](){ ORA_ZPX(); } }, Instruction{ "ASL", "zpg, X", [this](){ ASL_ZPX(); } }, illegal, Instruction{ "CLC", "impl", [this](){ CLC(); } }, Instruction{ "ORA", "abs, Y", [this](){ ORA_ABSY(); } }, illegal, illegal, illegal, Instruction{ "ORA", "abs, X", [this](){ ORA_ABSX(); } }, Instruction{ "ASL", "abs, X", [this](){ ASL_ABSX(); } }, illegal,
        Instruction{ "JSR", "abs", [this](){ JSR(); } },  Instruction{ "AND", "X, ind", [this](){ AND_IX(); } }, illegal, illegal, Instruction{ "BIT", "zpg", [this](){ BIT_ZP(); } }, Instruction{ "AND", "zpg", [this](){ AND_ZP(); } }, Instruction{ "ROL", "zpg", [this](){ ROL_ZP(); } }, illegal, Instruction{ "PLP", "impl", [this](){ PLP(); } }, Instruction{ "AND", "immediate", [this](){ AND_IMM(); } }, Instruction{ "ROL", "A", [this](){ ROL(); } }, illegal, Instruction{ "BIT", "abs", [this](){ BIT_ABS(); } }, Instruction{ "AND", "abs", [this](){ AND_ABS(); } }, Instruction{ "ROL", "abs", [this](){ ROL_ABS(); } }, illegal,
        Instruction{ "BMI", "rel", [this](){ BMI(); } },  Instruction{ "AND", "ind, Y", [this](){ AND_IY(); } }, illegal, illegal, illegal, Instruction{ "AND", "zpg, X", [this](){ AND_ZPX(); } }, Instruction{ "ROL", "zpg, X", [this](){ ROL_ZPX(); } }, illegal, Instruction{ "SEC", "impl", [this](){ SEC(); } }, Instruction{ "AND", "abs, Y", [this](){ AND_ABSY(); } }, illegal, illegal, illegal, Instruction{ "AND", "abs, X", [this](){ AND_ABSX(); } }, Instruction{ "ROL", "abs, X", [this](){ ROL_ABSX(); } }, illegal,
        Instruction{ "RTI", "impl", [this](){ RTI(); } }, Instruction{ "EOR", "X, ind", [this](){ EOR_IX(); } }, illegal, illegal, illegal, Instruction{ "EOR", "zpg", [this](){ EOR_ZP(); } }, Instruction{ "LSR", "zpg", [this](){ LSR_ZP(); } }, illegal, Instruction{ "PHA", "impl", [this](){ PHA(); } }, Instruction{ "EOR", "immediate", [this](){ EOR_IMM(); } }, Instruction{ "LSR", "A", [this](){ LSR(); } }, illegal, Instruction{ "JMP", "abs", [this](){ JMP_ABS(); } }, Instruction{ "EOR", "abs", [this](){ EOR_ABS(); } }, Instruction{ "LSR", "abs", [this](){ LSR_ABS(); } }, illegal,
        Instruction{ "BVC", "rel", [this](){ BVC(); } },  Instruction{ "EOR", "ind, Y", [this](){ EOR_IY(); } }, illegal, illegal, illegal, Instruction{ "EOR", "zpg, X", [this](){ EOR_ZPX(); } }, Instruction{ "LSR", "zpg, X", [this](){ LSR_ZPX(); } }, illegal, Instruction{ "CLI", "impl", [this](){ CLI(); } }, Instruction{ "EOR", "abs, Y", [this](){ EOR_ABSY(); } }, illegal, illegal, illegal, Instruction{ "EOR", "abs, X", [this](){ EOR_ABSX(); } }, Instruction{ "LSR", "abs, X", [this](){ LSR_ABSX(); } }, illegal,
        Instruction{ "RTS", "impl", [this](){ RTS(); } }, Instruction{ "ADC", "X, ind", [this](){ ADC_IX(); } }, illegal, illegal, illegal, Instruction{ "ADC", "zpg", [this](){ ADC_ZP(); } }, Instruction{ "ROR", "zpg", [this](){ ROR_ZP(); } }, illegal, Instruction{ "PLA", "impl", [this](){ PLA(); } }, Instruction{ "ADC", "immediate", [this](){ ADC_IMM(); } }, Instruction{ "ROR", "A", [this](){ ROR(); } }, illegal, Instruction{ "JMP", "ind", [this](){ JMP_I(); } }, Instruction{ "ADC", "abs", [this](){ ADC_ABS(); } }, Instruction{ "ROR", "abs", [this](){ ROR_ABS(); } }, illegal,
        Instruction{ "BVS", "rel", [this](){ BVS(); } },  Instruction{ "ADC", "ind, Y", [this](){ ADC_IY(); } }, illegal, illegal, illegal, Instruction{ "ADC", "zpg, X", [this](){ ADC_ZPX(); } }, Instruction{ "ROR", "zpg, X", [this](){ ROR_ZPX(); } }, illegal, Instruction{ "SEI", "impl", [this](){ SEI(); } }, Instruction{ "ADC", "abs, Y", [this](){ ADC_ABSY(); } }, illegal, illegal, illegal, Instruction{ "ADC", "abs", [this](){ ADC_ABS(); } }, Instruction{ "ROR", "abs, X", [this](){ ROR_ABSX(); } }, illegal,
        illegal, Instruction{ "STA", "X, ind", [this](){ STA_IX(); } }, illegal, illegal, Instruction{ "STY", "zpg", [this](){ STY_ZP(); } }, Instruction{ "STA", "zpg", [this](){ STA_ZP(); } }, Instruction{ "STX", "zpg", [this](){ STX_ZP(); } }, illegal, Instruction{ "DEY", "impl", [this](){ DEY(); } }, illegal, Instruction{ "TXA", "impl", [this](){ TXA(); } }, illegal, Instruction{ "STY", "abs", [this](){ STY_ABS(); } }, Instruction{ "STX", "abs", [this](){ STX_ABS(); } }, illegal,
        Instruction{ "BCC", "rel", [this](){ BCC(); } }, Instruction{ "STA", "ind, Y", [this](){ STA_IY(); } }, illegal, illegal, Instruction{ "STY", "zpg, X", [this](){ STY_ZPX(); } }, Instruction{ "STA", "zpg, X", [this](){ STA_ZPX(); } }, Instruction{ "STX", "zpg, Y", [this](){ STX_ZPY(); } }, illegal, Instruction{ "TYA", "impl", [this](){ TYA(); } }, Instruction{ "STA", "abs, Y", [this](){ STA_ABSY(); } }, Instruction{ "TXS", "impl", [this](){ TXS(); } }, illegal, illegal, Instruction{ "STA", "abs, X", [this](){ STA_ABSX(); } }, illegal, illegal,
        Instruction{ "LDY", "immeditate", [this](){ LDY_IMM(); } }, Instruction{ "LDA", "X, ind", [this](){ LDA_IX(); } }, Instruction{ "LDX", "immediate", [this](){ LDX_IMM(); } }, illegal, Instruction{ "LDY", "zpg", [this](){ LDY_ZP(); } }, Instruction{ "LDA", "zpg", [this](){ LDA_ZP(); } }, Instruction{ "LDX", "zpg", [this](){ LDX_ZP(); } }, illegal, Instruction{ "TAY", "impl", [this](){ TAY(); } }, Instruction{ "LDA", "immediate", [this](){ LDA_IMM(); } }, Instruction{ "TAX", "impl", [this](){ TAX(); } }, illegal, Instruction{ "LDY", "abs", [this](){ LDY_ABS(); } }, Instruction{ "LDA", "abs", [this](){ LDA_ABS(); } }, Instruction{ "LDX", "abs", [this](){ LDX_ABS(); } }, illegal,
        Instruction{ "BCS", "rel", [this](){ BCS(); } }, Instruction{ "LDA", "ind, Y", [this](){ LDA_IY(); } }, illegal, illegal, Instruction{ "LDY", "zpg, X", [this](){ LDY_ZPX(); } }, Instruction{ "LDA", "zpg, X", [this](){ LDA_ZPX(); } }, Instruction{ "LDX", "zpg, Y", [this](){ LDX_ZPY(); } }, illegal, Instruction{ "CLV", "impl", [this](){ CLV(); } }, Instruction{ "LDA", "abs, Y", [this](){ LDA_ABSY(); } }, Instruction{ "TSX", "impl", [this](){ TSX(); } }, illegal, Instruction{ "LDY", "abs, X", [this](){ LDY_ABSX(); } }, Instruction{ "LDA", "abs, X", [this](){ LDA_ABSX(); } }, Instruction{ "LDX", "abs, Y", [this](){ LDX_ABSY(); } }, illegal,
        Instruction{ "CPY", "immediate", [this](){ CPY_IMM(); } }, Instruction{ "CMP", "X, ind", [this](){ CMP_IX(); } }, illegal, illegal, Instruction{ "CPY", "zpg", [this](){ CPY_ZP(); } }, Instruction{ "CMP", "zpg", [this](){ CMP_ZP(); } }, Instruction{ "DEC", "zpg", [this](){ DEC_ZP(); } }, illegal, Instruction{ "INY", "impl", [this](){ INY(); } }, Instruction{ "CMP", "immediate", [this](){ CMP_IMM(); } }, Instruction{ "DEX", "impl", [this](){ DEX(); } }, illegal, Instruction{ "CPY", "abs", [this](){ CPY_ABS(); } }, Instruction{ "CMP", "abs", [this](){ CMP_ABS(); } }, Instruction{ "DEC", "abs", [this](){ DEC_ABS(); } }, illegal,
        Instruction{ "BNE", "rel", [this](){ BNE(); } }, Instruction{ "CMP", "ind, Y", [this](){ CMP_IY(); } }, illegal, illegal, illegal, Instruction{ "CMP", "zpg, X", [this](){ CMP_ZPX(); } }, Instruction{ "DEC", "zpg, X", [this](){ DEC_ZPX(); } }, illegal, Instruction{ "CLD", "impl", [this](){ CLD(); } }, Instruction{ "CMP", "abs, Y", [this](){ CMP_ABSY(); } }, illegal, illegal, illegal, Instruction{ "CMP", "abs, X", [this](){ CMP_ABSX(); } }, Instruction{ "DEC", "abs, X", [this](){ DEC_ABSX(); } }, illegal,
        Instruction{ "CPX", "immediate", [this](){ CPX_IMM(); } }, Instruction{ "SBC", "X, ind", [this](){ SBC_IX(); } }, illegal, illegal, Instruction{ "CPX", "zpg", [this](){ CPX_ZP(); } }, Instruction{ "SBC", "zpg", [this](){ SBC_ZP(); } }, Instruction{ "INC", "zpg", [this](){ INC_ZP(); } }, illegal, Instruction{ "INX", "impl", [this](){ INX(); } }, Instruction{ "SBC", "immediate", [this](){ SBC_IMM(); } }, Instruction{ "NOP", "impl", [this](){ NOP(); } }, illegal, Instruction{ "CPX", "abs", [this](){ CPX_ABS(); } }, Instruction{ "SBC", "abs", [this](){ SBC_ABS(); } }, Instruction{ "INC", "abs", [this](){ INC_ABS(); } }, illegal,
        Instruction{ "BEQ", "rel", [this](){ BEQ(); } }, Instruction{ "SBC", "ind, Y", [this](){ SBC_IY(); } }, illegal, illegal, illegal, Instruction{ "SBC", "zpg, X", [this](){ SBC_ZPX(); } }, Instruction{ "INC", "zpg, X", [this](){ INC_ZPX(); } }, illegal, Instruction{ "SED", "impl", [this](){ SED(); } }, Instruction{ "SBC", "abs, Y", [this](){ SBC_ABSY(); } }, illegal, illegal, illegal, Instruction{ "SBC", "abs, X", [this](){ SBC_ABSX(); } }, Instruction{ "INC", "abs, X", [this](){ INC_ABSX(); } }, illegal
    };
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

    instructions[currentOpcode].opcode();   // execute the appropriate function  
    
#ifdef SHOW_CLI_DEBUGGER // Just add #define SHOW_CLI_DEBUGGER ontop of the file to enable
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
#endif
}

//Stack Useful Functions
void Cpu6502::PushPCtoStack()
{
    mmu->writeByte((PC + 3) & 0xFF, 0x0100 | SP);           //writing next instruction lower byte into first page at SP (0x0100 | SP)
    SP--;                                                   //decrementing SP because Stack grows downward
    mmu->writeByte((PC + 3) >> 8, 0x0100 | SP);             //writing next instruction lower byte into first page at SP (0x0100 | SP)
    SP--;                                                   //decrementing SP because Stack grows downward
}

void Cpu6502::PushStoSTack()
{
    mmu->writeByte(S.to_ullong(), 0x0100 | SP);
    SP --;       
}

void Cpu6502::PopPCfromStack()
{
    Word value = (mmu->readByte(0x0100 | (SP+1)) << 8) | mmu->readByte(0x0100 | (SP+2));      //getting 2 bytes from first page at SP in little endianess
    mmu->writeByte(0, 0x0100 | (SP+2));         //zero-ing previous value of previous stack
    mmu->writeByte(0, 0x0100 | (SP+1));
    SP += 2;                                    //incrementing SP by 2 because we're poping (Stack grows downard)
    PC = value;                                 //setting PC to previous instruction to be executed before jumping 
}

void Cpu6502::PopSFromStack()
{
    SP ++;
    S = mmu->readByte(0x0100 | SP);
    mmu->writeByte(0, 0x0100 | SP);
}