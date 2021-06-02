#include "nes.h"
#include "defs.h"
#include "debugger.h"
#include <sstream>
#include <iomanip>
#include <vector>

SDL_Window* Debugger::window = nullptr;
SDL_Renderer* Debugger::renderer = nullptr;

Debugger::Debugger(Cpu6502* c, int& s) : 
    cpu(*c), step(s)
{
    const int width = WIDTH+45, height = HEIGHT-15;
    // window = Nes::window; // simply remove this line to switch to two windows option
    if (window == Nes::window)
    {
        SDL_SetWindowSize(window, WIDTH*2+10, HEIGHT);
        target = SDL_CreateRGBSurface(0, WIDTH+10, HEIGHT, 32, 0, 0, 0, 0xff);
        renderer = SDL_CreateSoftwareRenderer(target);
    } 
    else 
        SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN, &window, &renderer);

    const SDL_Point padding = { 5, 7 };
    const int interline = 7;
    Text* prev;

// registers
    div["register"] = new Box({
        5, 5, width/2+5, 130
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
        prev = reg.push(bit, bit+" ", { prev->getRight(), padding.y });
    reg["-"]->setColor(255, 0, 0); // always red

    std::vector<std::string> keys = { "PC", "A", "X", "Y", "SP" };
    for (auto& key : keys)
        prev = reg.push(key, " "+key+" : ", { padding.x, prev->getDown()+interline });

// instructions
    div["instruction"] = new Box({
        (reg.viewport.w+reg.viewport.x)+5, 5, width - (reg.viewport.w+10)-5, reg.viewport.h
    });
    auto& inst = *div["instruction"];
    inst.setLabel("instructions");
    inst.label->setColor(76, 204, 89);
    inst.background = { 13, 17, 23, 255 };
    inst.boxColor   = { 18, 92, 180, 255 };
    inst.push("first", " ", padding );   // dunno what to put so we'll just show the currentOpc

// buttons palette
    Button::colors = {
        { "idle"    , {  69,  69,  69, 255 } },
        { "hover"   , { 120, 120, 120, 255 } },
        { "pressed" , {  45,  45,  45, 255 } }
    };

    auto& v = inst.viewport;
    Button *b, *p;
    int y;

    b = buttons["step"] = new Button("step");
    y = v.y + v.h - (b->size.y+2*b->padding.y+5);
    b->position = { v.x + 5, y };
    p = b;

    b = buttons["continue"] = new Button("continue");
    b->position = { p->position.x+p->size.x+2*p->padding.x+5, y };
    p = b;

    b = buttons["stop"] = new Button("stop");
    b->position = { p->position.x+p->size.x+2*p->padding.x+5, y };

// memory
    div["memory"] = new MemoryViewer();

    std::cout << "debugger created\n";

}
Debugger::~Debugger()
{
    for (auto& pair : div)
        delete pair.second;
    div.clear();
    for (auto& pair : buttons)
        delete pair.second;
    buttons.clear();

    Text::destroyFont();

    SDL_FreeSurface(target);
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
        { 155, 0, 0, 255 }, // OFF
        { 192, 255, 91, 255 } //ON
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

    auto& i = *div["instruction"];
    auto& instruction = cpu.instructions[cpu.currentOpcode];
    i["first"]->text = ""; (*i["first"]) << " $" << cpu.currentOpcode << " : " << instruction.mnemonic << " {" << instruction.addressingMode << "} ";

    // memory
    auto& mem = *dynamic_cast<MemoryViewer*>(div["memory"]);
    for (Byte i=0; i<0x10; ++i)
        for (Byte j=0; j<0x10; ++j)
        {
            Word address = (mem.page << 8) | (i << 4) | j;
            auto& t = *mem.memory[i][j];
            t.text = ""; t << cpu.mmu->readByte(address) << " ";
        } 

}

void Debugger::handle(SDL_Event& event)
{  
    // work only if the focus is on the debugger
    if (event.window.windowID != SDL_GetWindowID(window))
        return;

    const int diff = 7;
/*
    if (event.type == SDL_MOUSEBUTTONUP)
        std::cout << event.button.x << ", " << event.button.y << std::endl;
*/
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);
    if (window == Nes::window)
        mouse.x -= WIDTH;

    for (auto& [name, box] : div)
    {
        // The mouse cursor is inside the box
        if (SDL_PointInRect(&mouse, &box->viewport))
        {
            if (event.type == SDL_KEYDOWN)
            {
                auto s = event.key.keysym.scancode;
                if (s == SDL_SCANCODE_UP)
                {
                    if (name == "memory")
                    {
                        auto& mem = *dynamic_cast<MemoryViewer*>(box);
                        int page = mem.page + 10;
                        mem.setPage(page%0x100);
                    }
                    else
                        box->slide(diff);
                }
                else if (s == SDL_SCANCODE_DOWN)
                {
                    if (name == "memory")
                    {
                        auto& mem = *dynamic_cast<MemoryViewer*>(box);
                        int page = mem.page - 10;
                        mem.setPage(page%0x100);
                    }
                    else
                        box->slide(-diff);
                }
            }
            if (event.type == SDL_MOUSEWHEEL)
            {
                int offset = (event.wheel.y>0)?-1:1;
                if (name != "memory")
                    box->slide(offset);
                else
                {
                    auto& mem = *dynamic_cast<MemoryViewer*>(box);
                    int page = mem.page - offset;
                    mem.setPage(page%0x100);
                }
            }
        }
    }
    
    for (auto& [name, button] : buttons)
    {
        button->state = "idle";
        if (button->contains(mouse))
        {
            if (event.type == SDL_MOUSEMOTION)
                button->state = "hover";
            if (event.type == SDL_MOUSEBUTTONDOWN)
                button->state = "pressed";
            if (event.type == SDL_MOUSEBUTTONUP)
            {
                if (name == "step")
                    step = 1;
                else if (name == "stop")
                    step = 0;
                else
                    step = -1;                
            }
        }
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
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);
    if (window == Nes::window)
        mouse.x -= WIDTH;

    for (auto& pair : div)
        pair.second->draw(mouse);
    for (auto& pair : buttons)
        pair.second->draw();

    if (window == Nes::window)
    {
        SDL_Rect dest = { WIDTH, 0, 0, 0 };
        SDL_BlitSurface(target, NULL, SDL_GetWindowSurface(window), &dest);
        SDL_UpdateWindowSurface(window);
    }

    SDL_RenderPresent(renderer);
}

