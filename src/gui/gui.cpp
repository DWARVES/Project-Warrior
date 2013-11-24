
#include "gui.hpp"
#include "widget.hpp"

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

    Widget* Gui::main(Widget* m, const geometry::Point& pos, float width, float height, bool tofree)
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
            
    void Gui::setRect(const geometry::Point& p, float width, float height)
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

    void Gui::focus(bool f)
    {
        if(f && !m_focus) /* Focus earned */
            m_main->focus(true);
        else if(!f && m_focus) /* Focus lost */
            m_main->focus(false);
        m_focus = f;
    }
            
    bool Gui::focus() const
    {
        return m_focus;
    }

    void Gui::update(const events::Events& ev)
    {
        if(!m_main || !m_focus)
            return;

        /* Input */
        std::string input = ev.lastInput();
        if(!input.empty())
            m_main->inputText(input);
        if(ev.keyJustPressed(events::KeyMap::Enter))
            m_main->inputText("\n");

        /* Actions */
        /** @todo Handle joysticks and allow personnalization of controls. */
        if(ev.keyJustPressed(events::KeyMap::Left))
            m_main->action(Widget::ScrollLeft);
        if(ev.keyJustPressed(events::KeyMap::Right))
            m_main->action(Widget::ScrollRight);
        if(ev.keyJustPressed(events::KeyMap::Up))
            m_main->action(Widget::ScrollUp);
        if(ev.keyJustPressed(events::KeyMap::Down))
            m_main->action(Widget::ScrollDown);
        if(ev.keyJustPressed(events::KeyMap::Return))
            m_main->action(Widget::Select);
        if(ev.keyJustPressed(events::KeyMap::Begin))
            m_main->action(Widget::First);
        if(ev.keyJustPressed(events::KeyMap::End))
            m_main->action(Widget::Last);
        if(ev.keyJustPressed(events::KeyMap::Backspace))
            m_main->action(Widget::Remove);
    }

}



