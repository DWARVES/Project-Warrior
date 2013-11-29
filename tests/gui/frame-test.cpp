
#include <iostream>
#include "gui/gui.hpp"
#include "gui/widget.hpp"
#include "gui/frame.hpp"
#include "gui/theme.hpp"
#include "graphics/graphics.hpp"
#include "graphics/color.hpp"
#include "geometry/aabb.hpp"
#include "geometry/circle.hpp"
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
            geometry::Circle circle;
            circle.radius = std::min(width(), height()) / 2.0f;
            geometry::Point center;
            center.x = width() / 2.0f;
            center.y = height() / 2.0f;

            graphics::Color c;
            if(m_focus)
                c = graphics::Color(0, 0, 255);
            else
                c = graphics::Color(127, 127, 255);
            m_gfx->move(center.x, center.y);
            m_gfx->draw(circle, c);
            m_gfx->move(-center.x, -center.y);
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

        virtual bool action(gui::Widget::Action a)
        {
            switch(a) {
                case gui::Widget::First:       std::cout << "Action : First !"       << std::endl; break;
                case gui::Widget::Last:        std::cout << "Action : Last !"        << std::endl; break;
                case gui::Widget::ScrollLeft:  std::cout << "Action : ScrollLeft !"  << std::endl; break;
                case gui::Widget::ScrollRight: std::cout << "Action : ScrollRight !" << std::endl; break;
                case gui::Widget::ScrollUp:    std::cout << "Action : ScrollUp !"    << std::endl; break;
                case gui::Widget::ScrollDown:  std::cout << "Action : ScrollDown !"  << std::endl; break;
                case gui::Widget::Select:      std::cout << "Action : Select !"      << std::endl; break;
                case gui::Widget::Remove:      std::cout << "Action : Remove !"      << std::endl; break;
                default:                       std::cout << "Action : unknown !"     << std::endl; break;
            }
            return true;
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

    /* Loading the theme */
    gui::Theme theme(gfx, "guirc");
    if(!theme.load()) {
        std::cout << "Couldn't load theme." << std::endl;
        return 1;
    }

    /* Setting the gui */
    gui::Gui g(gfx);
    BlueWidget w(gfx);

    gui::Frame fr(gfx, &w);
    theme.apply(&fr);

    g.main(&fr, geometry::Point(50.0f, 50.0f), 700.0f, 500.0f);

    /* Graphic content */
    geometry::AABB bg;
    bg.width = 800;
    bg.height = 600;
    graphics::Color bgc(255, 127, 127);

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

        gfx->beginDraw();
        gfx->draw(bg, bgc);
        gfx->enterNamespace("/");
        gfx->enterNamespace("gui");
        g.draw();
        gfx->enterNamespace("/");
        gfx->endDraw();
        SDL_Delay(1000/60);
    }

    delete gfx;
    core::logger::free();
    SDL_Quit();
    return 0;
}

