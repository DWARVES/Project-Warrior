
#include "fillbar.hpp"

namespace gui
{
    FillBar::FillBar(graphics::Graphics* gfx)
        : Widget(gfx), m_focus(0), m_vert(true), m_interact(true),
        m_size(0.0f), m_filled(0.0f), m_step(1.0f), m_ratio(10.0f)
    {}

    FillBar::~FillBar()
    {}

    float FillBar::size(float s)
    {
        return (m_size = s);
    }

    float FillBar::size() const
    {
        return m_size;
    }

    float FillBar::set(float f)
    {
        return (m_filled = f);
    }

    float FillBar::get() const
    {
        return m_filled;
    }

    float FillBar::percent() const
    {
        return (m_filled / m_size * 100.0f);
    }
            
    float FillBar::step(float s)
    {
        return (m_step = s);
    }

    float FillBar::step() const
    {
        return m_step;
    }

    float FillBar::ratio(float r)
    {
        return (m_ratio = r);
    }

    float FillBar::ratio() const
    {
        return m_ratio;
    }

    bool FillBar::dir(bool v)
    {
        return (m_vert = v);
    }

    bool FillBar::vert() const
    {
        return m_vert;
    }

    bool FillBar::interactive(bool i)
    {
        return (m_interact = i);
    }

    bool FillBar::interactive() const
    {
        return m_interact;
    }

    void FillBar::setTexture(bool f, const std::string& empty, const std::string& filled)
    {
        int idx = (f ? 1 : 0);
        m_texts[idx][0] = empty;
        m_texts[idx][1] = filled;
    }

    void FillBar::draw()
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
        float fact = h / m_size;

        /* Drawing the whole empty bar */
        geometry::AABB rect(w, h);
        m_gfx->draw(rect, m_texts[m_focus][0]);

        /* Drawing the filled part */
        float ry = m_filled / m_size;
        rect.height = m_filled * fact;
        m_gfx->draw(rect, m_texts[m_focus][1], 1.0f, ry);

        m_gfx->pop();
    }

    void FillBar::focus(bool f)
    {
        m_focus = (f ? 1 : 0);
    }

    bool FillBar::action(Widget::Action a)
    {
        if(!m_interact)
            return false;

        switch(a) {
            case Widget::First:
                if(m_filled > 0.0f) {
                    m_filled = 0.0f;
                    return true;
                }
                else
                    return false;
                break;
            case Widget::Last:
                if(m_filled < m_size) {
                    m_filled = m_size;
                    return true;
                }
                else
                    return false;
                break;
            case Widget::ScrollUp:
                if(!m_vert)
                    return false;
                if(m_filled > 0.0f) {
                    m_filled = std::max(m_filled - m_step, 0.0f);
                    return true;
                }
                else
                    return false;
                break;
            case Widget::ScrollLeft:
                if(m_vert)
                    return false;
                if(m_filled > 0.0f) {
                    m_filled = std::max(m_filled - m_step, 0.0f);
                    return true;
                }
                else
                    return false;
                break;
            case Widget::ScrollDown:
                if(!m_vert)
                    return false;
                if(m_filled < m_size) {
                    m_filled = std::min(m_filled + m_step, m_size);
                    return true;
                }
                else
                    return false;
                break;
            case Widget::ScrollRight:
                if(m_vert)
                    return false;
                if(m_filled < m_size) {
                    m_filled = std::min(m_filled + m_step, m_size);
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

}


