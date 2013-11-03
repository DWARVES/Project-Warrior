
#include "checkbox.hpp"

namespace gui
{
    CheckBox::CheckBox(graphics::Graphics* gfx)
        : Widget(gfx), m_state(0), m_focus(0), m_square(false), m_useSize(false)
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

    bool CheckBox::square(bool s)
    {
        return (m_square = s);
    }

    bool CheckBox::square() const
    {
        return m_square;
    }

    void CheckBox::maxSize(geometry::AABB s)
    {
        m_maxSize = s;
        m_useSize = true;
    }

    void CheckBox::disableMaxSize()
    {
        m_useSize = false;
    }

    void CheckBox::draw()
    {
        m_gfx->push();
        geometry::AABB rect(width(), height());

        if(m_useSize) {
            rect.width  = std::min(m_maxSize.width,  rect.width);
            rect.height = std::min(m_maxSize.height, rect.height);
        }

        if(m_square) {
            rect.width  = std::min(rect.width, rect.height);
            rect.height = rect.width;
        }

        if(m_square || m_useSize) {
            m_gfx->move(width() / 2.0f - rect.width / 2.0f,
                    height() / 2.0f - rect.height / 2.0f);
        }

        m_gfx->draw(rect, m_texts[m_focus][m_state]);
        m_gfx->pop();
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


