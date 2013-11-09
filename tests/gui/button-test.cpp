
#include <iostream>
#include "gui/gui.hpp"
#include "gui/widget.hpp"
#include "gui/gridlayout.hpp"
#include "gui/button.hpp"
#include "gui/theme.hpp"
#include "graphics/graphics.hpp"
#include "graphics/color.hpp"
#include "geometry/aabb.hpp"
#include "core/logger.hpp"
#include "events/events.hpp"
#include "events/key.hpp"
#include "events/keymap.hpp"

/* Button test */
class ButtonTest : public gui::Button
{
    public:
        ButtonTest(graphics::Graphics* gfx)
            : Button(gfx)
        {}
        virtual ~ButtonTest()
        {}

        virtual void select() {
            std::cout << "Selected \"" << text() << "\" button." << std::endl;
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

    cont = gfx->openWindow("Test gui::Gui", 800, 600);
    if(!cont)
        return 1;

    gui::Theme theme(gfx, "guirc");
    if(!theme.load()) {
        std::cout << "Couldn't load theme." << std::endl;
        return 1;
    }
    ButtonTest b00(gfx);
    theme.apply((gui::Button*)&b00);
    b00.text("0x0");
    b00.maxSize(geometry::AABB(-1.0f, 100.0f));
    ButtonTest b01(gfx);
    theme.apply((gui::Button*)&b01);
    b01.text("0x1");
    b01.maxSize(geometry::AABB(-1.0f, 100.0f));
    ButtonTest b1(gfx);
    theme.apply((gui::Button*)&b1);
    b1.text("1x*");
    b1.maxSize(geometry::AABB(-1.0f, 100.0f));

    /* Setting up the gridlayout */
    gui::GridLayout layout(gfx);
    theme.apply(&layout);
    layout.gaps(10.0f);
    layout.setSize(2, 2);
    layout.addWidget(&b00, 0, 0, 0, 0);
    layout.addWidget(&b01, 0, 1, 0, 0);
    layout.addWidget(&b1,  1, 0, 0, 1);

    gui::Gui g(gfx);
    g.main(&layout, geometry::Point(50.0f, 50.0f), 700.0f, 500.0f);
    geometry::AABB bg(800,600);
    graphics::Color bgc(50,50,50);

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
        gfx->draw(bg,bgc);
        g.draw();
        gfx->endDraw();
        SDL_Delay(1000/60);
    }

    delete gfx;
    core::logger::free();
    SDL_Quit();
    return 0;
}

