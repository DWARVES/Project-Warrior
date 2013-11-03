
#include <iostream>
#include "gui/gui.hpp"
#include "gui/widget.hpp"
#include "gui/gridlayout.hpp"
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
        ColorWidget(graphics::Graphics* gfx, graphics::Color c, graphics::Color fc)
            : Widget(gfx), m_focus(false), m_c(c), m_fc(fc)
        {}

        ~ColorWidget()
        { /* Nothing to do */ }

        virtual void draw()
        {
            geometry::AABB aabb(width(), height());
            if(m_focus)
                m_gfx->draw(aabb, m_fc);
            else
                m_gfx->draw(aabb, m_c);
        }

        virtual void focus(bool f)
        {
            m_focus = f;
        }

        virtual void inputText(const std::string& str)
        {
            std::cout << "\"" << str << "\" text just entered on the widget !" << std::endl;
        }

        virtual void pointer(const geometry::Point& m)
        {
            std::cout << "Mouse moved to " << m.x << "x" << m.y << std::endl;
        }

    private:
        bool m_focus;
        graphics::Color m_c;
        graphics::Color m_fc;
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

    /* Setting up the gui */
    gui::Gui g(gfx);
    ColorWidget blue (gfx, graphics::Color(127,127,255), graphics::Color(0,0,255));
    ColorWidget green(gfx, graphics::Color(127,255,127), graphics::Color(0,255,0));
    ColorWidget red  (gfx, graphics::Color(255,127,127), graphics::Color(255,0,0));
    ColorWidget grey (gfx, graphics::Color(127,127,127), graphics::Color(255,255,255));
    ColorWidget c1   (gfx, graphics::Color(255,255,127), graphics::Color(255,255,0));
    ColorWidget c2   (gfx, graphics::Color(127,255,255), graphics::Color(0,255,255));
    ColorWidget c3   (gfx, graphics::Color(255,127,255), graphics::Color(255,0,255));
    ColorWidget b    (gfx, graphics::Color(0,0,0),       graphics::Color(127,127,127));
    ColorWidget w    (gfx, graphics::Color(200,200,200), graphics::Color(255,255,255));

    gui::GridLayout layout(gfx);
    layout.gaps(10.0f);
    layout.setSize(5, 3);
    /* First row */
    layout.addWidget(&blue,  0, 0, 2, 0);
    layout.addWidget(&green, 0, 1, 1, 0);
    layout.addWidget(&red,   0, 2, 1, 0);
    /* Middle row */
    layout.addWidget(&grey,  3, 0, 0, 0);
    layout.addWidget(&c1,    2, 1, 2, 0);
    layout.addWidget(&c2,    2, 2, 0, 0);
    /* Last row */
    layout.addWidget(&c3,    4, 0, 0, 0);
    layout.addWidget(&b,     4, 1, 0, 0);
    layout.addWidget(&w,     3, 2, 1, 0);

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