MemoryViewer::MemoryViewer() :
    Box({ 5, 155, WIDTH+35, HEIGHT-190 })
{
    setPage(10);
    Text* prev = nullptr;
    std::stringstream ss;
    ss << std::hex << (int)page;
    std::string head = "$";
    if (page < 0x10)
        head.append(1, '0');
    head += ss.str();
    for (Byte i=0; i < 0x10; ++i)
    {
        ss.str("");
        if (!i) ss << "00"; else ss << (i<<4);
        auto h = head + ss.str();
        int y = 5+((prev==nullptr)?0:prev->getDown());
        prev = texts[h] = new Text(h + ": ", { 5, y }, viewport);
        for (Byte j=0; j < 0x10; ++j)
        {
            ss.str("");
            ss << (i<<4 | j);
            prev = memory[i][j] = new Text("00 ", { prev->getRight(), prev->getY() }, viewport);
        }
    } 
}

void MemoryViewer::setPage(Byte p)
{
    page = p;
    std::stringstream ss;
    ss << std::hex << (int)page;
    setLabel(ss.str() + " page"); // update header

    std::map<std::string, Text*> m;
    for (auto& [key, text] : texts)
    {
        std::string tmp = ss.str() + key.substr(3);
        if (page < 0x10)
            tmp = "0"+tmp;
        tmp = "$"+tmp;
        m[tmp] = text;
        text->setText(tmp+": ");
    }
    texts.clear();
    texts.merge(m);
}

void MemoryViewer::draw(const SDL_Point& mouse) 
{
    Box::draw(mouse);
    for (int i = 0; i <= 0xff; ++i)
        memory[i>>4][i&0xf]->draw();
}