
#include "checkbox.hpp"

namespace gui
{
    CheckBox::CheckBox(graphics::Graphics* gfx)
        : Widget(gfx), m_state(0), m_focus(0)
    {}

    CheckBox::~CheckBox()
    {}

    bool CheckBox::toggle()
    {
        bool st = (m_state == 1);
        st = !st;
        return set(st);
    }

    bool CheckBox::set(bool s)
    {
        return (m_state = (s ? 1 : 0));
    }

    bool CheckBox::get() const
    {
        return (m_state == 1);
    }

    void CheckBox::setTexture(bool f, const std::string& empty, const std::string& selected)
    {
        m_texts[(f ? 1 : 0)][0] = empty;
        m_texts[(f ? 1 : 0)][1] = selected;
    }

    void CheckBox::draw()
    {
        geometry::AABB rect(width(), height());
        m_gfx->draw(rect, m_texts[m_focus][m_state]);
    }

    void CheckBox::focus(bool f)
    {
        m_focus = (f ? 1 : 0);
    }

    bool CheckBox::action(Widget::Action a)
    {
        if(a == Widget::Select) {
            toggle();
            return true;
        }
        else
            return false;
    }

}


