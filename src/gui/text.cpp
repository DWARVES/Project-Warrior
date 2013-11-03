
#include "text.hpp"

namespace gui
{
    Text::Text(graphics::Graphics* gfx)
        : Widget(gfx), m_begin(0)
    {}

    Text::~Text()
    {}

    void Text::setText(const std::string& txt)
    {
        m_txt = txt;
        updateState();
    }

    std::string Text::getText() const
    {
        return m_txt;
    }

    float Text::width(float w)
    {
        float ret = Widget::width(w);
        updateState(true);
        return ret;
    }

    float Text::height(float h)
    {
        float ret = Widget::height(h);
        updateState(true);
        return ret;
    }

    float Text::width() const
    {
        return Widget::width();
    }

    float Text::height() const
    {
        return Widget::height();
    }

    float Text::totalHeight() const
    {
        /* TODO */
    }

    float Text::topPos() const
    {
        /* TODO */
    }

    float Text::shownHeight() const
    {
        /* TODO */
    }

    void Text::setFont(const std::string& f)
    {
        m_font = f;
    }

    void Text::draw()
    {
        /* TODO */
    }
            
    bool Text::action(Widget::Action a)
    {
        /* TODO */
    }

    void Text::updateState(bool rewind)
    {
        /* TODO */
    }

}


