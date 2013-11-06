
#include "scrollbar.hpp"
#include "geometry/aabb.hpp"

namespace gui
{
    ScrollBar::ScrollBar(graphics::Graphics* gfx)
        : Widget(gfx), m_size(0.0f), m_selSize(0.0f), m_selPos(0.0f), m_ratio(10.0f), m_step(1.0f),
        m_interact(true), m_vert(true), m_focus(0)
    {}

    ScrollBar::~ScrollBar()
    {}

    float ScrollBar::size(float s)
    {
        return (m_size = s);
    }

    float ScrollBar::size() const
    {
        return m_size;
    }

    void ScrollBar::selected(float p, float s)
    {
        if(p < 0.0f || s <= 0.0f)
            return;
        m_selSize = s;
        m_selPos  = p;
    }

    void ScrollBar::moveTo(float p)
    {
        if(p <= 0.0f)
            return;
        m_selPos = p;
    }

    float ScrollBar::selPos() const
    {
        return m_selPos;
    }

    float ScrollBar::selSize() const
    {
        return m_selSize;
    }

    float ScrollBar::ratio(float r)
    {
        return (m_ratio = r);
    }

    float ScrollBar::ratio() const
    {
        return m_ratio;
    }

    float ScrollBar::step(float s)
    {
        return (m_step = s);
    }

    float ScrollBar::step() const
    {
        return m_step;
    }

    bool ScrollBar::interactive(bool i)
    {
        return (m_interact = i);
    }

    bool ScrollBar::interactive() const
    {
        return m_interact;
    }

    bool ScrollBar::dir(bool v)
    {
        return (m_vert = v);
    }

    bool ScrollBar::vert() const
    {
        return m_vert;
    }

    void ScrollBar::setTexture(Text t, bool f, const std::string& path)
    {
        m_texts[(f ? 1 : 0)][(unsigned short)t] = path;
    }

    void ScrollBar::draw()
    {
        float w = width();
        float h = height();

        m_gfx->push();
        if(!m_vert) {
            m_gfx->move(0.0f, h);
            m_gfx->rotate(270.0f);
            float tmp = w;
            w = h;
            h = tmp;
        }

        /* Apply the ratio and center */
        float r = h / w;
        if(r > m_ratio) {
            float rh = h;
            h = w * m_ratio;
            m_gfx->move(0.0f, (rh - h) / 2.0f);
        }
        else {
            float rw = w;
            w = h / m_ratio;
            m_gfx->move((rw - w) / 2.0f, 0.0f);
        }

        float in   = h - 2.0f * w;
        float fact = in / m_size;

        /* Drawing the up and down scrollers */
        geometry::AABB rect(w,w);
        m_gfx->draw(rect, m_texts[m_focus][(unsigned short)Up]);
        m_gfx->move(0.0f, in + w);
        m_gfx->draw(rect, m_texts[m_focus][(unsigned short)Down]);

        /* Drawing the scrollbar */
        m_gfx->move(0.0f, -in);
        rect.width  = w;
        rect.height = in;
        m_gfx->draw(rect, m_texts[m_focus][(unsigned short)Norm]);
        m_gfx->move(0.0f, m_selPos * fact);
        rect.height = m_selSize * fact;
        m_gfx->draw(rect, m_texts[m_focus][(unsigned short)Selected]);

        m_gfx->pop();
    }

    void ScrollBar::focus(bool f)
    {
        m_focus = (f ? 1 : 0);
    }

    bool ScrollBar::action(Widget::Action a)
    {
        /* If not interactif, does not handle any event */
        if(!m_interact)
            return false;

        float lastPos = m_size - m_selSize;
        switch(a) {
            case Widget::First:
                if(m_selPos > 0.0f) {
                    m_selPos = 0.0f;
                    return true;
                }
                else
                    return false;
                break;
            case Widget::Last:
                if(m_selPos < lastPos) {
                    m_selPos = lastPos;
                    return true;
                }
                else
                    return false;
                break;
            case Widget::ScrollUp:
            case Widget::ScrollLeft:
                if(m_selPos > 0.0f) {
                    m_selPos = std::max(m_selPos - m_step, 0.0f);
                    return true;
                }
                else
                    return false;
                break;
            case Widget::ScrollDown:
            case Widget::ScrollRight:
                if(m_selPos < lastPos) {
                    m_selPos = std::min(m_selPos + m_step, lastPos);
                    return true;
                }
                else
                    return false;
                break;
            case Widget::Select:
            case Widget::Remove:
            default:
                return false;
        }
    }

    float ScrollBar::percent() const
    {
        float max = m_size - m_selSize;
        return m_selPos / max * 100.0f;
    }
}


