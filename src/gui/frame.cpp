
#include "frame.hpp"

namespace gui
{
    Frame::Frame(graphics::Graphics* gfx)
        : Widget(gfx), m_in(NULL), m_border(0.0f)
    {}

    Frame::Frame(graphics::Graphics* gfx, Widget* in)
        : Frame(gfx)
    {
        m_in = in;
    }

    Frame::~Frame()
    {
        /* Nothing to do */
    }

    float Frame::width(float w)
    {
        float ret = Widget::width(w);
        updateSize();
        return ret;
    }

    float Frame::height(float h)
    {
        float ret = Widget::height(h);
        updateSize();
        return ret;
    }

    float Frame::width() const
    {
        return Widget::width();
    }

    float Frame::height() const
    {
        return Widget::height();
    }

    float Frame::border(float size)
    {
        m_border = size;
        return m_border;
    }

    float Frame::border() const
    {
        return m_border;
    }

    void Frame::set(Frame::Border bd, const std::string& value)
    {
        m_borders[(unsigned short)bd] = value;
    }

    void Frame::set(Widget* in)
    {
        m_in = in;
    }
            
    void Frame::setBg(const std::string& value, bool strict)
    {
        m_bg = value;
        m_strict = strict;
    }

    Widget* Frame::getWidget() const
    {
        return m_in;
    }

    void Frame::draw()
    {
        m_gfx->push();

        /* Draw the background */
        geometry::AABB rect;
        m_gfx->push();
        rect.width = width();
        rect.height = height();
        if(m_strict) {
            rect.width -= 2*m_border;
            rect.height -= 2*m_border;
            m_gfx->move(m_border, m_border);
        }
        if(!m_bg.empty())
            m_gfx->draw(rect, m_bg);
        m_gfx->pop();

        /* Draw the corners */
        rect.width = m_border;
        rect.height = m_border;

        m_gfx->push();
        if(!m_borders[(unsigned short)TopLeft].empty())
            m_gfx->draw(rect, m_borders[(unsigned short)TopLeft]);
        m_gfx->move(width() - m_border, 0.0f);
        if(!m_borders[(unsigned short)TopRight].empty())
            m_gfx->draw(rect, m_borders[(unsigned short)TopRight]);
        m_gfx->move(0.0f, height() - m_border);

        if(!m_borders[(unsigned short)BottomRight].empty())
            m_gfx->draw(rect, m_borders[(unsigned short)BottomRight]);
        m_gfx->move(-width() + m_border, 0.0f);
        if(!m_borders[(unsigned short)BottomLeft].empty())
            m_gfx->draw(rect, m_borders[(unsigned short)BottomLeft]);
        m_gfx->pop();

        /* Draw right and left sides */
        rect.width = m_border;
        rect.height = height() - 2*m_border;
        m_gfx->push();
        m_gfx->move(0.0f, m_border);
        if(!m_borders[(unsigned short)Left].empty())
            m_gfx->draw(rect, m_borders[(unsigned short)Left]);
        m_gfx->move(width() - m_border, 0.0f);
        if(!m_borders[(unsigned short)Right].empty())
            m_gfx->draw(rect, m_borders[(unsigned short)Right]);
        m_gfx->pop();

        /* Draw top and bottom sides */
        rect.width = width() - 2*m_border;
        rect.height = m_border;
        m_gfx->push();
        m_gfx->move(m_border, 0.0f);
        if(!m_borders[(unsigned short)Top].empty())
            m_gfx->draw(rect, m_borders[(unsigned short)Top]);
        m_gfx->move(0.0f, height() - m_border);
        if(!m_borders[(unsigned short)Bottom].empty())
            m_gfx->draw(rect, m_borders[(unsigned short)Bottom]);
        m_gfx->pop();

        /* Draw the widget */
        m_gfx->move(m_border, m_border);
        if(m_in)
            m_in->draw();

        m_gfx->pop();
    }

    void Frame::focus(bool f)
    {
        if(m_in)
            m_in->focus(f);
    }

    void Frame::pointer(const geometry::Point& m)
    {
        if(!m_in)
            return;
        geometry::Point nm = m;
        nm.x -= m_border;
        nm.y -= m_border;
        m_in->pointer(nm);
    }

    bool Frame::action(Widget::Action a)
    {
        if(!m_in)
            return false;
        return m_in->action(a);
    }

    void Frame::inputText(const std::string& in)
    {
        if(m_in)
            m_in->inputText(in);
    }

    void Frame::updateSize()
    {
        if(!m_in)
            return;
        m_in->width(width() - 2*m_border);
        m_in->height(height() - 2*m_border);
    }

}

