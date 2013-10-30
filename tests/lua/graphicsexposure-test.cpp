
#include "core/logger.hpp"
#include "lua/script.hpp"
#include "lua/graphicsExposure.hpp"
#include "lua/exception.hpp"
#include "graphics/graphics.hpp"
#include "events/events.hpp"
#include <iostream>
#include <exception>

int main()
{
    /* Initializing */
    core::logger::init();
    core::logger::addOutput(&std::cout);
    lua::Script scr;
    graphics::Graphics* gfx = new graphics::Graphics;

    /* Load graphics */
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Couldn't load SDL : " << SDL_GetError() << std::endl;
        return 1;
    }
    if(!gfx->openWindow("Test graphics::Graphics", 800, 600))
        return 1;

    try {
        /* Exposing graphics */
        lua::exposure::Graphics::setGraphicsInstance(gfx);
        lua::exposure::Graphics::expose(&scr);

        /* Load scripts */
        if(!scr.load("gfx.lua"))
            throw lua::program_exception("Couldn't load gfx.lua.");

        /* Load ressources */
        bool ret = false;
        scr.callFunction<bool>("init", &ret);
        if(!ret)
            throw lua::program_exception("Error while executing init function of the script.");

        /* Main loop */
        events::Events evs;
        bool cont = true;
        geometry::AABB bg(800,600);
        graphics::Color bgc(127,127,255);

        while(cont) {
            evs.update();
            if(evs.keyJustPressed(' '))
                scr.callFunction<void>("press", NULL);
            if(evs.keyJustReleased(' '))
                scr.callFunction<void>("unpress", NULL);
            if(evs.keyJustPressed('q') || evs.closed() || evs.quit())
                cont = false;

            gfx->beginDraw();
            gfx->draw(bg, bgc);
            scr.callFunction<void>("update", NULL);
            gfx->endDraw();
            SDL_Delay(1000/30);
        }
    }
    catch(const std::exception& e) {
        std::cout << "An exception was caught : " << e.what() << std::endl;
    }
    catch(...) {
        std::cout << "An unknown exception was caught." << std::endl;
    }

    /* Freeing everything */
    delete gfx;
    SDL_Quit();
    core::logger::free();

    return 0;
}

