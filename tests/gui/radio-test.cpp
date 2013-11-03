
#include <iostream>
#include <sstream>
#include "gui/gui.hpp"
#include "gui/widget.hpp"
#include "gui/radio.hpp"
#include "gui/gridlayout.hpp"
#include "gui/frame.hpp"
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

    /* Loading the radio buttons*/
    gfx->loadTexture("nsel",  "guirc/radio/nsel.png");
    gfx->loadTexture("sel",   "guirc/radio/sel.png");
    gfx->loadTexture("nselS", "guirc/radio/nselS.png");
    gfx->loadTexture("selS",  "guirc/radio/selS.png");
    gui::Radio a1(gfx, "g1"), a2(gfx, "g1"), a3(gfx, "g1");
    gui::Radio b1(gfx, "g2"), b2(gfx, "g2"), b3(gfx, "g2");
    a1.setTexture(false, "nsel",  "sel");
    a1.setTexture(true,  "nselS", "selS");
    a1.square(true);
    a2.setTexture(false, "nsel",  "sel");
    a2.setTexture(true,  "nselS", "selS");
    a2.square(true);
    a3.setTexture(false, "nsel",  "sel");
    a3.setTexture(true,  "nselS", "selS");
    a3.square(true);
    b1.setTexture(false, "nsel",  "sel");
    b1.setTexture(true,  "nselS", "selS");
    b1.square(true);
    b2.setTexture(false, "nsel",  "sel");
    b2.setTexture(true,  "nselS", "selS");
    b2.square(true);
    b3.setTexture(false, "nsel",  "sel");
    b3.setTexture(true,  "nselS", "selS");
    b3.square(true);

    /* Loading the layouts of the two groups */
    gui::GridLayout layoutA(gfx, 1, 3);
    layoutA.gaps(20.0f);
    layoutA.addWidget(&a1, 0, 0);
    layoutA.addWidget(&a2, 0, 1);
    layoutA.addWidget(&a3, 0, 2);
    gui::GridLayout layoutB(gfx, 1, 3);
    layoutB.gaps(20.0f);
    layoutB.addWidget(&b1, 0, 0);
    layoutB.addWidget(&b2, 0, 1);
    layoutB.addWidget(&b3, 0, 2);

    /* Loading the frames */
    gfx->loadTexture("top",    "guirc/top.png");
    gfx->loadTexture("bottom", "guirc/bottom.png");
    gfx->loadTexture("left",   "guirc/left.png");
    gfx->loadTexture("right",  "guirc/right.png");
    gfx->loadTexture("tl",     "guirc/tl.png");
    gfx->loadTexture("tr",     "guirc/tr.png");
    gfx->loadTexture("bl",     "guirc/bl.png");
    gfx->loadTexture("br",     "guirc/br.png");
    gfx->loadTexture("bg",     "guirc/bg.png");
    gui::Frame frA(gfx, &layoutA);
    frA.border(15.0f);
    frA.set(gui::Frame::Top,         "top");
    frA.set(gui::Frame::Bottom,      "bottom");
    frA.set(gui::Frame::Left,        "left");
    frA.set(gui::Frame::Right,       "right");
    frA.set(gui::Frame::TopLeft,     "tl");
    frA.set(gui::Frame::TopRight,    "tr");
    frA.set(gui::Frame::BottomLeft,  "bl");
    frA.set(gui::Frame::BottomRight, "br");
    frA.setBg("bg", false);
    gui::Frame frB(gfx, &layoutB);
    frB.border(15.0f);
    frB.set(gui::Frame::Top,         "top");
    frB.set(gui::Frame::Bottom,      "bottom");
    frB.set(gui::Frame::Left,        "left");
    frB.set(gui::Frame::Right,       "right");
    frB.set(gui::Frame::TopLeft,     "tl");
    frB.set(gui::Frame::TopRight,    "tr");
    frB.set(gui::Frame::BottomLeft,  "bl");
    frB.set(gui::Frame::BottomRight, "br");
    frB.setBg("bg", false);

    /* Setting up the gui */
    gui::GridLayout global(gfx, 2, 1);
    global.gaps(10.0f);
    global.addWidget(&frA, 0, 0);
    global.addWidget(&frB, 1, 0);
    gui::Gui g(gfx);
    g.main(&global, geometry::Point(50.0f, 50.0f), 700.0f, 500.0f);

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

