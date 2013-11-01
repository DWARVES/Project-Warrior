
#include <iostream>
#include <sstream>
#include "gui/gui.hpp"
#include "gui/widget.hpp"
#include "gui/scrollbar.hpp"
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
    gfx->loadTexture("up",    "guirc/scrollbar/up.png");
    gfx->loadTexture("down",  "guirc/scrollbar/down.png");
    gfx->loadTexture("norm",  "guirc/scrollbar/norm.png");
    gfx->loadTexture("sel",   "guirc/scrollbar/sel.png");
    gfx->loadTexture("upS",   "guirc/scrollbar/upS.png");
    gfx->loadTexture("downS", "guirc/scrollbar/downS.png");
    gfx->loadTexture("selS",  "guirc/scrollbar/selS.png");
    gfx->loadFont   ("font",  "guirc/font.png");

    /* Setting up the scrollbar */
    gui::ScrollBar sc(gfx);
    sc.setTexture(gui::ScrollBar::Up,       false, "up");
    sc.setTexture(gui::ScrollBar::Down,     false, "down");
    sc.setTexture(gui::ScrollBar::Norm,     false, "norm");
    sc.setTexture(gui::ScrollBar::Selected, false, "sel");
    sc.setTexture(gui::ScrollBar::Up,       true,  "upS");
    sc.setTexture(gui::ScrollBar::Down,     true,  "downS");
    sc.setTexture(gui::ScrollBar::Norm,     true,  "norm");
    sc.setTexture(gui::ScrollBar::Selected, true,  "selS");
    sc.size(100);
    sc.selected(40, 20);
    sc.step(5.0f);

    gui::Gui g(gfx);
    g.main(&sc, geometry::Point(50.0f, 50.0f), 700.0f, 500.0f);

    geometry::AABB bg(800,600);
    graphics::Color bgc(127,255,127);

    while(cont)
    {
        ev.update();
        ev.clearInput(); /* We don't need full input save, so we clear it */
        if(ev.isKeyPressed(events::KeyMap::Escape) || ev.isKeyPressed('q'))
            cont = false;
        if(ev.keyJustPressed(events::KeyMap::Return))
            sc.dir(!sc.vert());
        g.update(ev);

        std::ostringstream oss;
        oss << sc.percent();
        std::string text(oss.str());
        text += "%";

        gfx->beginDraw();
        gfx->draw(bg, bgc);
        gfx->draw(text, "font");
        g.draw();
        gfx->endDraw();
        SDL_Delay(1000/60);
    }

    delete gfx;
    core::logger::free();
    SDL_Quit();
    return 0;
}

