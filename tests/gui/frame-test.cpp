
#include <iostream>
#include "gui/gui.hpp"
#include "gui/widget.hpp"
#include "gui/frame.hpp"
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

    /* Loading ressources */
    gfx->enterNamespace("/");
    gfx->createNamespace("gui");
    gfx->enterNamespace("gui");
    gfx->loadTexture("top",    "guirc/top.png");
    gfx->loadTexture("bottom", "guirc/bottom.png");
    gfx->loadTexture("left",   "guirc/left.png");
    gfx->loadTexture("right",  "guirc/right.png");
    gfx->loadTexture("tl",     "guirc/tl.png");
    gfx->loadTexture("tr",     "guirc/tr.png");
    gfx->loadTexture("bl",     "guirc/bl.png");
    gfx->loadTexture("br",     "guirc/br.png");
    gfx->loadTexture("bg",     "guirc/bg.png");

    /* Setting the gui */
    gui::Gui g(gfx);
    BlueWidget w(gfx);

    gui::Frame fr(gfx, &w);
    fr.border(15.0f);
    fr.set(gui::Frame::Top,         "top");
    fr.set(gui::Frame::Bottom,      "bottom");
    fr.set(gui::Frame::Left,        "left");
    fr.set(gui::Frame::Right,       "right");
    fr.set(gui::Frame::TopLeft,     "tl");
    fr.set(gui::Frame::TopRight,    "tr");
    fr.set(gui::Frame::BottomLeft,  "bl");
    fr.set(gui::Frame::BottomRight, "br");
    fr.setBg("bg", false);

    g.main(&fr, geometry::Point(50.0f, 50.0f), 700.0f, 500.0f);

    /* Graphic content */
    geometry::AABB bg;
    bg.width = 800;
    bg.height = 600;
    graphics::Color bgc(255, 127, 127);

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

