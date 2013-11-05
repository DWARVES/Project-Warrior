
#include "text.hpp"
#include <sstream>

namespace gui
{
    Text::Text(graphics::Graphics* gfx)
        : Widget(gfx), m_begin(0), m_size(0)
    {}

    Text::~Text()
    {}

    void Text::setText(const std::string& txt)
    {
        m_txt = txt;
        m_lines = cutToReturn(m_txt);
        shrinkLines();
    }
            
    void Text::addText(const std::string& txt)
    {
        /* TODO to test */
        size_t idx = m_lines.size();
        m_txt += txt;
        std::vector<std::string> nlines = cutToReturn(txt);
        m_lines.insert(m_lines.end(), nlines.begin(), nlines.end());
        shrinkLines(idx);
    }

    std::string Text::getText() const
    {
        return m_txt;
    }

    float Text::width(float w)
    {
        float ret = Widget::width(w);
        shrinkLines();
        return ret;
    }

    float Text::height(float h)
    {
        float ret = Widget::height(h);
        computeSize();
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
        return (float)m_lines.size() * m_pts;
    }

    float Text::topPos() const
    {
        return (float)m_begin * m_pts;
    }

    float Text::shownHeight() const
    {
        return (float)m_size * m_pts;
    }

    void Text::setFont(const std::string& f, float size)
    {
        m_font = f;
        m_pts = size;
        shrinkLines();
        computeSize();
    }

    void Text::draw()
    {
        m_gfx->draw(getPrinted(), m_font, m_pts);
    }

    bool Text::action(Widget::Action a)
    {
        size_t last = std::max((ssize_t)0, (ssize_t)(m_lines.size() - m_size));
        switch(a) {
            case Widget::First:
                if(m_begin != 0) {
                    m_begin = 0;
                    return true;
                }
                else
                    return false;
                break;
            case Widget::Last:
                if(m_begin != last) {
                    m_begin = last;
                    return true;
                }
                else
                    return false;
                break;
            case Widget::ScrollUp:
                if(m_begin > 0) {
                    --m_begin;
                    return true;
                }
                else
                    return false;
                break;
            case Widget::ScrollDown:
                if(m_begin < last) {
                    ++m_begin;
                    return true;
                }
                else
                    return false;
                break;
            case Widget::ScrollLeft:
            case Widget::ScrollRight:
            case Widget::Select:
            default:
                return false;
                break;
        }
    }

    std::vector<std::string> Text::shrinkLine(const std::string& line, float w)
    {
        /* TODO new lines on words, not letters */
        /* FIXME improve performances */
        size_t rbound = line.size();
        float wid = 0.0f;
        std::vector<std::string> ret;
        ret.reserve(10);

        wid = m_gfx->stringWidth(m_font, line.substr(0, rbound), m_pts);
        while(wid >= w) {
            --rbound;
            wid = m_gfx->stringWidth(m_font, line.substr(0, rbound), m_pts);
        }
        ret.push_back(line.substr(0, rbound));

        if(rbound < line.size()) {
            std::string sub = line.substr(rbound);
            if(m_gfx->stringWidth(m_font, sub, m_pts) >= w) {
                std::vector<std::string> others = shrinkLine(sub, w);
                ret.insert(ret.end(), others.begin(), others.end());
            }
            else
                ret.push_back(sub);
        }

        return ret;
    }

    void Text::computeSize()
    {
        if(m_pts < 0.00001f)
            return;
        m_size = static_cast<size_t>(height() / m_pts);
    }

    std::string Text::getPrinted()
    {
        std::string str;
        /* We may reserve a little too mush,
         * but that doesn't matter on an actual computer
         */
        str.reserve(m_size * 20);

        for(size_t i = 0; i < m_size && m_begin + i < m_lines.size(); ++i) {
            str += m_lines[m_begin + i];
            str += '\n';
        }
        return str;
    }

    void Text::shrinkLines(size_t from)
    {
        if(width() < 0.00001f || m_lines.empty())
            return;

        std::vector<std::string> nlines;
        nlines.reserve(m_lines.size() * 3);
        if(from > 0)
            nlines.insert(nlines.begin(), m_lines.begin(), m_lines.begin() + from);

        for(size_t i = from; i < m_lines.size(); ++i) {
            std::vector<std::string> ret = shrinkLine(m_lines[i], width());
            nlines.insert(nlines.end(), ret.begin(), ret.end());
        }

        m_lines = nlines;
    }

    std::vector<std::string> Text::cutToReturn(const std::string& txt)
    {
        std::vector<std::string> ret;
        std::istringstream iss(txt);
        std::string line;
        while(std::getline(iss, line))
            ret.push_back(line);
        return ret;
    }

}


