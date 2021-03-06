
#include <iostream>
#include "gui/gui.hpp"
#include "gui/widget.hpp"
#include "gui/list.hpp"
#include "gui/theme.hpp"
#include "graphics/graphics.hpp"
#include "graphics/color.hpp"
#include "geometry/aabb.hpp"
#include "core/logger.hpp"
#include "events/events.hpp"
#include "events/key.hpp"
#include "events/keymap.hpp"

class TestList : public gui::List
{
    public:
        TestList(graphics::Graphics* gfx)
            : List(gfx)
        {}
        virtual ~TestList() {
        }

        virtual void select()
        {
            std::cout << "Selection of item #" << selected() << " : \"" << selectedText() << "\" (" << *(int*)selectedData() << ")." << std::endl;
        }

        virtual void enter()
        {
            std::cout << "Entered item #" << selected() << " : \"" << selectedText() << "\" (" << *(int*)selectedData() << ")." << std::endl;
        }
};

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

    /* Loading the list */
    gui::Theme theme(gfx, "guirc");
    if(!theme.load()) {
        std::cout << "Couldn't load theme." << std::endl;
        return 1;
    }
    TestList list(gfx);
    theme.apply((gui::List*)&list);

    int n1 = 1, n2 = 2, n3 = 3, n4 = 4,
        n5 = 5, n6 = 6, n7 = 7, n8 = 8,
        n9 = 9, n10 = 10, n11 = 11,
        n12 = 12, n13 = 13, n14 = 14;

    /* Loading the items */
    list.addItem(0, "Last",                             30.0f, &n1);
    list.addItem(0, "Unte",                            -5.0f,  &n2);
    list.addItem(0, "Hy everybody out of here",         0.0f,  &n3);
    list.addItem(0, "Hello world",                      15.0f, &n4);
    list.addItem(0, "Raspberry PI",                     50.0f, &n5);
    list.addItem(0, "@Centered widget !",               0.0f,  &n6);
    list.addItem(0, "Middle2",                          10.0f, &n7);
    list.addItem(0, "Middle1",                         -40.0f, &n8);
    list.addItem(0, "Linux news and advices",          -50.0f, &n9);
    list.addItem(0, "Play the new DWARVES group game",  10.0f, &n10);
    list.addItem(0, "Champitoad",                      -20.0f, &n11);
    list.addItem(0, "Lucas8",                           20.0f, &n12);
    list.addItem(0, "Second",                          -50.0f, &n13);
    list.addItem(0, "First",                            0.0f,  &n14);

    /* Loading the gui */
    gui::Gui g(gfx);
    g.main(&list, geometry::Point(50.0f,50.0f), 700.0f, 500.0f);

    geometry::AABB bg;
    bg.width = 800.0f;
    bg.height = 600.0f;
    graphics::Color bgc(127, 127, 255);
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

