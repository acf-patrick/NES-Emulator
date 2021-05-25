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

    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
    SDL_SetWindowTitle(window, "NES Emulator");

    TTF_Init();

    running = true;
    debugger = new Debugger(cpu, step); // commenting this line will remove the debugger
}

Nes::~Nes()
{
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
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_WINDOWEVENT)
            {
                // quit only when X button of the main window i.e the Nes-Emulator window
                // has been hit
                if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                    if (SDL_GetWindowID(window) == event.window.windowID)
                        running = false;
                    else // must be the debugger window
                    {
                        delete debugger;
                        debugger = nullptr;
                    }
            }
            else if (event.type == SDL_KEYDOWN)
                keys[event.key.keysym.scancode] = true;
            else if (event.type == SDL_KEYUP)
                keys[event.key.keysym.scancode] = false;
                
            // check for event
            if (debugger)
                debugger->handle(event);
        }

        if (step)
        {
            if (step == 1)
                step = 0;
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
