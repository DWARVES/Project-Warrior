
#include <iostream>
#include <sstream>
#include "gui/gui.hpp"
#include "gui/widget.hpp"
#include "gui/text.hpp"
#include "gui/scrollbar.hpp"
#include "gui/gridlayout.hpp"
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

    /* Loading the ressources */
    gui::Theme theme(gfx, "guirc");
    if(!theme.load()) {
        std::cout << "Couldn't load theme." << std::endl;
        return 1;
    }
    gui::Text txt(gfx);
    theme.apply(&txt);
    txt.setText("Linux is a Unix-like and POSIX-compliant computer operating system assembled under the model of free and open source software development and distribution. The main form of distribution are Linux distributions. The defining component of Linux is the Linux kernel, an operating system kernel first released on 5 October 1991, by Linus Torvalds.\n\tBecause it considers Linux to be a variant of the GNU operating system, initiated in 1983 by Richard Stallman, the Free Software Foundation prefers the name GNU/Linux when referring to the operating system as a whole (see GNU/Linux naming controversy).\n\nLinux was originally developed as a free operating system for Intel x86-based personal computers. It has since been ported to more computer hardware platforms than any other operating system. It is a leading operating system on servers and other big iron systems such as mainframe computers and supercomputers: as of June 2013, more than 95% of the world's 500 fastest supercomputers run some variant of Linux, including all the 44 fastest. Linux also runs on embedded systems (devices where the operating system is typically built into the firmware and highly tailored to the system) such as mobile phones, tablet computers, network routers, building automation controls, televisions and video game consoles; the Android system in wide use on mobile devices is built on the Linux kernel.");



    /* Setting up the scrollbar */
    gui::ScrollBar sc(gfx);
    theme.apply(&sc);
    sc.interactive(false);

    /* Setting up the gridlayout */
    gui::GridLayout layout(gfx, 5, 1);
    theme.apply(&layout);
    layout.addWidget(&txt, 0, 0, 3, 0);
    layout.addWidget(&sc,  4, 0, 0, 0);

    gui::Gui g(gfx);
    g.main(&layout, geometry::Point(50.0f, 50.0f), 700.0f, 500.0f);

    geometry::AABB bg(800,600);
    graphics::Color bgc(127,255,127);

    while(cont)
    {
        ev.update();
        ev.clearInput(); /* We don't need full input save, so we clear it */
        if(ev.isKeyPressed(events::KeyMap::Escape) || ev.isKeyPressed('q'))
            cont = false;
        if(ev.keyJustPressed(events::KeyMap::Backspace))
            g.focus(!g.focus());
        g.update(ev);

        sc.size(txt.totalHeight());
        sc.selected(txt.topPos(), txt.shownHeight());

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

