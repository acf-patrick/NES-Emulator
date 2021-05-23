#include "nes.h"
#include "defs.h"
#include "debugger.h"
#include <sstream>
#include <vector>

Debugger::Debugger(Cpu6502* c) : cpu(*c)
{
    const SDL_Point padding = { 10, 10 };
    const int interline = 5;
    Text* prev;

// boxes
    viewport.emplace_back( WIDTH, 0, WIDTH/2, HEIGHT );
    viewport.emplace_back( int(WIDTH*3.0/2), 0, WIDTH/2, HEIGHT );

// registers
    std::vector<std::string> keys = { "PC", "A", "X", "Y", "SP" };

    // a little bit tricky
    prev = texts["STATUS"] = new Text(" STATUS : ", padding, viewport[0]);
    std::vector<std::string> bits = { "N", "V", "-", "B", "D", "I", "Z", "C" };
    for (auto& bit : bits)
        prev = texts[bit] = new Text(bit+" ", { prev->getRight(), padding.y }, viewport[0]);
    texts["-"]->setColor(255, 0, 0); // always red

    for (auto& key : keys)
        prev = texts[key] = new Text(" "+key+" : ", { padding.x, prev->getDown()+interline }, viewport[0]);
    
// memory
    texts["memory"] = new Text(" Memory ", { padding.x + 5, padding.y }, viewport[1]);
    prev = texts["$1ff"] = new Text("$1ff : ", { padding.x, texts["memory"]->getDown()+interline }, viewport[1]);
    for (Word x = 0x01FE; x >= 0x0100; --x)
    {
        std::stringstream ss;
        ss << '$' << std::hex << x;
        auto key = ss.str();
        ss << " : ";
        prev = texts[key] = new Text(ss.str(), { padding.x, prev->getDown()+interline }, viewport[1]);
    }

}
Debugger::~Debugger()
{
    for (auto& pair : texts)
        delete pair.second;
    texts.clear();
}

void Debugger::draw()
{
// update
    // registers
        // Status : red? it's off. green? it's on
        texts["N"]->setColor(255*(!cpu.S[cpu.N]), 255*cpu.S[cpu.N], 0);
        texts["V"]->setColor(255*(!cpu.S[cpu.V]), 255*cpu.S[cpu.V], 0);
        texts["B"]->setColor(255*(!cpu.S[cpu.B]), 255*cpu.S[cpu.B], 0);
        texts["D"]->setColor(255*(!cpu.S[cpu.D]), 255*cpu.S[cpu.D], 0);
        texts["I"]->setColor(255*(!cpu.S[cpu.I]), 255*cpu.S[cpu.I], 0);
        texts["Z"]->setColor(255*(!cpu.S[cpu.Z]), 255*cpu.S[cpu.Z], 0);
        texts["C"]->setColor(255*(!cpu.S[cpu.C]), 255*cpu.S[cpu.C], 0);

        // Set to empty then push content fetched from CPU
        texts["PC"]->text = ""; (*texts["PC"]) << " PC : $" << cpu.PC;
        texts["SP"]->text = ""; (*texts["SP"]) << " SP : $" << cpu.SP;
        texts["A"]->text = ""; (*texts["A"])  << " A : $" << cpu.A;
        texts["X"]->text = ""; (*texts["X"])  << " X : $" << cpu.X;
        texts["Y"]->text = ""; (*texts["Y"])  << " Y : $" << cpu.Y;

    // memory
    for (Word address = 0x01FF; address >= 0x0100; --address)
    {
        std::stringstream ss;
        ss << '$' << std::hex << address;
        auto key = ss.str();
        texts[key]->text = "";
        (*texts[key]) << "$" << address << " : " << cpu.mmu->readByte(address);
    }

// clean background
    SDL_Color bg = Text::getBackground();
    int window_w;
    SDL_GetWindowSize(Nes::window, &window_w, NULL);
    SDL_Rect field  = {
        WIDTH, 0, window_w - WIDTH, HEIGHT
    };
    SDL_SetRenderDrawColor(Nes::renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_RenderFillRect(Nes::renderer, &field);

    SDL_SetRenderDrawColor(Nes::renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(Nes::renderer, &viewport[0]);
    SDL_RenderDrawRect(Nes::renderer, &viewport[1]);

// time to show what we got
    for (auto& pair : texts)
        pair.second->draw();
}