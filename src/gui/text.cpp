
#include "text.hpp"
#include <sstream>

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
        return m_gfx->stringSize(m_font, m_txt, m_size).height;
    }

    float Text::topPos() const
    {
        return m_gfx->stringSize(m_font, m_txt.substr(0,m_begin), m_size).height;
    }

    float Text::shownHeight() const
    {
        return m_gfx->stringSize(m_font, m_txt.substr(m_begin,m_end), m_size).height;
    }

    void Text::setFont(const std::string& f, float size)
    {
        m_font = f;
        m_size = size;
        updateState();
    }

    void Text::draw()
    {
        m_gfx->draw(m_fmt, m_font, m_size);
    }

    bool Text::action(Widget::Action a)
    {
        size_t last = m_txt.size();
        while(m_txt[last] != '\n' && last > 0)
            --last;
        if(last != 0)
            ++last;

        size_t tmp;
        /* FIXME doesn't appear to be working */
        switch(a) {
            case Widget::First:
                if(m_begin != 0) {
                    updateState(true);
                    return true;
                }
                else
                    return false;
                break;
            case Widget::Last:
                if(m_begin != last) {
                    m_begin = last;
                    updateState(false);
                    return true;
                }
                else
                    return false;
                break;
            case Widget::ScrollUp:
                tmp = m_begin;
                while(m_txt[m_begin] != '\n' && m_begin > 0)
                    --m_begin;
                if(m_begin != 0)
                    ++m_begin;
                if(tmp != m_begin) {
                    updateState(false);
                    return true;
                }
                else
                    return false;
                break;
            case Widget::ScrollDown:
                tmp = m_begin;
                while(m_txt[m_begin] != '\n' && m_begin < m_txt.size())
                    ++m_begin;
                if(m_begin == m_txt.size())
                    return false;
                if(tmp == m_begin)
                    return false;
                updateState(false);
                return true;
            case Widget::ScrollLeft:
            case Widget::ScrollRight:
            case Widget::Select:
            default:
                return false;
                break;
        }
    }

    void Text::updateState(bool rewind)
    {
        /* TODO compute m_end */
        if(width() < 0.00001f || height() < 0.00001f)
            return;

        /* Prepare the text */
        m_fmt = m_txt;
        /* Replace the tab by four spaces */
        for(size_t i = 0; i < m_fmt.size(); ++i) {
            if(m_fmt[i] == '\t') {
                m_fmt[i] = ' ';
                m_fmt.insert(i, "   ");
            }
        }

        if(rewind)
            m_begin = 0;
        else
            m_fmt = m_fmt.substr(m_begin);

        if(m_fmt.empty())
            return;

        /* Find the right width */
        std::istringstream iss(m_fmt);
        std::string line;
        m_fmt.clear();
        while(std::getline(iss, line)) {
            if(!line.empty()) {
                shrinkLine(line, width());
                m_fmt += line;
            }
            m_fmt += '\n';
        }

        /* Find the right height */
        size_t rbound = m_fmt.size() - 1;
        float h = height();
        float hgt = 0.0f;
        do {
            while(m_fmt[rbound] != '\n' && rbound > 0)
                --rbound;
            --rbound;
            hgt = m_gfx->stringSize(m_font, m_fmt.substr(0, rbound), m_size).height;
        } while(hgt >= h);

        m_fmt = m_fmt.substr(0, rbound);
    }

    void Text::shrinkLine(std::string& line, float w)
    {
        /* TODO new lines on words, not letters */
        size_t rbound = line.size();
        float wid = 0.0f;
        do {
            --rbound;
            wid = m_gfx->stringWidth(m_font, line.substr(0, rbound), m_size);
        } while(wid >= w);
        line.insert(rbound, "\n");

        std::string sub = line.substr(rbound+1);
        if(m_gfx->stringWidth(m_font, sub, m_size) >= w)
            shrinkLine(sub, w);

        line = line.substr(0.0f, rbound+1) + sub;
    }

}


