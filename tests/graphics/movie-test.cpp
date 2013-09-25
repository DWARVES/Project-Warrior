
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

    cont = gfx->openWindow("Test graphics::Graphics movie playing abilities", 800, 600);
    if(!cont)
        return 1;
    gfx->setVirtualSize(300, 200);
    gfx->preserveRatio(true);

    if(!gfx->loadMovie("vid", "movie"))
        return 1;
    bool playing = true;
    bool ratio = true;

    /* Background */
    geometry::AABB bgaabb(300.0f, 200.0f);
    graphics::Color bgc;
    bgc.r = bgc.g = 127;
    bgc.b = 255;

    while(cont) {
        while(SDL_PollEvent(&ev)) {
            switch(ev.type) {
                case SDL_QUIT:
                    cont = false;
                    break;
                case SDL_WINDOWEVENT:
                    if(ev.window.event == SDL_WINDOWEVENT_CLOSE)
                        cont = false;
                    break;
                case SDL_KEYDOWN:
                    switch(ev.key.keysym.sym) {
                        case SDLK_q:
                        case SDLK_ESCAPE:
                            cont = false;
                            break;
                        case SDLK_i:
                            gfx->invertYAxis(!gfx->isYAxisInverted());
                            break;
                        case SDLK_r:
                            ratio = !ratio;
                            break;
                        case SDLK_RETURN:
                            gfx->rewindMovie("vid");
                            break;
                        case SDLK_m:
                            gfx->setMovieSpeed("vid", gfx->getMovieSpeed("vid") + 0.5f);
                            break;
                        case SDLK_l:
                            gfx->setMovieSpeed("vid", gfx->getMovieSpeed("vid") - 0.5f);
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        gfx->beginDraw();
        gfx->draw(bgaabb, bgc);
        if(playing) {
            playing = gfx->play("vid", bgaabb, ratio);
            if(!playing) std::cout << "The video ended !" << std::endl;
        }
        gfx->endDraw();
        SDL_Delay(1000/30);
    }

    delete gfx;
    core::logger::free();
    return 0;
}

