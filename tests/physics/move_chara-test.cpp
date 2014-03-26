
#include <SDL.h>
#include <iostream>
#include <sstream>
#include "core/logger.hpp"
#include "graphics/graphics.hpp"
#include "events/events.hpp"
#include "gameplay/character.hpp"
#include "gameplay/controler.hpp"
#include "lua/graphicsExposure.hpp"
#include "lua/charaExposure.hpp"
#include "physics/World.hpp"
#include "global.hpp"

namespace global {
    graphics::Graphics* gfx;
    events::Events* evs;
}

int main()
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        std::cout << "Couldn't load SDL : " << SDL_GetError() << std::endl;
        return 1;
    }

    core::logger::init();
    core::logger::addOutput(&std::cout);
    graphics::Graphics* gfx = new graphics::Graphics;
    global::gfx = gfx;
    lua::exposure::Graphics::setGraphicsInstance(gfx);
    physics::World world(0,-10.0f);

    bool cont = false;
    events::Events evs;
    evs.enableInput(false);
    evs.openJoysticks();
    global::evs = &evs;

    cont = gfx->openWindow("Test graphics::Graphics", 800, 600);
    if(!cont)
        return 1;

    gameplay::Character* chara = new gameplay::Character("rcs/chara/fighter");
    if(!chara->preload()) {
        core::logger::logm("Couldn't load the character preview.", core::logger::FATAL);
        return 1;
    }
    std::cout << "Character **" << chara->name() << "** loaded : " << chara->desc() << std::endl;
    lua::exposure::characters[0] = chara;

    if(!chara->load(gameplay::Character::None, 0)) {
        core::logger::logm("Couldn't load the character.", core::logger::FATAL);
        return 1;
    }

    /* Loading the controlers. */
    if(!gameplay::Controler::load("rcs/controls")) {
        core::logger::logm("Couldn't load the controlers config.", core::logger::FATAL);
        return 1;
    }
    auto ids = gameplay::Controler::getIDs();
    for(size_t i = 0; i < ids.size(); ++i)
        std::cout << "\t#" << i << " -> " << ids[i] << std::endl;
    std::cout << "Choose your device : ";
    size_t choice;
    std::cin >> choice;
    if(choice >= ids.size())
        return 1;

    ids = gameplay::Controler::listAll();
    std::cout << "All configured :" << std::endl;
    for(auto str : ids)
        std::cout << "\t-> " << str << std::endl;

    gameplay::Controler ctrl(ids[choice]);
    if(!ctrl.isOpen()) {
        core::logger::logm("Couldn't load the keyboard config.", core::logger::FATAL);
        return 1;
    }
    ctrl.attach(chara);

    /* Creating the world. */
    graphics::Color obstcol(255,0,0);
    geometry::AABB groundbox(1600,20);
    world.createPlatform("ground", geometry::Point(800,10), groundbox);
    chara->appearancePos(geometry::Point(800,600));
    chara->world(&world);
    chara->enableMSize(false, geometry::AABB(20.0f, 20.0f));

    geometry::AABB bg(1600, 1200);
    graphics::Color bgc(0, 255, 255);

    /* Appearance over 2 seconds. */
    for(Uint32 i = 0; i <= 20; ++i) {
        gfx->beginDraw();
        gfx->draw(bg, bgc);
        chara->appear(float(i*5), geometry::AABB(800,600));
        gfx->endDraw();
        SDL_Delay(100);
    }
    gfx->invertYAxis(true);
    gfx->setVirtualSize(1600, 1200);

    world.start();
    while(cont) {
        evs.update();
        if(evs.closed() || evs.quit() || evs.isKeyPressed(events::KeyMap::Escape))
            cont = false;
        ctrl.update();

        gfx->beginDraw();
        gfx->draw(bg, bgc);
        gfx->draw(groundbox, obstcol);
        chara->draw();
        gfx->endDraw();

        world.step();
        SDL_Delay(1000/60);
    }

    delete chara;
    delete gfx;
    core::logger::free();
    SDL_Quit();
    return 0;
}

