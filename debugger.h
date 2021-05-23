#pragma once

#include "text.h"
#include "cpu6502.h"
#include <map>
#include <vector>

class Debugger
{
private:
    std::vector<SDL_Rect> viewport;
    std::map<std::string, Text*> texts;
    Cpu6502& cpu;

public:
    Debugger(Cpu6502*);
    ~Debugger();

    void draw();
};