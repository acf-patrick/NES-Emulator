#include "nes.h"
#include "defs.h"
#include "debugger.h"
#include <sstream>
#include <vector>

SDL_Window* Debugger::window = nullptr;
SDL_Renderer* Debugger::renderer = nullptr;

Debugger::Debugger(Cpu6502* c) : cpu(*c)
{
    const int width = WIDTH, height = HEIGHT;
    window = SDL_CreateWindow("Debugger", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_MOUSE_FOCUS);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    const SDL_Point padding = { 15, 10 };
    const int interline = 7;
    Text* prev;

// registers
    div["register"] = new Box({
        5, 5, width/2-10, height/2-10
    });
    auto& reg = *div["register"];
    // setup
    reg.setLabel("Registers");
    reg.label->setColor(76, 204, 89);
    reg.background = { 13, 17, 23, 255 };
    reg.boxColor   = { 18, 92, 180, 255 };

    // a little bit tricky
    prev = reg.push("STATUS", " STATUS : ", padding);
    std::vector<std::string> bits = { "N", "V", "-", "B", "D", "I", "Z", "C" };
    for (auto& bit : bits)
        prev = reg.push(bit, bit+"   ", { prev->getRight(), padding.y });
    reg["-"]->setColor(255, 0, 0); // always red

    std::vector<std::string> keys = { "PC", "A", "X", "Y", "SP" };
    for (auto& key : keys)
        prev = reg.push(key, " "+key+" : ", { padding.x, prev->getDown()+interline });
    
// memory
    // create the box
    div["memory"] = new Box({
        width/2+5, 5, width/2-10, height/2-10
    });
    auto& mem = *div["memory"];
    // setup
    mem.setLabel("Memory");
    mem.label->setColor(76, 204, 89);
    mem.background = { 13, 17, 23, 255 };
    mem.boxColor   = { 18, 92, 180, 255 };

    prev = mem.push("$1ff", "$1ff", padding);
    for (Word x = 0x01FE; x >= 0x0100; --x)
    {
        std::stringstream ss;
        ss << '$' << std::hex << x;
        auto key = ss.str();
        ss << " : ";
        prev = mem.push(key, ss.str(), { padding.x, prev->getDown()+interline });
    }
}
Debugger::~Debugger()
{
    for (auto& pair : div)
        delete pair.second;
    div.clear();

    Text::destroyFont();

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}

void Debugger::update()
{
    // Status : red? it's off. green? it's on
    std::map<std::string, int> bind = {
        {"N", cpu.N}, {"V", cpu.V}, {"B", cpu.B},
        {"D", cpu.D}, {"I", cpu.I}, {"Z", cpu.Z},
        {"C", cpu.C}
    };
    SDL_Color color[2] = {
        { 255, 0, 0, 255 }, // OFF
        { 0, 255, 0, 255 } //ON
    };
    auto& reg = *div["register"];
    for (auto& pair : bind)
    {
        auto& c = color[cpu.S[pair.second]];
        reg[pair.first]->setColor(c.r, c.g, c.b, c.a);
    }

    // Set to empty then push content fetched from CPU
    reg["PC"]->text = ""; (*reg["PC"]) << " PC : $" << cpu.PC;
    reg["SP"]->text = ""; (*reg["SP"]) << " SP : $" << cpu.SP;
    reg["A"]->text  = ""; (*reg["A"])  << "  A : $"  << cpu.A;
    reg["X"]->text  = ""; (*reg["X"])  << "  X : $"  << cpu.X;
    reg["Y"]->text  = ""; (*reg["Y"])  << "  Y : $"  << cpu.Y;

    // memory
    auto& mem = *div["memory"];
    for (Word address = 0x01FF; address >= 0x0100; --address)
    {
        int value = cpu.mmu->readByte(address); // ts mety mpoitra ref atao Byte ts haiko hoe manin
        std::stringstream ss;
        ss << '$' << std::hex << address;
        auto key = ss.str();
        mem[key]->text = "";
        ss.str("");
        ss << std::hex << "$" << address << " : " << value << " [" << std::dec << value << "] ";
        (*mem[key]) << ss.str();
    }
}

void Debugger::handle(SDL_Event& event)
{  
    const int diff = 7;

    // if the focus is on the debugger window
    if (event.window.windowID == SDL_GetWindowID(window))
        if (event.type == SDL_MOUSEWHEEL)
            for (auto& pair : div)
            {
                auto& box = *pair.second;
                SDL_Point mouse;
                SDL_GetMouseState(&mouse.x, &mouse.y);
                if (SDL_PointInRect(&mouse, &box.viewport))
                    box.slide((event.wheel.y>0) ? -diff:diff);
            }
}

void Debugger::draw()
{
    update();

// clean background
    SDL_Color bg = { 9, 12, 16, 255 };
    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_RenderClear(renderer);

// time to show what we got
    for (auto& pair : div)
        pair.second->draw();

    SDL_RenderPresent(renderer);
}