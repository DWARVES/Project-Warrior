
#include <iostream>
#include <sstream>
#include "gui/gui.hpp"
#include "gui/widget.hpp"
#include "gui/radio.hpp"
#include "gui/gridlayout.hpp"
#include "gui/frame.hpp"
#include "gui/theme.hpp"
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

    gui::Theme theme(gfx, "guirc");
    if(!theme.load()) {
        std::cout << "Couldn't load theme." << std::endl;
        return 1;
    }
    /* Loading the radio buttons*/
    gui::Radio a1(gfx, "g1"), a2(gfx, "g1"), a3(gfx, "g1");
    gui::Radio b1(gfx, "g2"), b2(gfx, "g2"), b3(gfx, "g2");
    theme.apply(&a1);
    theme.apply(&a2);
    theme.apply(&a3);
    theme.apply(&b1);
    theme.apply(&b2);
    theme.apply(&b3);

    /* Loading the layouts of the two groups */
    gui::GridLayout layoutA(gfx, 1, 3);
    theme.apply(&layoutA);
    layoutA.addWidget(&a1, 0, 0);
    layoutA.addWidget(&a2, 0, 1);
    layoutA.addWidget(&a3, 0, 2);
    gui::GridLayout layoutB(gfx, 1, 3);
    theme.apply(&layoutB);
    layoutB.addWidget(&b1, 0, 0);
    layoutB.addWidget(&b2, 0, 1);
    layoutB.addWidget(&b3, 0, 2);

    /* Loading the frames */
    gui::Frame frA(gfx, &layoutA);
    theme.apply(&frA);
    gui::Frame frB(gfx, &layoutB);
    theme.apply(&frB);

    /* Setting up the gui */
    gui::GridLayout global(gfx, 2, 1);
    theme.apply(&global);
    global.gaps(10.0f);
    global.addWidget(&frA, 0, 0);
    global.addWidget(&frB, 1, 0);
    gui::Gui g(gfx);
    g.main(&global, geometry::Point(50.0f, 50.0f), 700.0f, 500.0f);

    geometry::AABB bg(800,600);
    graphics::Color bgc(127,255,127);

    ev.openJoysticks();
    while(cont)
    {
        ev.update();
        ev.clearInput(); /* We don't need full input save, so we clear it */
        if(ev.isKeyPressed(events::KeyMap::Escape) || ev.isKeyPressed('q'))
            cont = false;
        if(ev.keyJustPressed(events::KeyMap::Backspace))
            g.focus(!g.focus());
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

