
#include <SDL.h>
#include <iostream>
#include <sstream>
#include "core/logger.hpp"
#include "graphics/graphics.hpp"

int main()
{
    core::logger::init();
    core::logger::addOutput(&std::cout);
    graphics::Graphics* gfx = new graphics::Graphics;

    bool cont = true;
    SDL_Event ev;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Couldn't load SDL : " << SDL_GetError() << std::endl;
        return 1;
    }
    
    cont = gfx->openWindow("Test graphics::Graphics", 800, 600);
    if(!cont)
        return 1;
    gfx->setVirtualSize(600, 400);
    gfx->preserveRatio(true);

    while(cont) {
        SDL_WaitEvent(&ev);
        switch(ev.type) {
            case SDL_QUIT:
                cont = false;
                break;
            case SDL_WINDOWEVENT:
                if(ev.window.event == SDL_WINDOWEVENT_CLOSE)
                    cont = false;
                break;
            case SDL_KEYDOWN:
                if(ev.key.keysym.sym == SDLK_q)
                    cont = false;
                break;
            default:
                break;
        }
    }

    gfx->preserveRatio(false); /* Just for testing logging */
    delete gfx;
    core::logger::free();
    return 0;
}

