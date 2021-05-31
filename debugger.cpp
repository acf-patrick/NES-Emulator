#include "nes.h"
#include "defs.h"
#include "debugger.h"
#include <sstream>
#include <vector>

SDL_Window* Debugger::window = nullptr;
SDL_Renderer* Debugger::renderer = nullptr;

Debugger::Debugger(Cpu6502* c, int& s) : cpu(*c), step(s)
{
    const int width = WIDTH+10, height = HEIGHT;
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
        5, 5, width/2+5, height/4 + 10
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
        5, (reg.viewport.y+reg.viewport.h)+5, reg.viewport.w, height/4-10
    });
    auto& inst = *div["instruction"];
    inst.setLabel("instructions");
    inst.push("first", " ", padding );   // dunno what to put so we'll just show the currentOpc
    
// memory
    // create the box
    div["memory"] = new Box({
        (reg.viewport.w+reg.viewport.x)+5, 5, width/2-20, height/2+19
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

    for (auto& pair : div)
    {
        auto& box = *pair.second;
        // The mouse cursor is inside the box
        if (SDL_PointInRect(&mouse, &box.viewport))
        {
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_UP)
                    box.slide(diff);
                else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                    box.slide(-diff);
            }
            if (event.type == SDL_MOUSEWHEEL)
                box.slide((event.wheel.y>0) ? -diff:diff);
        }
    }
    
    for (auto& pair : buttons)
    {
        auto& button = *pair.second;
        button.state = "idle";
        if (button.contains(mouse))
        {
            if (event.type == SDL_MOUSEMOTION)
                button.state = "hover";
            if (event.type == SDL_MOUSEBUTTONDOWN)
                button.state = "pressed";
            if (event.type == SDL_MOUSEBUTTONUP)
            {
                if (pair.first == "step")
                    step = 1;
                else if (pair.first == "stop")
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