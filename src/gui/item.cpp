
#include "item.hpp"

namespace gui
{
    namespace internal
    {
        Item::Item(graphics::Graphics* gfx)
            : m_gfx(gfx), m_width(0.0f), m_height(0.0f), m_selected(false)
        {}

        Item::~Item()
        {
            /* Nothing to do */
        }

        std::string Item::text(const std::string& t)
        {
            m_text = t;
            updateState();
            return m_text;
        }

        std::string Item::text() const
        {
            return m_text;
        }

        float Item::width(float w)
        {
            m_width = w;
            updateState();
            return width();
        }

        float Item::height(float h)
        {
            m_height = h;
            updateState();
            return height();
        }

        float Item::width() const
        {
            if(m_selected)
                return widthE();
            else
                return m_width;
        }

        float Item::height() const
        {
            if(m_selected)
                return heightE();
            else
                return m_height;
        }

        bool Item::select(bool s)
        {
            if(m_selected == s)
                return m_selected;
            m_selected = s;
            updateState();
            return m_selected;
        }

        bool Item::selected() const
        {
            return m_selected;
        }

        void Item::scrollLeft()
        {
            /* TODO */
        }

        void Item::scrollRight()
        {
            /* TODO */
        }

        void Item::draw()
        {
            /* TODO */
        }

        void Item::setPart(Part p, bool state, const std::string& path)
        {
            int st = (state ? 1 : 0);
            m_texts[st][(unsigned short)p] = path;
        }

        float Item::widthE() const
        {
            return m_width * 1.1f;
        }

        float Item::heightE() const
        {
            return m_height * 1.1f;
        }

        void Item::updateState()
        {
            /* TODO */
        }

    }
}

