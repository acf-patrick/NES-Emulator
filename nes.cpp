#include "nes.h"

Nes::Nes()
{
// hardware
    mmu = new Mmu();
    cpu = new Cpu6502(mmu);

// GUI
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    running = true;
    debug   = true;
}

Nes::~Nes()
{
    delete mmu;
    delete cpu;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Nes::run()
{
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

        cpu->step();

        SDL_RenderClear(renderer);
        drawGame();
        if (debug)
            drawDebugger();
        SDL_RenderPresent(renderer);
    }
}

void Nes::drawGame()
{

}

void Nes::drawDebugger()
{

}