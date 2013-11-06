
#include <iostream>
#include <sstream>
#include "gui/gui.hpp"
#include "gui/widget.hpp"
#include "gui/input.hpp"
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

    /* Loading the ressources */
    gfx->loadFont("font", "guirc/font.png");
    gui::Input input(gfx);
    input.setFont("font", 30.0f);

    /* Setting up the frame */
    gfx->loadTexture("top",    "guirc/top.png");
    gfx->loadTexture("bottom", "guirc/bottom.png");
    gfx->loadTexture("left",   "guirc/left.png");
    gfx->loadTexture("right",  "guirc/right.png");
    gfx->loadTexture("tl",     "guirc/tl.png");
    gfx->loadTexture("tr",     "guirc/tr.png");
    gfx->loadTexture("bl",     "guirc/bl.png");
    gfx->loadTexture("br",     "guirc/br.png");
    gui::Frame fr(gfx, &input);
    fr.border(15.0f);
    fr.set(gui::Frame::Top,         "top");
    fr.set(gui::Frame::Bottom,      "bottom");
    fr.set(gui::Frame::Left,        "left");
    fr.set(gui::Frame::Right,       "right");
    fr.set(gui::Frame::TopLeft,     "tl");
    fr.set(gui::Frame::TopRight,    "tr");
    fr.set(gui::Frame::BottomLeft,  "bl");
    fr.set(gui::Frame::BottomRight, "br");

    gui::Gui g(gfx);
    g.main(&fr, geometry::Point(50.0f, 50.0f), 700.0f, 500.0f);

    geometry::AABB bg(800,600);
    graphics::Color bgc(127,255,127);

    while(cont)
    {
        ev.update();
        ev.clearInput(); /* We don't need full input save, so we clear it */
        if(ev.isKeyPressed('q'))
            cont = false;
        if(ev.keyJustPressed(events::KeyMap::Escape))
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

