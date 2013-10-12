
#include "gui.hpp"

namespace gui
{
    Gui::Gui(graphics::Graphics* gfx)
        : m_tofree(false), m_focus(false), m_main(NULL), m_gfx(gfx)
    {}

    Gui::~Gui()
    {
        if(m_tofree && m_main != NULL)
            delete m_main;
    }

    Widget* Gui::main(Widget* m, const geometry::Point& pos, int width, int height, bool tofree)
    {
        if(m_tofree && m_main != NULL)
            delete m_main;
        m_tofree = tofree;
        m_main   = m;
        m_pos    = pos;
        m->width(width);
        m->height(height);
        return m_main;
    }

    Widget* Gui::main() const
    {
        return m_main;
    }
            
    void Gui::setRect(const geometry::Point& p, int width, int height)
    {
        m_pos = p;
        m_main->width(width);
        m_main->height(height);
    }

    void Gui::draw()
    {
        if(!m_main)
            return;

        m_gfx->push();
        m_gfx->move(m_pos.x,m_pos.y);
        m_main->draw();
        m_gfx->pop();
    }

    void Gui::update(const events::Events& ev)
    {
        if(!m_main)
            return;

        geometry::Point pointer = ev.mousePos();
        bool nfocus = isInRect(pointer, m_pos, m_main->width(), m_main->height());

        /* Focus */
        if(nfocus && !m_focus)      /* Focus earned */
            m_main->focus(true);
        else if(!nfocus && m_focus) /* Focus lost */
            m_main->focus(false);
        m_focus = nfocus;
        if(!m_focus)
            return;

        /* Click */
        if(ev.buttonJustPressed(events::Button::Left)) {
            geometry::Point click = pointer;
            click.x -= m_pos.x;
            click.y -= m_pos.y;
            m_main->click(click);
        }

        /* Keyboard */
        std::vector<events::Key> keys = ev.lastKeysPressed();
        for(events::Key k : keys)
            m_main->keyPress(k);
        keys = ev.lastKeysReleased();
        for(events::Key k : keys)
            m_main->keyRelease(k);

        /* Input */
        std::string input = ev.lastInput();
        if(!input.empty())
            m_main->inputText(input);
    }

    bool Gui::isInRect(const geometry::Point& p, const geometry::Point& o, int width, int height)
    {
        if(p.x < o.x
                || p.y < o.y
                || p.x > o.x + (float)width
                || p.y > o.y + (float)height)
            return false;
        else
            return true;
    }

}



