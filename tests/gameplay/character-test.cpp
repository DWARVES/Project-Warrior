
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

    if(!chara->load(gameplay::Character::None)) {
        core::logger::logm("Couldn't load the character.", core::logger::FATAL);
        return 1;
    }

    geometry::AABB bg(800, 600);
    graphics::Color bgc(0, 255, 255);
    auto dir = gameplay::Character::Fixed;

    /* Appearance over 2 seconds. */
    for(Uint32 i = 0; i <= 20; ++i) {
        gfx->beginDraw();
        gfx->draw(bg, bgc);
        chara->appear(float(i*5), geometry::AABB(800,600));
        gfx->endDraw();
        SDL_Delay(100);
    }

    while(cont) {
        evs.update();
        if(evs.closed() || evs.quit() || evs.isKeyPressed(events::KeyMap::Escape))
            cont = false;

        if(evs.isKeyPressed(events::KeyMap::Left))
            dir = gameplay::Character::Left;
        else if(evs.isKeyPressed(events::KeyMap::Right))
            dir = gameplay::Character::Right;
        else if(evs.isKeyPressed(events::KeyMap::Up))
            dir = gameplay::Character::Up;
        else if(evs.isKeyPressed(events::KeyMap::Down))
            dir = gameplay::Character::Down;
        else
            dir = gameplay::Character::Fixed;

        if(evs.keyJustPressed(events::Key('a')))
            chara->action(gameplay::Character::Attack, dir);
        else if(evs.keyJustPressed(events::Key('z')))
            chara->action(gameplay::Character::Spell, dir);
        else if(evs.keyJustPressed(events::Key('e')))
            chara->action(gameplay::Character::Smash, dir);
        else if(evs.keyJustPressed(events::Key('q')))
            chara->action(gameplay::Character::Dodge, dir);
        else if(evs.keyJustPressed(events::Key('s')))
            chara->action(gameplay::Character::DDodge, dir);
        else if(evs.keyJustPressed(events::Key('d')))
            chara->action(gameplay::Character::Catch, dir);
        else if(evs.keyJustPressed(events::Key('t')))
            chara->action(gameplay::Character::Run, dir);
        else if(evs.keyJustPressed(events::Key('r')))
            chara->action(gameplay::Character::Walk, dir);
        else if(evs.keyJustPressed(events::Key('o')))
            chara->won(geometry::AABB(800,600));
        else if(evs.keyJustPressed(events::Key('p')))
            chara->lost(geometry::AABB(800,600));

        gfx->beginDraw();
        gfx->draw(bg, bgc);
        chara->draw();
        gfx->endDraw();

        SDL_Delay(1000/30);
    }

    delete chara;
    delete gfx;
    core::logger::free();
    SDL_Quit();
    return 0;
}

