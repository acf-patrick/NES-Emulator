#pragma once

#include "defs.h"
#include "mmu.h"
#include <functional>
#include <vector>

class Debugger;

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
    void PushStoSTack();    //push Satus flag register to stack
    void PopSFromStack();   //pop Satus flag register previous value from stack

    // type used to store instruction information
    struct Instruction
    {
        std::string mnemonic;
        std::string addressingMode;
        std::function<void(void)> opcode;
    };
    std::vector<Instruction> instructions;	// Our instructions, initialized inside the constructor
                                            // Use the instruction hex as index. e.g : ADC absolute  : 0x6D = 109, 6-th line, 13-th column
    
    Instruction& getCurrent();      // retrieve current instruction

    /***********************LOAD/STORE Operations*********************/
    
    //LDA Instructions
    void LDA_IMM();          //loading A register with immediate addressing mode
    void LDA_ZP();           //loading A register with zero page addressing mode
    void LDA_ZPX();          //loading A register with zero page + X addressing mode
    void LDA_ABS();          //loading A register with absolute addressing mode
    void LDA_ABSX();         //loading A register with absolute + x addressing mode
    void LDA_ABSY();         //loading A register with absolute + y addressing mode
    void LDA_IX();           //loading A register with (indirect + X) addressing mode
    void LDA_IY();           //loading A register with (indirect) + Y addressing mode

    //LDX Instructions
    void LDX_IMM();          //loading X register with immediate addressing mode
    void LDX_ZP();           //loading X register with zero page addressing mode
    void LDX_ZPY();          //loading X register with zero page + Y addressing mode
    void LDX_ABS();          //loading X register with absolute addressing mode
    void LDX_ABSY();         //loading X register with absolute + Y addressing mode

    //LDY Instructions
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


    /*****************************************************************/


    /**********************JUMP AND CALLS Operations******************/
    
    //JSR Instruction
    void JSR();              //Jump to subroutine

    //RTS Instruction
    void RTS();             //Return from subroutine

    //JMP Instruction
    void JMP_ABS();              //Jump to a specific location in memory in absolute addressing mode
    void JMP_I();                //Jump to a specific location in memory in indirect addressing mode

    /*****************************************************************/

    /********************REGISTER TRANSFERS Operations****************/


    //TAX Instruction
    void TAX();                 //transfers A into X

    //TAY Instruction
    void TAY();                 //transfers A into Y

    //TXA Instruction
    void TXA();                 //transfers X into A

    //TYA Instruction
    void TYA();                 //transfers Y into A

    /*****************************************************************/

    /**************************STACK Operations***********************/

    //TSX Instruction
    void TSX();                 //transfers SP into X

    //TXS Instruction
    void TXS();                 //transfers X into SP

    //PHA Instruction
    void PHA();                 //push A into SP

    //PHP Instruction
    void PHP();                 //push S into SP

    //PLA Instruction
    void PLA();                 //pull a byte from SP into A

    //PLP Instruction
    void PLP();                 //pull a byte from SP into S

    /*****************************************************************/

    /*************************LOGICAL Operations**********************/

    //AND Instructions
    void AND_IMM();             //AND A with immediate next byte
    void AND_ZP();              //AND A with next byte in zero page addressing mode
    void AND_ZPX();             //AND A with next byte in zero page + X addressing mode
    void AND_ABS();             //AND A with next byte in absolute addressing mode
    void AND_ABSX();            //AND A with next byte in absolute + X addressing mode
    void AND_ABSY();            //AND A with next byte in absolute + Y addressing mode
    void AND_IX();              //AND A with next byte in (indirect + X) addressing mode
    void AND_IY();              //AND A with next byte in (indirect) + Y addressing mode

    //EOR Instructions
    void EOR_IMM();             //EOR A with immediate next byte
    void EOR_ZP();              //EOR A with next byte in zero page addressing mode
    void EOR_ZPX();             //EOR A with next byte in zero page + X addressing mode
    void EOR_ABS();             //EOR A with next byte in absolute addressing mode
    void EOR_ABSX();            //EOR A with next byte in absolute + X addressing mode
    void EOR_ABSY();            //EOR A with next byte in absolute + Y addressing mode
    void EOR_IX();              //EOR A with next byte in (indirect + X) addressing mode
    void EOR_IY();              //EOR A with next byte in (indirect) + Y addressing mode

    //ORA Instructions
    void ORA_IMM();             //ORA A with immediate next byte
    void ORA_ZP();              //ORA A with next byte in zero page addressing mode
    void ORA_ZPX();             //ORA A with next byte in zero page + X addressing mode
    void ORA_ABS();             //ORA A with next byte in absolute addressing mode
    void ORA_ABSX();            //ORA A with next byte in absolute + X addressing mode
    void ORA_ABSY();            //ORA A with next byte in absolute + Y addressing mode
    void ORA_IX();              //ORA A with next byte in (indirect + X) addressing mode
    void ORA_IY();              //ORA A with next byte in (indirect) + Y addressing mode

    //BIT Instructions
    void BIT_ZP();              //Tests if bit 7 and 6 form memory is set/unset in zero page addresing mode
    void BIT_ABS();             //Tests if bit 7 and 6 form memory is set/unset in absolue addresing mode



    /*********************************************************************/

    /*************************ARITHMETIC Operations**********************/

    //ADC Instructions
    void ADC_IMM();             //Add A with carry and next byte in immediate mode
    void ADC_ZP();              //Add A with carry and next byte in zero page mode
    void ADC_ZPX();             //Add A with carry and next byte in zero page + X mode
    void ADC_ABS();             //Add A with carry and next byte in absolute mode
    void ADC_ABSX();            //Add A with carry and next byte in absolute + X mode
    void ADC_ABSY();            //Add A with carry and next byte in absolute + Y mode
    void ADC_IX();              //Add A with carry and next byte in (indirect + X) mode
    void ADC_IY();              //Add A with carry and next byte in (indirect) + Y mode

    //SBC Instructions
    void SBC_IMM();             //substract  next byte to A and carry in immediate mode
    void SBC_ZP();              //substract  next byte to A and carry in zero page mode
    void SBC_ZPX();             //substract  next byte to A and carry in zero page + X mode
    void SBC_ABS();             //substract  next byte to A and carry in absolute mode
    void SBC_ABSX();            //substract  next byte to A and carry in absolute + X mode
    void SBC_ABSY();            //substract  next byte to A and carry in absolute + Y mode
    void SBC_IX();              //substract  next byte to A and carry in (indirect + X) mode
    void SBC_IY();              //substract  next byte to A and carry in (indirect) + Y mode

    //CMP Instructions
    void CMP_IMM();             //Compare A with next 2 byte in immediate adressing mode
    void CMP_ZP();              //Compare A with next 2 byte in zero page adressing mode
    void CMP_ZPX();             //Compare A with next 2 byte in zero page + X adressing mode
    void CMP_ABS();             //Compare A with next 2 byte in absolute adressing mode
    void CMP_ABSX();            //Compare A with next 2 byte in absolute + X adressing mode
    void CMP_ABSY();            //Compare A with next 2 byte in absolute + Y adressing mode
    void CMP_IX();              //Compare A with next 2 byte in (indirect + X) adressing mode
    void CMP_IY();              //Compare A with next 2 byte in (indriect) = Y adressing mode

    //CPX Instructions
    void CPX_IMM();             //Compare X with next 2 byte in immediate adressing mode
    void CPX_ZP();              //Compare X with next 2 byte in zero page adressing mode
    void CPX_ABS();             //Compare X with next 2 byte in absolute adressing mode

    //CPY Instructions
    void CPY_IMM();             //Compare Y with next 2 byte in immediate adressing mode
    void CPY_ZP();              //Compare Y with next 2 byte in zero page adressing mode
    void CPY_ABS();             //Compare Y with next 2 byte in absolute adressing mode

    /***********************************************************************/

    /*******************INCREMENTS/DECREMENTS Operations*********************/

    //INC Instructions
    void INC_ZP();              //Increments specific value in zero page
    void INC_ZPX();             //Increments specific value in zero page + X
    void INC_ABS();             //Increments specific value in absolute addressing mode 
    void INC_ABSX();            //Increments specific value in absolute + X addressing mode

    //INX Instruction
    void INX();                 //Increments X

    //INY Instruction
    void INY();                 //Increments Y

    //DEC Instructions
    void DEC_ZP();              //Decrements specific value in zero page
    void DEC_ZPX();             //Decrements specific value in zero page + X
    void DEC_ABS();             //Decrements specific value in absolute addressing mode 
    void DEC_ABSX();            //Decrements specific value in absolute + X addressing mode

    //DEX Instruction
    void DEX();                 //Decrements X

    //DEY Instruction
    void DEY();                 //Decrements Y

    /***********************************************************************/

    /***************************SHIFTS Operations***************************/

    //ASL Instructions
    void ASL();                 //Arithmetic Shift Left with A
    void ASL_ZP();              //Arithmetic Shift Left in zero page addressing mode
    void ASL_ZPX();             //Arithmetic Shift Left in zero page + X addressing mode
    void ASL_ABS();             //Arithmetic Shift Left in absolute addressing mode
    void ASL_ABSX();            //Arithmetic Shift Left in absolute + X addressing mode

    //LSR Instructions
    void LSR();                 //Logical Shift Right with A
    void LSR_ZP();              //Logical Shift Right in zero page addressing mode
    void LSR_ZPX();             //Logical Shift Right in zero page + X addressing mode
    void LSR_ABS();             //Logical Shift Right in absolute addressing mode
    void LSR_ABSX();            //Logical Shift Right in absolute + X addressing mode

    //ROL Instructions
    void ROL();                 //Rotate Left with A
    void ROL_ZP();              //Rotate Left in zero page addressing mode
    void ROL_ZPX();             //Rotate Left in zero page + X addressing mode
    void ROL_ABS();             //Rotate Left in absolute addressing mode
    void ROL_ABSX();            //Rotate Left in absolute + X addressing mode

    //ROR Instructions
    void ROR();                 //Rotate Right with A
    void ROR_ZP();              //Rotate Right in zero page addressing mode
    void ROR_ZPX();             //Rotate Right in zero page + X addressing mode
    void ROR_ABS();             //Rotate Right in absolute addressing mode
    void ROR_ABSX();            //Rotate Right in absolute + X addressing mode

    /***********************************************************************/

    /***********************SYSTEM FUNCTIONS Operations**********************/

    //BRK Instruction
    void BRK();                 //Force an interrupt
    void NOP();                 //No Operation
    void RTI();                 //Return from Interrupt


    /***********************************************************************/

    /**************************BRANCHES Operations*************************/

    //BCC Instruction
    void BCC();                 //Branch if carry flag clear

    //BCS Instruction
    void BCS();                 //Branch if carry flag set

    //BEQ Instruction
    void BEQ();                 //Branch if zero flag set

    //BMI Instruction
    void BMI();                 //Branch if negative flag set

    //BNE Instruction
    void BNE();                 //Branch if zero flag clear

    //BPL Instruction
    void BPL();                 //Branch if negative flag clear

    //BVC Instruction
    void BVC();                 //Branch if overflow flag clear

    //BVS Instruction
    void BVS();                 //Branch if overflow flag set

    /***********************************************************************/

    /************************STATUS FLAGS Operations************************/

    //CLC Instruction
    void CLC();                 //Clear carry flag

    //CLD Instruction
    void CLD();                 //Clear decimal mode flag

    //CLC Instruction
    void CLI();                 //Clear interrupt disable flag

    //CLC Instruction
    void CLV();                 //Clear overflow flag

    //SEC Instruction
    void SEC();                 //Set carry flag

    //SED Instruction
    void SED();                 //Set decimal mode flag

    //SEI Instruction
    void SEI();                 //Set interrupt disable flag

    /***********************************************************************/

friend class Debugger;
};