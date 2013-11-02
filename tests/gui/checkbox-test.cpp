
#include <iostream>
#include <sstream>
#include "gui/gui.hpp"
#include "gui/widget.hpp"
#include "gui/checkbox.hpp"
#include "graphics/graphics.hpp"
#include "graphics/color.hpp"
#include "geometry/aabb.hpp"
#include "core/logger.hpp"
#include "events/events.hpp"
#include "events/key.hpp"
#include "events/keymap.hpp"

int main()
{
    core::logger::init();
    core::logger::addOutput(&std::cout);
    graphics::Graphics* gfx = new graphics::Graphics;

    events::Events ev;
    bool cont = true;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        std::cout << "Couldn't load SDL : " << SDL_GetError() << std::endl;
        return 1;
    }

    cont = gfx->openWindow("Test gui::Gui", 800, 600);
    if(!cont)
        return 1;

    /* Loading the ressources */
    gfx->loadTexture("nsel",  "guirc/checkbox/nsel.png");
    gfx->loadTexture("sel",   "guirc/checkbox/sel.png");
    gfx->loadTexture("nselS", "guirc/checkbox/nselS.png");
    gfx->loadTexture("selS",  "guirc/checkbox/selS.png");

    /* Setting up the fillbar */
    gui::CheckBox cb(gfx);
    cb.setTexture(false, "nsel",  "sel");
    cb.setTexture(true,  "nselS", "selS");

    gui::Gui g(gfx);
    g.main(&cb, geometry::Point(50.0f, 50.0f), 700.0f, 500.0f);

    geometry::AABB bg(800,600);
    graphics::Color bgc(127,255,127);

    while(cont)
    {
        ev.update();
        ev.clearInput(); /* We don't need full input save, so we clear it */
        if(ev.isKeyPressed(events::KeyMap::Escape) || ev.isKeyPressed('q'))
            cont = false;
        g.update(ev);

        gfx->beginDraw();
        gfx->draw(bg, bgc);
        g.draw();
        gfx->endDraw();
        SDL_Delay(1000/60);
    }

    delete gfx;
    core::logger::free();
    SDL_Quit();
    return 0;
}
