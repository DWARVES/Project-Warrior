
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
        m_main->width(width);
        m_main->height(height);
        m_main->focus(m_focus);
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
        m_focus = f;
        if(!m_main)
            return;

        if(f && !m_focus) /* Focus earned */
            m_main->focus(true);
        else if(!f && m_focus) /* Focus lost */
            m_main->focus(false);
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
        /* Directions */
        std::vector<std::pair<int,events::Joystick*>> axes = ev.lastAxesMoved();
        std::vector<std::pair<int,events::Joystick*>> hats = ev.lastHatsMoved();
        bool left = false;
        bool right = false;
        bool up = false;
        bool down = false;
        for(auto p : axes) {
            if(p.first % 2 == 0) {
                if(p.second->axis(p.first) > 16384 && !m_joys[p].first) {
                    m_joys[p].first = true;
                    right = true;
                }
                else if(p.second->axis(p.first) < 16000)
                    m_joys[p].first = false;

                if(p.second->axis(p.first) < -16384 && !m_joys[p].second) {
                    m_joys[p].second = true;
                    left = true;
                }
                else if(p.second->axis(p.first) > -16000)
                    m_joys[p].second = false;
            }
            else {
                if(p.second->axis(p.first) > 16384 && !m_joys[p].first) {
                    m_joys[p].first = true;
                    down = true;
                }
                else if(p.second->axis(p.first) < 16000)
                    m_joys[p].first = false;

                if(p.second->axis(p.first) < -16384 && !m_joys[p].second) {
                    m_joys[p].second = true;
                    up = true;
                }
                else if(p.second->axis(p.first) > -16000)
                    m_joys[p].second = false;
            }
        }
        for(auto p : hats) {
            switch(p.second->hat(p.first)) {
                case events::JoyHatState::Up:    up = true;    break;
                case events::JoyHatState::Down:  down = true;  break;
                case events::JoyHatState::Left:  left = true;  break;
                case events::JoyHatState::Right: right = true; break;
                case events::JoyHatState::RightUp:
                case events::JoyHatState::RightDown:
                case events::JoyHatState::LeftUp:
                case events::JoyHatState::LeftDown:
                case events::JoyHatState::Center:
                default:                                       break;
            }
        }

        /* LEFT */
        if(left || ev.keyJustPressed(events::KeyMap::Left))
            m_main->action(Widget::ScrollLeft);
        /* RIGHT */
        if(right || ev.keyJustPressed(events::KeyMap::Right))
            m_main->action(Widget::ScrollRight);
        /* UP */
        if(up || ev.keyJustPressed(events::KeyMap::Up))
            m_main->action(Widget::ScrollUp);
        /* DOWN */
        if(down || ev.keyJustPressed(events::KeyMap::Down))
            m_main->action(Widget::ScrollDown);

        /* SELECT */
        if(ev.keyJustPressed(events::KeyMap::Return)
                || !ev.lastJoyButtonsPressed().empty())
            m_main->action(Widget::Select);
        /* FIRST */
        if(ev.keyJustPressed(events::KeyMap::Begin))
            m_main->action(Widget::First);
        /* END */
        if(ev.keyJustPressed(events::KeyMap::End))
            m_main->action(Widget::Last);
        /* REMOVE */
        if(ev.keyJustPressed(events::KeyMap::Backspace))
            m_main->action(Widget::Remove);
    }

}



