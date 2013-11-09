
#include <iostream>
#include "gui/gui.hpp"
#include "gui/widget.hpp"
#include "gui/frame.hpp"
#include "gui/list.hpp"
#include "gui/gridlayout.hpp"
#include "gui/theme.hpp"
#include "graphics/graphics.hpp"
#include "graphics/color.hpp"
#include "geometry/aabb.hpp"
#include "core/logger.hpp"
#include "events/events.hpp"
#include "events/key.hpp"
#include "events/keymap.hpp"

/* Widget test */
class ColorWidget : public gui::Widget
{
    public:
        ColorWidget(graphics::Graphics* gfx)
            : Widget(gfx)
        {}
        ~ColorWidget()
        { /* Nothing to do */ }

        virtual void draw()
        {
            geometry::Circle circle;
            circle.radius = std::min(width(), height()) / 2.0f;
            geometry::Point center;
            center.x = width() / 2.0f;
            center.y = height() / 2.0f;

            m_gfx->move(center.x, center.y);
            m_gfx->draw(circle, m_col);
            m_gfx->move(-center.x, -center.y);
        }

        void setColor(graphics::Color col)
        {
            m_col = col;
        }

    private:
        bool m_focus;
        graphics::Color m_col;
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
    /* Loading the list */
    gui::List list(gfx);
    theme.apply(&list);
    list.setItemSize(geometry::AABB(200.0f,50.0f));
    list.addItem(0, "red", 15.0f);
    list.addItem(0, "green", -25.0f);
    list.addItem(0, "blue");
    list.addItem(0, "yellow", -20.0f);
    list.addItem(0, "grey", -5.0f);
    list.addItem(0, "purple");
    list.addItem(0, "azul", 25.0f);
    list.addItem(0, "brown", 10.0f);
    list.addItem(0, "orange", 20.0f);
    list.addItem(0, "white", -7.0f);
    list.addItem(0, "pink");

    /* Loading the frame */
    ColorWidget c(gfx);
    gui::Frame fr(gfx, &c);
    theme.apply(&fr);
    fr.border(15.0f);
    fr.setBg("bg", false);

    /* Setting up the grid */
    gui::GridLayout layout(gfx);
    theme.apply(&layout);
    layout.gaps(10.0f);
    layout.setSize(2, 1);
    layout.addWidget(&list, 0, 0);
    layout.addWidget(&fr,   1, 0);

    gui::Gui g(gfx);
    g.main(&layout, geometry::Point(50.0f,50.0f), 700.0f, 500.0f);

    geometry::AABB bg(800.0f,600.0f);
    graphics::Color bgc(127,127,255);

    while(cont)
    {
        ev.update();
        ev.clearInput(); /* We don't need full input save, so we clear it */
        if(ev.isKeyPressed(events::KeyMap::Escape) || ev.isKeyPressed('q'))
            cont = false;
        if(ev.keyJustPressed(events::KeyMap::Backspace))
            g.focus(!g.focus());
        g.update(ev);

        std::string sel = list.selectedText();
        if(sel == "red")
            c.setColor(graphics::Color(255,0,0));
        else if(sel == "green")
            c.setColor(graphics::Color(0,255,0));
        else if(sel == "blue")
            c.setColor(graphics::Color(0,0,255));
        else if(sel == "white")
            c.setColor(graphics::Color(255,255,255));
        else if(sel == "grey")
            c.setColor(graphics::Color(127,127,127));
        else if(sel == "yellow")
            c.setColor(graphics::Color(255,255,0));
        else if(sel == "purple")
            c.setColor(graphics::Color(200,0,255));
        else if(sel == "azul")
            c.setColor(graphics::Color(0,255,255));
        else if(sel == "orange")
            c.setColor(graphics::Color(255,127,0));
        else if(sel == "brown")
            c.setColor(graphics::Color(127,65,0));
        else if(sel == "pink")
            c.setColor(graphics::Color(255,0,255));
        else
            c.setColor(graphics::Color(0,0,0));

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

