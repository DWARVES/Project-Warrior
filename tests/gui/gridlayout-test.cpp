
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

        virtual void click(const geometry::Point& p)
        {
            std::cout << "Widget clicked at (" << p.x << ";" << p.y << ") [relative coordinates]" << std::endl;
        }

        virtual void keyPress(events::Key k)
        {
            std::cout << "Key " << k.name() << " just pressed on the widget !" << std::endl;
        }

        virtual void keyRelease(events::Key k)
        {
            std::cout << "Key " << k.name() << " just released on the widget !" << std::endl;
        }

        virtual void inputText(const std::string& str)
        {
            std::cout << "\"" << str << "\" text just entered on the widget !" << std::endl;
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

    gui::GridLayout layout(gfx);
    layout.gaps(10.0f);
    layout.setSize(5, 3);
    layout.addWidget(&blue,  0, 0, 0, 2);
    layout.addWidget(&green, 1, 0, 3, 0);
    layout.addWidget(&red,   1, 1, 2, 1);
    layout.addWidget(&grey,  4, 1, 0, 1);

    g.main(&layout, geometry::Point(50.0f, 50.0f), 700.0f, 500.0f);

    while(cont)
    {
        ev.update();
        ev.clearInput(); /* We don't need full input save, so we clear it */
        if(ev.isKeyPressed(events::KeyMap::Escape) || ev.isKeyPressed('q'))
            cont = false;
        g.update(ev);
        if(ev.keyJustPressed(events::KeyMap::Left)) {
            if(!layout.next())
                std::cout << "Ended the layout !" << std::endl;
        }

        gfx->beginDraw();
        g.draw();
        gfx->endDraw();
        SDL_Delay(1000/60);
    }

    delete gfx;
    core::logger::free();
    SDL_Quit();
    return 0;
}

