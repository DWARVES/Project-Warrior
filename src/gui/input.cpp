
#include "input.hpp"

namespace gui
{
    Input::Input(graphics::Graphics* gfx)
        : Widget(gfx), m_maxLen(0), m_txt(gfx)
    {}

    Input::~Input()
    {}

    float Input::width(float w)
    {
        return m_txt.width(w);
    }

    float Input::height(float h)
    {
        return m_txt.height(h);
    }

    float Input::width() const
    {
        return m_txt.width();
    }

    float Input::height() const
    {
        return m_txt.height();
    }

    void Input::setMaxLen(size_t len)
    {
        m_maxLen = len;
        std::string act = getText();
        if(len > 0 && act.size() > len) {
            act = act.substr(0, len);
            m_txt.setText(act);
        }
    }

    std::string Input::getText() const
    {
        return m_txt.getText();
    }

    void Input::setFont(const std::string& font, float size)
    {
        m_txt.setFont(font, size);
    }

    void Input::draw()
    {
        m_txt.draw();
    }

    void Input::focus(bool f)
    {
        m_txt.focus(f);
    }

    void Input::inputText(const std::string& in)
    {
        if(m_maxLen == 0)
            m_txt.addText(in);
        else {
            std::string txt = getText();
            size_t pos = txt.size();
            txt.reserve(pos + in.size());
            for(size_t i = 0; i < in.size() && txt.size() <= m_maxLen; ++i)
                txt.push_back(in[i]);
            if(txt.size() == pos)
                return;
            else
                m_txt.addText(txt.substr(pos));
        }
        action(Widget::Last);
    }

    bool Input::action(Action a)
    {
        if(a == Widget::Remove && !getText().empty()) {
            std::string str = getText();
            str.pop_back();
            m_txt.setText(str);
            return true;
        }
        else
            return m_txt.action(a);
    }

}


