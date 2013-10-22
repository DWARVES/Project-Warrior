
#include <iostream>
#include "gui/gui.hpp"
#include "gui/widget.hpp"
#include "gui/item.hpp"
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

    cont = gfx->openWindow("Test gui::List", 800, 600);
    if(!cont)
        return 1;

    /* Loading the textures */
    gfx->createNamespace("gui");
    gfx->enterNamespace("gui");
    gfx->loadTexture("r",  "guirc/item/r.png");
    gfx->loadTexture("m",  "guirc/item/m.png");
    gfx->loadTexture("l",  "guirc/item/l.png");
    gfx->loadFont   ("f",  "guirc/item/f.png");
    gfx->loadTexture("rs", "guirc/item/rs.png");
    gfx->loadTexture("ms", "guirc/item/ms.png");
    gfx->loadTexture("ls", "guirc/item/ls.png");
    gfx->loadFont   ("fs", "guirc/item/fs.png");

    /* Loading the item */
    gui::internal::Item item(gfx);
    item.setPart(gui::internal::Item::Right,  false, "r");
    item.setPart(gui::internal::Item::Middle, false, "m");
    item.setPart(gui::internal::Item::Left,   false, "l");
    item.setPart(gui::internal::Item::Font,   false, "f");
    item.setPart(gui::internal::Item::Right,  true,  "rs");
    item.setPart(gui::internal::Item::Middle, true,  "ms");
    item.setPart(gui::internal::Item::Left,   true,  "ls");
    item.setPart(gui::internal::Item::Font,   true,  "fs");
    item.text("HELLO WORLD!");
    item.width(600.0f);
    item.height(70.0f);

    geometry::AABB bg;
    bg.width = 800.0f;
    bg.height = 600.0f;
    graphics::Color bgc(127, 127, 255);
    while(cont)
    {
        ev.update();
        ev.clearInput(); /* We don't need full input save, so we clear it */
        if(ev.isKeyPressed(events::KeyMap::Escape) || ev.isKeyPressed('q'))
            cont = false;
        if(ev.keyJustPressed(events::KeyMap::Right))
            item.scrollRight();
        if(ev.keyJustPressed(events::KeyMap::Left))
            item.scrollLeft();
        if(ev.keyJustPressed(' '))
            item.select(!item.selected());

        gfx->beginDraw();
        gfx->draw(bg, bgc);
        gfx->move(400.0f, 300.0f);
        item.draw();
        gfx->endDraw();
        SDL_Delay(1000/60);
    }

    delete gfx;
    core::logger::free();
    SDL_Quit();
    return 0;
}

