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

// boxes
    viewport.emplace_back( 0, 0, width/2, height/2 );
    viewport.emplace_back( width/2, 0, width/2, height/2 );

// registers
    std::vector<std::string> keys = { "PC", "A", "X", "Y", "SP" };

    // a little bit tricky
    prev = texts["STATUS"] = new Text(" STATUS : ", padding, viewport[0]);
    std::vector<std::string> bits = { "N", "V", "-", "B", "D", "I", "Z", "C" };
    for (auto& bit : bits)
        prev = texts[bit] = new Text(bit+"   ", { prev->getRight(), padding.y }, viewport[0]);
    texts["-"]->setColor(255, 0, 0); // always red

    for (auto& key : keys)
        prev = texts[key] = new Text(" "+key+" : ", { padding.x, prev->getDown()+interline }, viewport[0]);
    
// memory
    prev = texts["memory"] = new Text("       Memory       ", { padding.x, padding.y }, viewport[1]);  // header
    prev->setColor(255, 0, 255);

    // create the box
    sliders["memory"] = new SliderBox({ width/2 + 10, viewport[1].y+texts["memory"]->getDown(), width/2 - 20, height/2 - 40 });
    prev = sliders["memory"]->push("$1ff", "$1ff", padding);
    for (Word x = 0x01FE; x >= 0x0100; --x)
    {
        std::stringstream ss;
        ss << '$' << std::hex << x;
        auto key = ss.str();
        ss << " : ";
        prev = sliders["memory"]->push(key, ss.str(), { padding.x, prev->getDown()+interline });
    }

}
Debugger::~Debugger()
{
    for (auto& pair : sliders)
        delete pair.second;
    sliders.clear();

    for (auto& pair : texts)
        delete pair.second;
    texts.clear();

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
    for (auto& pair : bind)
    {
        auto& c = color[cpu.S[pair.second]];
        texts[pair.first]->setColor(c.r, c.g, c.b, c.a);
    }

    // Set to empty then push content fetched from CPU
    texts["PC"]->text = ""; (*texts["PC"]) << " PC : $" << cpu.PC;
    texts["SP"]->text = ""; (*texts["SP"]) << " SP : $" << cpu.SP;
    texts["A"]->text = ""; (*texts["A"])  << " A : $" << cpu.A;
    texts["X"]->text = ""; (*texts["X"])  << " X : $" << cpu.X;
    texts["Y"]->text = ""; (*texts["Y"])  << " Y : $" << cpu.Y;

    // memory
    auto& mem = *sliders["memory"];
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
            for (auto& pair : sliders)
            {
                auto slider = pair.second;
                SDL_Point mouse;
                SDL_GetMouseState(&mouse.x, &mouse.y);
                if (SDL_PointInRect(&mouse, &slider->viewport))
                    slider->slide((event.wheel.y>0) ? -diff:diff);
            }
}

void Debugger::draw()
{
    update();

// clean background
    SDL_Color bg = Text::getBackground();
    int window_w;
    SDL_GetWindowSize(window, &window_w, NULL);
    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_RenderClear(renderer);

// time to show what we got
    for (auto& pair : sliders)
        pair.second->draw();
    for (auto& pair : texts)
        pair.second->draw();

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (auto& v : viewport)
        SDL_RenderDrawRect(renderer, &v);

    SDL_RenderPresent(renderer);
}