
#include "button.hpp"

namespace gui
{
    Button::Button(graphics::Graphics* gfx)
        : Widget(gfx), m_it(gfx), m_last(0), m_focus(false), m_msize(-1.0f, -1.0f)
    {
        m_it.resize(false);
    }

    Button::~Button()
    {}

    void Button::text(const std::string& t)
    {
        m_it.text(t);
    }

    std::string Button::text() const
    {
        return m_it.text();
    }
            
    void Button::maxSize(const geometry::AABB& msize)
    {
        m_msize = msize;
    }
            
    void Button::maxFontSize(float fms)
    {
        m_it.fontMaxSize(fms);
    }

    float Button::width(float w)
    {
        Widget::width(w);
        if(m_msize.width > 0.0f)
            return m_it.width(std::min(m_msize.width, w));
        else
            return m_it.width(w);
    }

    float Button::height(float h)
    {
        Widget::height(h);
        if(m_msize.height > 0.0f)
            return m_it.height(std::min(m_msize.height, h));
        else
            return m_it.height(h);
    }

    float Button::width() const
    {
        return m_it.width();
    }

    float Button::height() const
    {
        return m_it.height();
    }

    void Button::setTexture(Texture p, State st, const std::string& name)
    {
        m_it.setPart((internal::Item::Part)p, (internal::Item::State)st, name);
    }

    void Button::draw()
    {
        if(SDL_GetTicks() - m_last < 1000)
            m_it.select();
        else if(m_focus)
            m_it.focus();
        else
            m_it.norm();

        m_gfx->push();
        m_gfx->move(Widget::width()/2.0f, Widget::height()/2.0f);
        m_it.draw();
        m_gfx->pop();
    }

    void Button::focus(bool f)
    {
        m_focus = f;
    }

    bool Button::action(Widget::Action a)
    {
        if(a == Widget::Select) {
            select();
            m_last = SDL_GetTicks();
            return true;
        }
        else
            return false;
    }

    void Button::select()
    {
    }
}


