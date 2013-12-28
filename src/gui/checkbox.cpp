
#include "checkbox.hpp"

namespace gui
{
    CheckBox::CheckBox(graphics::Graphics* gfx)
        : Widget(gfx), m_state(0), m_focus(0), m_useSize(false), m_fontSize(-1.0f), m_size(0.0f)
    {}

    CheckBox::~CheckBox()
    {}

    float CheckBox::width(float w)
    {
        Widget::width(w);
        updateSizes();
        return width();
    }

    float CheckBox::height(float h)
    {
        Widget::height(h);
        updateSizes();
        return height();
    }

    float CheckBox::width() const
    {
        return Widget::width();
    }

    float CheckBox::height() const
    {
        return Widget::height();
    }

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

    void CheckBox::setFont(bool f, const std::string& font)
    {
        m_fonts[(f ? 1 : 0)] = font;
        updateSizes();
    }

    void CheckBox::maxSize(float s)
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
        geometry::AABB rect(m_size, m_size);
        m_gfx->move(m_offset.x, m_offset.y);
        m_gfx->draw(rect, m_texts[m_focus][m_state]);

        if(!m_fonts[m_focus].empty() && !m_label.empty()) {
            m_gfx->move(m_size + 1.0f, 0.0f);
            m_gfx->draw(m_label, m_fonts[m_focus], m_fontSize);
        }
        m_gfx->pop();
    }

    void CheckBox::focus(bool f)
    {
        m_focus = (f ? 1 : 0);
        updateSizes();
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

    void CheckBox::label(const std::string& lab)
    {
        m_label = lab;
        updateSizes();
    }

    std::string CheckBox::label() const
    {
        return m_label;
    }

    void CheckBox::updateSizes()
    {
        float w = width(), h = height();
        m_size = w;
        if(m_useSize)
            m_size = m_maxSize + 1.0f;
        m_size = std::min(m_size, h);

        do {
            m_size -= 1.0f;
        } while((m_gfx->stringWidth(m_fonts[m_focus], m_label, m_size) + m_size + 1.0f) > w);

        m_fontSize = m_size;
        m_offset.x = w - (m_gfx->stringWidth(m_fonts[m_focus], m_label, m_size) + m_size + 1.0f);
        m_offset.x /= 2.0f;
        m_offset.y = (h - m_size) / 2.0f;
    }

}


