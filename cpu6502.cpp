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
    Instruction illegal = Instruction{ "Unknown", "", [](){} };
    instructions = {        // we then initialize the list
        Instruction{ "BRK", "IMP", [this](){ BRK(); } }, Instruction{ "ORA", "I-X", [this](){ ORA_IX(); } }, illegal, illegal, illegal, Instruction{ "ORA", "ZPG", [this](){ ORA_ZP(); } }, Instruction{ "ASL", "ZPG", [this](){ ASL_ZP(); } }, illegal, Instruction{ "PHP", "IMP", [this](){ PHP(); } }, Instruction{ "ORA", "IMM", [this](){ ORA_IMM(); } }, Instruction{ "ASL", "A", [this](){ ASL(); } }, illegal, illegal, Instruction{ "ORA", "ABS", [this](){ ORA_ABS(); } }, Instruction{ "ASL", "ABS", [this](){ ASL_ABS(); } }, illegal,
        Instruction{ "BPL", "REL", [this](){ BPL(); } },  Instruction{ "ORA", "I-Y", [this](){ ORA_IY(); } }, illegal, illegal, illegal, Instruction{ "ORA", "ZPX", [this](){ ORA_ZPX(); } }, Instruction{ "ASL", "ZPX", [this](){ ASL_ZPX(); } }, illegal, Instruction{ "CLC", "IMP", [this](){ CLC(); } }, Instruction{ "ORA", "ABY", [this](){ ORA_ABSY(); } }, illegal, illegal, illegal, Instruction{ "ORA", "ABX", [this](){ ORA_ABSX(); } }, Instruction{ "ASL", "ABX", [this](){ ASL_ABSX(); } }, illegal,
        Instruction{ "JSR", "ABS", [this](){ JSR(); } },  Instruction{ "AND", "I-X", [this](){ AND_IX(); } }, illegal, illegal, Instruction{ "BIT", "ZPG", [this](){ BIT_ZP(); } }, Instruction{ "AND", "ZPG", [this](){ AND_ZP(); } }, Instruction{ "ROL", "ZPG", [this](){ ROL_ZP(); } }, illegal, Instruction{ "PLP", "IMP", [this](){ PLP(); } }, Instruction{ "AND", "IMM", [this](){ AND_IMM(); } }, Instruction{ "ROL", "A", [this](){ ROL(); } }, illegal, Instruction{ "BIT", "ABS", [this](){ BIT_ABS(); } }, Instruction{ "AND", "ABS", [this](){ AND_ABS(); } }, Instruction{ "ROL", "ABS", [this](){ ROL_ABS(); } }, illegal,
        Instruction{ "BMI", "REL", [this](){ BMI(); } },  Instruction{ "AND", "I-Y", [this](){ AND_IY(); } }, illegal, illegal, illegal, Instruction{ "AND", "ZPX", [this](){ AND_ZPX(); } }, Instruction{ "ROL", "ZPX", [this](){ ROL_ZPX(); } }, illegal, Instruction{ "SEC", "IMP", [this](){ SEC(); } }, Instruction{ "AND", "ABY", [this](){ AND_ABSY(); } }, illegal, illegal, illegal, Instruction{ "AND", "ABX", [this](){ AND_ABSX(); } }, Instruction{ "ROL", "ABX", [this](){ ROL_ABSX(); } }, illegal,
        Instruction{ "RTI", "IMP", [this](){ RTI(); } }, Instruction{ "EOR", "I-X", [this](){ EOR_IX(); } }, illegal, illegal, illegal, Instruction{ "EOR", "ZPG", [this](){ EOR_ZP(); } }, Instruction{ "LSR", "ZPG", [this](){ LSR_ZP(); } }, illegal, Instruction{ "PHA", "IMP", [this](){ PHA(); } }, Instruction{ "EOR", "IMM", [this](){ EOR_IMM(); } }, Instruction{ "LSR", "A", [this](){ LSR(); } }, illegal, Instruction{ "JMP", "ABS", [this](){ JMP_ABS(); } }, Instruction{ "EOR", "ABS", [this](){ EOR_ABS(); } }, Instruction{ "LSR", "ABS", [this](){ LSR_ABS(); } }, illegal,
        Instruction{ "BVC", "REL", [this](){ BVC(); } },  Instruction{ "EOR", "I-Y", [this](){ EOR_IY(); } }, illegal, illegal, illegal, Instruction{ "EOR", "ZPX", [this](){ EOR_ZPX(); } }, Instruction{ "LSR", "ZPX", [this](){ LSR_ZPX(); } }, illegal, Instruction{ "CLI", "IMP", [this](){ CLI(); } }, Instruction{ "EOR", "ABY", [this](){ EOR_ABSY(); } }, illegal, illegal, illegal, Instruction{ "EOR", "ABX", [this](){ EOR_ABSX(); } }, Instruction{ "LSR", "ABX", [this](){ LSR_ABSX(); } }, illegal,
        Instruction{ "RTS", "IMP", [this](){ RTS(); } }, Instruction{ "ADC", "I-X", [this](){ ADC_IX(); } }, illegal, illegal, illegal, Instruction{ "ADC", "ZPG", [this](){ ADC_ZP(); } }, Instruction{ "ROR", "ZPG", [this](){ ROR_ZP(); } }, illegal, Instruction{ "PLA", "IMP", [this](){ PLA(); } }, Instruction{ "ADC", "IMM", [this](){ ADC_IMM(); } }, Instruction{ "ROR", "A", [this](){ ROR(); } }, illegal, Instruction{ "JMP", "ind", [this](){ JMP_I(); } }, Instruction{ "ADC", "ABS", [this](){ ADC_ABS(); } }, Instruction{ "ROR", "ABS", [this](){ ROR_ABS(); } }, illegal,
        Instruction{ "BVS", "REL", [this](){ BVS(); } },  Instruction{ "ADC", "I-Y", [this](){ ADC_IY(); } }, illegal, illegal, illegal, Instruction{ "ADC", "ZPX", [this](){ ADC_ZPX(); } }, Instruction{ "ROR", "ZPX", [this](){ ROR_ZPX(); } }, illegal, Instruction{ "SEI", "IMP", [this](){ SEI(); } }, Instruction{ "ADC", "ABY", [this](){ ADC_ABSY(); } }, illegal, illegal, illegal, Instruction{ "ADC", "ABS", [this](){ ADC_ABS(); } }, Instruction{ "ROR", "ABX", [this](){ ROR_ABSX(); } }, illegal,
        illegal, Instruction{ "STA", "I-X", [this](){ STA_IX(); } }, illegal, illegal, Instruction{ "STY", "ZPG", [this](){ STY_ZP(); } }, Instruction{ "STA", "ZPG", [this](){ STA_ZP(); } }, Instruction{ "STX", "ZPG", [this](){ STX_ZP(); } }, illegal, Instruction{ "DEY", "IMP", [this](){ DEY(); } }, illegal, Instruction{ "TXA", "IMP", [this](){ TXA(); } }, illegal, Instruction{ "STY", "ABS", [this](){ STY_ABS(); } }, Instruction{ "STA", "ABS", [this](){ STA_ABS(); } }, Instruction{ "STX", "ABS", [this](){ STX_ABS(); } }, illegal,
        Instruction{ "BCC", "REL", [this](){ BCC(); } }, Instruction{ "STA", "I-Y", [this](){ STA_IY(); } }, illegal, illegal, Instruction{ "STY", "ZPX", [this](){ STY_ZPX(); } }, Instruction{ "STA", "ZPX", [this](){ STA_ZPX(); } }, Instruction{ "STX", "ZPY", [this](){ STX_ZPY(); } }, illegal, Instruction{ "TYA", "IMP", [this](){ TYA(); } }, Instruction{ "STA", "ABY", [this](){ STA_ABSY(); } }, Instruction{ "TXS", "IMP", [this](){ TXS(); } }, illegal, illegal, Instruction{ "STA", "ABX", [this](){ STA_ABSX(); } }, illegal, illegal,
        Instruction{ "LDY", "immeditate", [this](){ LDY_IMM(); } }, Instruction{ "LDA", "I-X", [this](){ LDA_IX(); } }, Instruction{ "LDX", "IMM", [this](){ LDX_IMM(); } }, illegal, Instruction{ "LDY", "ZPG", [this](){ LDY_ZP(); } }, Instruction{ "LDA", "ZPG", [this](){ LDA_ZP(); } }, Instruction{ "LDX", "ZPG", [this](){ LDX_ZP(); } }, illegal, Instruction{ "TAY", "IMP", [this](){ TAY(); } }, Instruction{ "LDA", "IMM", [this](){ LDA_IMM(); } }, Instruction{ "TAX", "IMP", [this](){ TAX(); } }, illegal, Instruction{ "LDY", "ABS", [this](){ LDY_ABS(); } }, Instruction{ "LDA", "ABS", [this](){ LDA_ABS(); } }, Instruction{ "LDX", "ABS", [this](){ LDX_ABS(); } }, illegal,
        Instruction{ "BCS", "REL", [this](){ BCS(); } }, Instruction{ "LDA", "I-Y", [this](){ LDA_IY(); } }, illegal, illegal, Instruction{ "LDY", "ZPX", [this](){ LDY_ZPX(); } }, Instruction{ "LDA", "ZPX", [this](){ LDA_ZPX(); } }, Instruction{ "LDX", "ZPY", [this](){ LDX_ZPY(); } }, illegal, Instruction{ "CLV", "IMP", [this](){ CLV(); } }, Instruction{ "LDA", "ABY", [this](){ LDA_ABSY(); } }, Instruction{ "TSX", "IMP", [this](){ TSX(); } }, illegal, Instruction{ "LDY", "ABX", [this](){ LDY_ABSX(); } }, Instruction{ "LDA", "ABX", [this](){ LDA_ABSX(); } }, Instruction{ "LDX", "ABY", [this](){ LDX_ABSY(); } }, illegal,
        Instruction{ "CPY", "IMM", [this](){ CPY_IMM(); } }, Instruction{ "CMP", "I-X", [this](){ CMP_IX(); } }, illegal, illegal, Instruction{ "CPY", "ZPG", [this](){ CPY_ZP(); } }, Instruction{ "CMP", "ZPG", [this](){ CMP_ZP(); } }, Instruction{ "DEC", "ZPG", [this](){ DEC_ZP(); } }, illegal, Instruction{ "INY", "IMP", [this](){ INY(); } }, Instruction{ "CMP", "IMM", [this](){ CMP_IMM(); } }, Instruction{ "DEX", "IMP", [this](){ DEX(); } }, illegal, Instruction{ "CPY", "ABS", [this](){ CPY_ABS(); } }, Instruction{ "CMP", "ABS", [this](){ CMP_ABS(); } }, Instruction{ "DEC", "ABS", [this](){ DEC_ABS(); } }, illegal,
        Instruction{ "BNE", "REL", [this](){ BNE(); } }, Instruction{ "CMP", "I-Y", [this](){ CMP_IY(); } }, illegal, illegal, illegal, Instruction{ "CMP", "ZPX", [this](){ CMP_ZPX(); } }, Instruction{ "DEC", "ZPX", [this](){ DEC_ZPX(); } }, illegal, Instruction{ "CLD", "IMP", [this](){ CLD(); } }, Instruction{ "CMP", "ABY", [this](){ CMP_ABSY(); } }, illegal, illegal, illegal, Instruction{ "CMP", "ABX", [this](){ CMP_ABSX(); } }, Instruction{ "DEC", "ABX", [this](){ DEC_ABSX(); } }, illegal,
        Instruction{ "CPX", "IMM", [this](){ CPX_IMM(); } }, Instruction{ "SBC", "I-X", [this](){ SBC_IX(); } }, illegal, illegal, Instruction{ "CPX", "ZPG", [this](){ CPX_ZP(); } }, Instruction{ "SBC", "ZPG", [this](){ SBC_ZP(); } }, Instruction{ "INC", "ZPG", [this](){ INC_ZP(); } }, illegal, Instruction{ "INX", "IMP", [this](){ INX(); } }, Instruction{ "SBC", "IMM", [this](){ SBC_IMM(); } }, Instruction{ "NOP", "IMP", [this](){ NOP(); } }, illegal, Instruction{ "CPX", "ABS", [this](){ CPX_ABS(); } }, Instruction{ "SBC", "ABS", [this](){ SBC_ABS(); } }, Instruction{ "INC", "ABS", [this](){ INC_ABS(); } }, illegal,
        Instruction{ "BEQ", "REL", [this](){ BEQ(); } }, Instruction{ "SBC", "I-Y", [this](){ SBC_IY(); } }, illegal, illegal, illegal, Instruction{ "SBC", "ZPX", [this](){ SBC_ZPX(); } }, Instruction{ "INC", "ZPX", [this](){ INC_ZPX(); } }, illegal, Instruction{ "SED", "IMP", [this](){ SED(); } }, Instruction{ "SBC", "ABY", [this](){ SBC_ABSY(); } }, illegal, illegal, illegal, Instruction{ "SBC", "ABX", [this](){ SBC_ABSX(); } }, Instruction{ "INC", "ABX", [this](){ INC_ABSX(); } }, illegal
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