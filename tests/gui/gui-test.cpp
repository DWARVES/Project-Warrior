
#include <iostream>
#include "gui/gui.hpp"
#include "gui/widget.hpp"
#include "graphics/graphics.hpp"
#include "graphics/color.hpp"
#include "geometry/aabb.hpp"
#include "core/logger.hpp"
#include "events/events.hpp"
#include "events/key.hpp"
#include "events/keymap.hpp"

/* Widget test */
class BlueWidget : public gui::Widget
{
    public:
        BlueWidget(graphics::Graphics* gfx)
            : Widget(gfx), m_focus(false)
        {}
        ~BlueWidget()
        { /* Nothing to do */ }

        virtual void draw()
        {
            geometry::AABB aabb(width(), height());
            graphics::Color c;
            if(m_focus)
                c = graphics::Color(0, 0, 255);
            else
                c = graphics::Color(127, 127, 255);
            m_gfx->draw(aabb, c);
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

        virtual void mouse(const geometry::Point& m)
        {
            std::cout << "Mouse moved to " << m.x << "x" << m.y << std::endl;
        }

    private:
        bool m_focus;
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

    gui::Gui g(gfx);
    BlueWidget w(gfx);
    g.main(&w, geometry::Point(50.0f, 50.0f), 700.0f, 500.0f);

    while(cont)
    {
        ev.update();
        ev.clearInput(); /* We don't need full input save, so we clear it */
        if(ev.isKeyPressed(events::KeyMap::Escape) || ev.isKeyPressed('q'))
            cont = false;
        g.update(ev);

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

