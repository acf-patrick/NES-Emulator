#include "nes.h"
#include <SDL2/SDL_ttf.h>

SDL_Window*     Nes::window   = nullptr;
SDL_Renderer*   Nes::renderer = nullptr;

Nes::Nes()
{
// hardware
    mmu = new Mmu();
    cpu = new Cpu6502(mmu);

// GUI
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    int width(2*WIDTH);
    SDL_CreateWindowAndRenderer(width, HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
    SDL_SetWindowTitle(window, "NES Emulator");
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    TTF_Init();

    running = true;
    debugger = new Debugger(cpu);
}

Nes::~Nes()
{
    Text::destroyFont();

    delete debugger;
    delete mmu;
    delete cpu;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void Nes::run()
{
    static bool once = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
            else if (event.type == SDL_KEYDOWN)
                keys[event.key.keysym.scancode] = true;
            else if (event.type == SDL_KEYUP)
                keys[event.key.keysym.scancode] = false;
        }

        if (once)
        {
            once = false;
            cpu->step();
        }

        // SDL_RenderClear(renderer);
        drawGame();
        if (debugger)
            debugger->draw();
        SDL_RenderPresent(renderer);
    }
}

void Nes::drawGame()
{

}
