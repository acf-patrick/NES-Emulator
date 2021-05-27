#include "cpu6502.h"

Cpu6502::Cpu6502(Mmu *_mmu)
{
    A = X = Y = 0;
    PC = 0xC184;    //  program usually start at 0xFFFC 
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
        
        /*******************INCREMENTS/DECREMENTS Operations*********************/
        //INC Instructions
        case 0xE6:  INC_ZP();   break;
        case 0xF6:  INC_ZPX();  break;
        case 0xEE:  INC_ABS();  break;
        case 0xFE:  INC_ABSX(); break;

        //INX Instruction
        case 0xE8: INX(); break;

        //INY Instruction
        case 0xC8: INY(); break;

        //DEC Instructions
        case 0xC6:  DEC_ZP();   break;
        case 0xD6:  DEC_ZPX();  break;
        case 0xCE:  DEC_ABS();  break;
        case 0xDE:  DEC_ABSX(); break;

        //DEX Instruction
        case 0xCA: DEX(); break;

        //DEX Instruction
        case 0x88: DEY(); break;

        /***************************SHIFTS Operations***************************/
        //ASL Instructions
        case 0x0A:  ASL();      break;
        case 0x06:  ASL_ZP();   break;
        case 0x16:  ASL_ZPX();  break;
        case 0x0E:  ASL_ABS();  break;
        case 0x1E:  ASL_ABSX(); break;

        //LSR Instructions
        case 0x4A:  LSR();      break;
        case 0x46:  LSR_ZP();   break;
        case 0x56:  LSR_ZPX();  break;
        case 0x4E:  LSR_ABS();  break;
        case 0x5E:  LSR_ABSX(); break;

        //ROL Instructions
        case 0x2A:  ROL();      break;
        case 0x26:  ROL_ZP();   break;
        case 0x36:  ROL_ZPX();  break;
        case 0x2E:  ROL_ABS();  break;
        case 0x3E:  ROL_ABSX(); break;

        //ROR Instructions
        case 0x6A:  ROR();      break;
        case 0x66:  ROR_ZP();   break;
        case 0x76:  ROR_ZPX();  break;
        case 0x6E:  ROR_ABS();  break;
        case 0x7E:  ROR_ABSX(); break;

        /***********************SYSTEM FUNCTIONS Operations**********************/
        //BRK Instruction
        case 0x00:  BRK();  break;

        //NOP Instruction
        case 0xEA:  NOP();  break;

        //RTI Instruction
        case 0x40:  RTI();  break;

        /**************************BRANCHES Operations*************************/
        //BCC Instruction
        case 0x90:  BCC();  break;

        //BCS Instruction
        case 0xB0:  BCS();  break;

        //BEQ Instruction
        case 0xF0:  BEQ();  break;

        //BNE Instruction
        case 0xD0:  BNE();  break;

        //BMI Instruction
        case 0x30:  BMI();  break;

        //BPL Instruction
        case 0x10:  BPL();  break;

        //BVC Instruction
        case 0x50:  BVC();  break;

        //BVS Instruction
        case 0x70:  BVS();  break;

        /************************STATUS FLAGS Operations************************/
        //CLC Instruction
        case 0x18:  CLC();  break;

        //CLD Instruction
        case 0xD8:  CLD();  break;

        //CLI Instruction
        case 0x58:  CLI();  break;

        //CLV Instruction
        case 0xB8:  CLV();  break;

        //SEC Instruction
        case 0x38:  SEC();  break;

        //SED Instruction
        case 0xF8:  SED();  break;

        //SEI Instruction
        case 0x78:  SEI();  break;

        default:
            std::cout << "Unknown Opcode = 0x" << std::hex << (int)currentOpcode << std::endl;
        break;
    }
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