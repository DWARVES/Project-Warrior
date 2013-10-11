
#include "widget.hpp"

namespace gui
{
    Widget::Widget(graphics::Graphics* gfx)
        : m_gfx(gfx), m_width(0), m_height(0)
    {}

    Widget::~Widget()
    {}

    int Widget::width(int w)
    {
        m_width = w;
        return m_width;
    }

    int Widget::height(int h)
    {
        m_height = h;
        return m_height;
    }

    int Widget::width() const
    {
        return m_width;
    }

    int Widget::height() const
    {
        return m_height;
    }


    /* Do nothing, must be overrided */
    void Widget::focus(bool)
    {}

    void Widget::click(const geometry::Point&)
    {}

    void Widget::keyPress(events::Key)
    {}

    void Widget::keyReleased(events::Key)
    {}

    void Widget::inputText(const std::string&)
    {}

    bool Widget::next()
    {
        return true;
    }

}


