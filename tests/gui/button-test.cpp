
#include <iostream>
#include "gui/gui.hpp"
#include "gui/widget.hpp"
#include "gui/gridlayout.hpp"
#include "gui/button.hpp"
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

    /* Setting up the buttons */
    gfx->loadTexture("r",  "guirc/button/r.png");
    gfx->loadTexture("m",  "guirc/button/m.png");
    gfx->loadTexture("l",  "guirc/button/l.png");
    gfx->loadFont   ("f",  "guirc/button/f.png");
    gfx->loadTexture("rf", "guirc/button/rf.png");
    gfx->loadTexture("mf", "guirc/button/mf.png");
    gfx->loadTexture("lf", "guirc/button/lf.png");
    gfx->loadFont   ("ff", "guirc/button/ff.png");
    gfx->loadTexture("rs", "guirc/button/rs.png");
    gfx->loadTexture("ms", "guirc/button/ms.png");
    gfx->loadTexture("ls", "guirc/button/ls.png");
    gfx->loadFont   ("fs", "guirc/button/fs.png");
    ButtonTest b00(gfx);
    b00.text("0x0");
    b00.maxSize(geometry::AABB(-1.0f, 100.0f));
    ButtonTest b01(gfx);
    b01.text("0x1");
    b01.maxSize(geometry::AABB(-1.0f, 100.0f));
    ButtonTest b1(gfx);
    b1.text("1x*");
    b1.maxSize(geometry::AABB(-1.0f, 100.0f));
    b00.setTexture(gui::Button::Right,  false, "r");
    b00.setTexture(gui::Button::Middle, false, "m");
    b00.setTexture(gui::Button::Left,   false, "l");
    b00.setTexture(gui::Button::Font,   false, "f");
    b00.setTexture(gui::Button::Right,  true,  "rs");
    b00.setTexture(gui::Button::Middle, true,  "ms");
    b00.setTexture(gui::Button::Left,   true,  "ls");
    b00.setTexture(gui::Button::Font,   true,  "fs");
    b00.setSel(gui::Button::Right,  "rf");
    b00.setSel(gui::Button::Middle, "mf");
    b00.setSel(gui::Button::Left,   "lf");
    b00.setSel(gui::Button::Font,   "ff");
    b01.setTexture(gui::Button::Right,  false, "r");
    b01.setTexture(gui::Button::Middle, false, "m");
    b01.setTexture(gui::Button::Left,   false, "l");
    b01.setTexture(gui::Button::Font,   false, "f");
    b01.setTexture(gui::Button::Right,  true,  "rs");
    b01.setTexture(gui::Button::Middle, true,  "ms");
    b01.setTexture(gui::Button::Left,   true,  "ls");
    b01.setTexture(gui::Button::Font,   true,  "fs");
    b01.setSel(gui::Button::Right,  "rf");
    b01.setSel(gui::Button::Middle, "mf");
    b01.setSel(gui::Button::Left,   "lf");
    b01.setSel(gui::Button::Font,   "ff");
    b1.setTexture(gui::Button::Right,  false, "r");
    b1.setTexture(gui::Button::Middle, false, "m");
    b1.setTexture(gui::Button::Left,   false, "l");
    b1.setTexture(gui::Button::Font,   false, "f");
    b1.setTexture(gui::Button::Right,  true,  "rs");
    b1.setTexture(gui::Button::Middle, true,  "ms");
    b1.setTexture(gui::Button::Left,   true,  "ls");
    b1.setTexture(gui::Button::Font,   true,  "fs");
    b1.setSel(gui::Button::Right,  "rf");
    b1.setSel(gui::Button::Middle, "mf");
    b1.setSel(gui::Button::Left,   "lf");
    b1.setSel(gui::Button::Font,   "ff");

    /* Setting up the gridlayout */
    gui::GridLayout layout(gfx);
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

