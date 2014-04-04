
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

/* Defining the platforms. */
/* center.x, center.y, width, height */
float plats[][4] = {
    {0.0f, 0.0f, 100.0f, 1.0f},    /* Low border. */
    {-50.0f, 50.0f, 1.0f, 101.0f}, /* Left border. */
    {50.0f, 50.0f, 1.0f, 101.0f},  /* Right border. */
    {0.0f, 10.0f, 60.0f, 0.1f},    /* Base platform. */
    {-35.0f, 5.0f, 15.0f, 0.1f},   /* Left accesser (from low border to base platform). */
    {35.0f, 5.0f, 15.0f, 0.1f},    /* Right accesser (from low border to base platform). */
    {0.0f, 15.0f, 20.0f, 0.1f},    /* Up 1 platform. */
    {25.0f, 17.0f, 20.0f, 0.1f},   /* Up 2 platform. */
    {-20.0f, 20.0f, 20.0f, 0.1f},  /* Up 3 platform. */
    {-5.0f, 25.0f, 10.0f, 0.1f},   /* Top platform. */
    {0.0f, 0.0f, 0.0f, 0.0f}
};

/* Place the platforms. */
void placePlats(physics::World& w)
{
    geometry::Point pos(0.0f, 0.0f);
    geometry::AABB rect(1.0f, 1.0f);
    int i = 0;

    while(true) {
        pos  = geometry::Point(plats[i][0], plats[i][1]);
        rect = geometry::AABB(plats[i][2], plats[i][3]);
        if(rect.width < 0.001f)
            break;

        std::ostringstream name;
        name << "platform" << i;
        if(i < 3)
            w.createObstacle(name.str(), pos, rect);
        else
            w.createPlatform(name.str(), pos, rect);

        ++i;
    }
}

/* Draw the platforms. */
void drawPlats()
{
    geometry::Point pos(0.0f, 0.0f);
    geometry::AABB rect(1.0f, 1.0f);
    int i = 0;

    while(true) {
        pos  = geometry::Point(plats[i][0], plats[i][1]);
        rect = geometry::AABB(plats[i][2], plats[i][3]);
        if(rect.width < 0.001f)
            break;

        global::gfx->push();
        global::gfx->move(pos.x, pos.y);
        global::gfx->move(-rect.width/2.0f, -rect.height/2.0f);
        global::gfx->draw(rect, graphics::Color(0, 0, 255));
        global::gfx->pop();

        ++i;
    }
}

/* Center the view to the character. */
void center(const gameplay::Character& ch, const geometry::AABB& view)
{
    geometry::Point pos = ch.getPos();
    pos.x *= -1.f;
    pos.y *= -1.f;
    pos.x += view.width / 2.0f;
    pos.y += view.height / 2.0f;
    global::gfx->move(pos.x, pos.y);
}

/* Handle when the character get outside the map. */
void outside(gameplay::Character* ch)
{
    geometry::Point pos = ch->getPos();
    if(pos.x < -60.0f || pos.x > 60.0f
            || pos.y < -20.0f || pos.y > 120.0f)
        ch->warp(geometry::Point(13.3f, 15.0f));
}

int main()
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        std::cout << "Couldn't load SDL : " << SDL_GetError() << std::endl;
        return 1;
    }

    core::logger::init();
    core::logger::addOutput(&std::cout);
    core::logger::minLevel(core::logger::WARNING);
    graphics::Graphics* gfx = new graphics::Graphics;
    global::gfx = gfx;
    lua::exposure::Graphics::setGraphicsInstance(gfx);
    physics::World world(0,-10.0f);

    bool cont = false;
    events::Events evs;
    evs.enableInput(false);
    evs.openJoysticks();
    global::evs = &evs;

    cont = gfx->openWindow("Test graphics::Graphics", 1152, 864);
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
    placePlats(world);
    chara->appearancePos(geometry::Point(13.3f,15.0f));
    chara->world(&world);

    /* Graphics RCS. */
    if(!gfx->loadTexture("bg", "rc/bg.png"))
        return 1;
    if(!gfx->loadTexture("climb", "rc/bg_climb.png"))
        return 1;
    geometry::AABB bg(26.667f, 20.f);
    geometry::AABB bgClimb(100.0f, 100.0f);
    graphics::Color bgc(0, 255, 255);

    /* Appearance over 2 seconds. */
    for(Uint32 i = 0; i <= 20; ++i) {
        gfx->beginDraw();
        gfx->draw(geometry::AABB(1152,864), bgc);
        chara->appear(float(i*5), geometry::AABB(1152,864));
        gfx->endDraw();
        SDL_Delay(100);
    }
    gfx->invertYAxis(true);
    gfx->setVirtualSize(bg.width, bg.height);
    chara->physicMSize(1.0f, true);

    world.start();
    world.enableDebugDraw(true);
    while(cont) {
        evs.update();
        if(evs.closed() || evs.quit() || evs.isKeyPressed(events::KeyMap::Escape))
            cont = false;
        if(evs.keyJustPressed(events::Key(' ')))
            world.enableDebugDraw(!world.debugDraw());
        ctrl.update();

        {
            bool changed = false;
            if(evs.keyJustPressed(events::Key('l'))) {
                bg.height += 2.0f;
                changed = true;
            }
            else if(evs.keyJustPressed(events::Key('m'))) {
                bg.height -= 2.0f;
                changed = true;
            }
            if(changed) {
                bg.width = bg.height / 3.0f * 4.0f;
                gfx->setVirtualSize(bg.width, bg.height);
            }
        }

        gfx->beginDraw();

        if(world.debugDraw())
            gfx->draw(bg, bgc);
        else
            gfx->draw(bg, "bg");

        center(*chara, bg);
        if(!world.debugDraw()) {
            gfx->move(-50.0f, 0.0f);
            gfx->draw(bgClimb, "climb");
            gfx->move(50.0f, 0.0f);
            drawPlats();
        }
        chara->draw();
        world.debugDraw(gfx);
        gfx->endDraw();

        outside(chara);
        world.step();
        SDL_Delay(1000/60);
    }

    delete chara;
    delete gfx;
    core::logger::free();
    SDL_Quit();
    return 0;
}

