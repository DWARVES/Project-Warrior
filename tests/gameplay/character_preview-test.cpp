
#include <SDL.h>
#include <iostream>
#include <sstream>
#include "core/logger.hpp"
#include "graphics/graphics.hpp"
#include "events/events.hpp"
#include "gameplay/character.hpp"
#include "lua/graphicsExposure.hpp"
#include "global.hpp"

namespace global {
    graphics::Graphics* gfx;
}

int main()
{
    core::logger::init();
    core::logger::addOutput(&std::cout);
    graphics::Graphics* gfx = new graphics::Graphics;
    global::gfx = gfx;
    lua::exposure::Graphics::setGraphicsInstance(gfx);

    bool cont = false;
    events::Events evs;
    evs.enableInput(false);

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Couldn't load SDL : " << SDL_GetError() << std::endl;
        return 1;
    }

    cont = gfx->openWindow("Test graphics::Graphics", 800, 600);
    if(!cont)
        return 1;

    gameplay::Character* chara = new gameplay::Character("rcs/chara/stick");
    if(!chara->preload()) {
        core::logger::logm("Couldn't load the character preview.", core::logger::FATAL);
        return 1;
    }
    std::cout << "Character **" << chara->name() << "** loaded : " << chara->desc() << std::endl;

    geometry::AABB bg(800, 600);
    graphics::Color bgc(255, 255, 0);
    geometry::AABB boxprev(360, 560);
    graphics::Color prevcol(0, 255, 255);

    int color = 5;

    while(cont) {
        evs.update();
        if(evs.closed() || evs.quit() || evs.isKeyPressed(events::Key('Q')))
            cont = false;
        if(evs.keyJustPressed(events::Key(' '))) {
            ++color;
            color %= 6;
        }

        gfx->beginDraw();
        gfx->draw(bg, bgc);
        gfx->move(20, 20);
        gfx->draw(boxprev, prevcol);
        chara->preview(boxprev);
        gfx->move(400, 0);
        gfx->draw(boxprev, prevcol);
        chara->bigPreview((gameplay::Character::Color)color, boxprev);
        gfx->endDraw();

        SDL_Delay(1000/30);
    }

    delete chara;
    delete gfx;
    core::logger::free();
    SDL_Quit();
    return 0;
}

