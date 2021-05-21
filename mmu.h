#pragma once

#include "defs.h"

class Mmu
{
    private:
        uint8_t *RAM;    
    public:
        Mmu();
        ~Mmu();
};