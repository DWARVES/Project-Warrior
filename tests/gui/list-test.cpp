
#include <iostream>
#include "gui/gui.hpp"
#include "gui/widget.hpp"
#include "gui/list.hpp"
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

    /* Loading the list */
    gui::List list(gfx);
    list.setPart(gui::List::Right,  false, "r");
    list.setPart(gui::List::Middle, false, "m");
    list.setPart(gui::List::Left,   false, "l");
    list.setPart(gui::List::Font,   false, "f");
    list.setPart(gui::List::Right,  true,  "rs");
    list.setPart(gui::List::Middle, true,  "ms");
    list.setPart(gui::List::Left,   true,  "ls");
    list.setPart(gui::List::Font,   true,  "fs");
    list.setItemSize(geometry::AABB(500.0f,50.0f));

    /* Loading the items */
    list.addItem(0, "Last", 30.0f);
    list.addItem(0, "Unte", -5.0f);
    list.addItem(0, "Hy everybody out of here");
    list.addItem(0, "Hello world", 15.0f);
    list.addItem(0, "Raspberry PI", 50.0f);
    list.addItem(0, "@Centered widget !", 0.0f);
    list.addItem(0, "Middle2", 10.0f);
    list.addItem(0, "Middle1", -40.0f);
    list.addItem(0, "Linux news and advices", -50.0f);
    list.addItem(0, "Play the new DWARVES group game", 10.0f);
    list.addItem(0, "Champitoad", -20.0f);
    list.addItem(0, "Lucas8", 20.0f);
    list.addItem(0, "Second", -50.0f);
    list.addItem(0, "First");

    /* Loading the gui */
    gui::Gui g(gfx);
    g.main(&list, geometry::Point(50.0f,50.0f), 700.0f, 500.0f);

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
        g.update(ev);
        if(ev.keyJustPressed(' ')) {
            std::cout << std::dec << "Selected : \"" << list.selectedText() << "\", #" 
                << list.selected() << " (id : " << std::hex << list.selectedID() << ")" << std::endl;
        }

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

