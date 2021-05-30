#include "mmu.h"

Mmu::Mmu()
{
    RAM = new Byte[0xFFFF];             //max memory size is 0xFFFF because the NES have a 16bit cpu?  
    for (int i=0; i<0xFFFF; i++)
        RAM[i] = 0;
    loadCartridge("./Excitebike.nes");  //loading the cartridge here for now, may add file chooser later
}

Mmu::~Mmu()
{
    delete(RAM);
}

Byte Mmu::readByte(Word address)
{
    return RAM[address];                     //just getting the value at RAM[address]
}

void Mmu::writeByte(Byte value, Word address)            //just write value to RAM[address]
{
    RAM[address] = value;
}

Word Mmu::readWord(Word address)
{
    Word value = (Word)RAM[address+1];         //getting value at RAM[address+1] (little andian)    
    value <<= 8;
    return (value |= RAM[address]);   //shifting the value by 8bit then OR it with the value at RAM[address]
}

void Mmu::loadCartridge(const std::string filename)
{
    /*
        For mapper 0, the cartridge is structured as follow:
        - 16 bytes of informations about the cartridge (0x0 - 0xFF) //check docs for all the info, needed for cartridge info later
        - 512 bytes (usually not present on most cartridge, so may not exist)
        - 32 kbytes of PRG-ROM, contains the base game data:
            - may be two 16 kbytes of PRG-ROM, first part loaded in 0x8000 - 0xBFFF and second part loaded in 0xC000 - 0xFFFF
            - may be only one 16 kbytes part loaded in 0x8000 - 0xBFFF and mirrored in 0xC000 - 0xFFFF
        - 8 kbytes of CHR-ROM, contains the graphics data (may vary depending of cartridge, see docs)
    */

    std::ifstream Rom(filename, std::ios::binary | std::ios::ate);              //file to read
    std::ofstream RomOut("rom_out.bin");                                        //file dumped for debugging
	if (Rom.is_open())
	{
		int RomSize = Rom.tellg();                          //size of the rom
        std::cout << RomSize << std::endl;
        Rom.seekg(std::ios::beg);
		char *buff = new char[RomSize];
		Rom.seekg(0, std::ios::beg);
		Rom.read(buff, RomSize);                            //filling the buffer with contents of the cartridge
		for (int i = 0x8000; i < 0xC000 ; i++)              //this actullay works only with rom with mapper 0, must add some mapper in the end game of development stage
        {
			RAM[i] = buff[i - 0x8000 + 0x10];               //this rom actually have only one 16 kbytes of PRG-ROM so we laod it in 0x8000 - 0xBFFF, skipping first 16 bytes
            RAM[i+0x4000] = buff[i - 0x8000 + 0x10];        //mirros the PRG-ROM into 0xC000 - 0xFFFF (just loading it also there, may use OLC technics but let's keep things easy now)
        }
		Rom.close();

        buff = new char[0x8000];
        if (RomOut.is_open())                               //UNKNOWN REASON WHY THERE IS DIFFRENECES BETWEEN DTA LOADED IN RAM AND CARTRIDGE? MUST CHECK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        {
            for (int i=0x8000; i<0xFFFF; i++)
                buff[i - 0x8000] = RAM[i];
            RomOut.write(buff, 0x8000);
            RomOut.close();
        }
        else exit(EXIT_FAILURE);

	}
    else
    {
        std::cout << "...Can't load " << filename << "..."; 
    }
}