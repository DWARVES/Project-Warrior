
#include "button.hpp"

namespace gui
{
    Button::Button(graphics::Graphics* gfx)
        : Widget(gfx), m_focus(0), m_last(0)
    {}

    Button::~Button()
    {}

    void Button::setTextures(const std::string& un, const std::string& foc, const std::string& sel)
    {
        m_un  = un;
        m_foc = foc;
        m_sel = sel;
    }

    void Button::draw()
    {
        std::string used;
        if(SDL_GetTicks() - m_last < 1000)
            used = m_sel;
        else if(m_focus == 1)
            used = m_foc;
        else
            used = m_un;

        geometry::AABB rect(width(), height());
        m_gfx->draw(rect, used);
    }

    void Button::focus(bool f)
    {
        m_focus = (f ? 1 : 0);
    }

    bool Button::action(Widget::Action a)
    {
        if(a == Widget::Select) {
            select();
            m_last = SDL_GetTicks();
            return true;
        }
        else
            return false;
    }
            
    void Button::select()
    {
    }

}


