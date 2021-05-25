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
        case 0xA9:   LDA_IMM();    break;
        case 0xA5:   LDA_ZP();     break;
        case 0xB5:   LDA_ZPX();    break;
        case 0xAD:   LDA_ABS();    break;
        case 0xBD:   LDA_ABSX();   break;
        case 0xB9:   LDA_ABSY();   break;
        case 0xA1:   LDA_IX();     break;
        case 0xB1:   LDA_IY();     break;

        //LDX Instructions
        case 0xA2:   LDX_IMM();    break;
        case 0xA6:   LDX_ZP();     break;
        case 0xB6:   LDX_ZPY();    break;
        case 0xAE:   LDX_ABS();    break;
        case 0xBE:   LDX_ABSY();   break;

        //LDY Instructions
        case 0xA0:   LDY_IMM();    break;
        case 0xA4:   LDY_ZP();     break;
        case 0xB4:   LDY_ZPX();    break;
        case 0xAC:   LDY_ABS();    break;
        case 0xBC:   LDY_ABSX();   break;

        //STA Instructions
        case 0x85:   STA_ZP();     break;
        case 0x95:   STA_ZPX();    break;
        case 0x8D:   STA_ABS();    break;
        case 0x9D:   STA_ABSX();   break;
        case 0x99:   STA_ABSY();   break;
        case 0x81:   STA_IX();     break;
        case 0x91:   STA_IY();     break;

        //STX Instructions
        case 0x86:   STX_ZP();    break;
        case 0x96:   STX_ZPY();   break;
        case 0x8E:   STX_ABS();   break;

        //STY Instructions
        case 0x84:   STY_ZP();    break;
        case 0x94:   STY_ZPX();   break;
        case 0x8C:   STY_ABS();   break;


        /*******************JUMP AND CALLS INSTRUCTIONS*******************/
        //JSR Instruction
        case 0x20:   JSR();   break;
        
        //RTS Instruction
        case 0x60:   RTS();    break;

        //JMP Instruction
        case 0x4C:   JMP_ABS();   break;
        case 0x6C:   JMP_I();     break;

        /********************REGISTER TRANSFERS Operations****************/
        //TAX Instruction
        case 0xAA:   TAX();   break;
        //TAY Instruction
        case 0xA8:   TAY();   break;
        //TXA Instruction
        case 0x8A:   TXA();   break;
        //TYA Instruction
        case 0x98:   TYA();   break;

        /**************************STACK Operations***********************/
        //TSX Instruction
        case 0xBA:   TSX();   break;
        //TXS Instruction
        case 0x9A:   TXS();   break;
        //PHA Instruction
        case 0x48:   PHA();   break;
        //PHP Instruction
        case 0x08:   PHP();   break;
        //PLA Instruction
        case 0x68:   PLA();   break;
        //PLP Instruction
        case 0x28:   PLP();   break;
        
        /**************************LOGICAL Operations***********************/
        //AND Instructions
        case 0x29:   AND_IMM();  break;
        case 0x25:   AND_ZP();   break;
        case 0x35:   AND_ZPX();  break;
        case 0x2D:   AND_ABS();  break;
        case 0x3D:   AND_ABSX(); break;
        case 0x39:   AND_ABSY(); break;
        case 0x21:   AND_IX();   break;
        case 0x31:   AND_IY();   break;

        //EOR INstructions
        case 0x49:   EOR_IMM();   break;
        case 0x45:   EOR_ZP();    break;
        case 0x55:   EOR_ZPX();   break;
        case 0x4D:   EOR_ABS();   break;
        case 0x5D:   EOR_ABSX();  break;
        case 0x59:   EOR_ABSY();  break;
        case 0x41:   EOR_IX();    break;
        case 0x51:   EOR_IY();    break;

        //ORA INstructions
        case 0x09:   ORA_IMM();   break;
        case 0x05:   ORA_ZP();    break;
        case 0x15:   ORA_ZPX();   break;
        case 0x0D:   ORA_ABS();   break;
        case 0x1D:   ORA_ABSX();  break;
        case 0x19:   ORA_ABSY();  break;
        case 0x01:   ORA_IX();    break;
        case 0x11:   ORA_IY();    break;

        //BIT Instructions
        case 0x24:   BIT_ZP();    break;
        case 0x2C:   BIT_ABS();   break;

        /**************************ARITHMETICS Operations***********************/
        //ADC Instructions
        case 0x69:   ADC_IMM();   break;
        case 0x65:   ADC_ZP();    break;
        case 0x75:   ADC_ZPX();   break;
        case 0x6D:   ADC_ABS();   break;
        case 0x7D:   ADC_ABSX();  break;
        case 0x79:   ADC_ABSY();  break;
        case 0x61:   ADC_IX();    break;
        case 0x71:   ADC_IY();    break;

        //SBC Instructions
        case 0xE9:   SBC_IMM();   break;
        case 0xE5:   SBC_ZP();    break;
        case 0xF5:   SBC_ZPX();   break;
        case 0xED:   SBC_ABS();   break;
        case 0xFD:   SBC_ABSX();  break;
        case 0xF9:   SBC_ABSY();  break;
        case 0xE1:   SBC_IX();    break;
        case 0xF1:   SBC_IY();    break;

        //CMP Instruction
        case 0xC9:   CMP_IMM();   break;
        case 0xC5:   CMP_ZP();    break;
        case 0xD5:   CMP_ZPX();   break;
        case 0xCD:   CMP_ABS();   break;
        case 0xDD:   CMP_ABSX();  break;
        case 0xD9:   CMP_ABSY();  break;
        case 0xC1:   CMP_IX();    break;
        case 0xD1:   CMP_IY();    break;

        //CPX Instructions
        case 0xE0:   CPX_IMM();   break;
        case 0xE4:   CPX_ZP();    break;
        case 0xEC:   CPX_ABS();   break;

        //CPY Instructions
        case 0xC0:   CPY_IMM();   break;
        case 0xC4:   CPY_ZP();    break;
        case 0xCC:   CPY_ABS();   break;

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
