
#include "item.hpp"

namespace gui
{
    namespace internal
    {
        Item::Item(graphics::Graphics* gfx)
            : m_gfx(gfx), m_width(0.0f), m_height(0.0f), m_state(Norm), m_fontMS(-1.0f), m_resize(true), m_lastSel(0)
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
                
        void Item::resize(bool r)
        {
            m_resize = r;
        }

        float Item::fontMaxSize(float ms)
        {
            m_fontMS = ms;
            updateState();
            return m_fontMS;
        }

        float Item::fontMaxSize() const
        {
            return m_fontMS;
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
            if(m_state != Norm)
                return widthE();
            else
                return m_width;
        }

        float Item::height() const
        {
            if(m_state != Norm)
                return heightE();
            else
                return m_height;
        }

        void Item::select()
        {
            m_state = Selected;
            m_lastSel = SDL_GetTicks();
            updateState();
        }
                
        void Item::focus()
        {
            m_state = Focused;
            updateState();
        }

        void Item::norm()
        {
            m_state = Norm;
            updateState();
        }

        bool Item::selected() const
        {
            return m_state == Selected;
        }
                
        bool Item::focused() const
        {
            return m_state == Focused;
        }

        bool Item::scrollLeft()
        {
            if(m_lbound > 0) {
                --m_lbound;
                updateState(false);
                m_lastSel = SDL_GetTicks();
                return true;
            }
            else
                return false;
        }

        bool Item::scrollRight()
        {
            if(m_rbound < m_text.size()) {
                ++m_lbound;
                m_lastSel = SDL_GetTicks();
                updateState(false);
                return true;
            }
            else
                return false;
        }

        void Item::draw()
        {
            /* Automatic scrolling after 2 seconds */
            static bool re = false;
            Uint32 t = SDL_GetTicks();
            if(m_state == Focused
                    && (m_rext || re)
                    && t - m_lastSel > 2000) {
                re = false;
                t -= m_lastSel;
                t -= 2000;
                size_t b = t / 250;
                b %= std::max(m_text.size(), (size_t)1);
                if(b != m_lbound) {
                    m_lbound = b;
                    updateState(false);
                }
            }
            else if(!m_rext && m_lbound != 0 && !re) {
                m_lastSel = t - 1000;
                re = true;
            }

            m_gfx->push();
            float w = m_width;
            float h = m_height;
            if(m_state != Norm) {
                w = widthE();
                h = heightE();
            }
            m_gfx->move(-w/2.0f, -h/2.0f);
            int sel = (int)m_state;

            /* Drawing sides part */
            geometry::AABB rect;
            rect.width = w * 0.1f;
            rect.height = h;
            m_gfx->draw(rect, m_texts[sel][(unsigned short)Left]);
            m_gfx->move(w - rect.width, 0.0f);
            m_gfx->draw(rect, m_texts[sel][(unsigned short)Right]);

            /* Drawing the middle */
            m_gfx->move(-w + 2*rect.width, 0.0f);
            rect.width = w - 2*rect.width;
            m_gfx->draw(rect, m_texts[sel][(unsigned short)Middle]);

            /* Drawing the text */
            std::string font = m_texts[sel][(unsigned short)Font];
            float fontSize = m_height * (m_state != Norm ? 0.9f : 0.8f);
            if(m_fontMS > 0.0f)
                fontSize = std::min(fontSize, m_fontMS);
            std::string txt("");
            if(m_lext) txt += "...";
            txt += m_text.substr(m_lbound, m_rbound - m_lbound);
            if(m_rext) txt += "...";
            m_gfx->move(0.0f, (h - fontSize) / 2.0f);
            m_gfx->draw(txt, font, fontSize);

            m_gfx->pop();
        }

        void Item::setPart(Part p, State state, const std::string& path)
        {
            m_texts[(unsigned short)state][(unsigned short)p] = path;
        }

        float Item::widthE() const
        {
            if(m_resize)
                return m_width * 1.1f;
            else
                return m_width;
        }

        float Item::heightE() const
        {
            if(m_resize)
                return m_height * 1.1f;
            else
                return m_height;
        }

        void Item::updateState(bool restart)
        {
            if(restart)
                m_lbound = 0;

            if(m_lbound == 0)
                m_lext = false;
            else
                m_lext = true;

            std::string font = m_texts[(unsigned short)m_state][Font];
            std::string rtext = m_text.substr(m_lbound);
            float fontSize = m_height * (m_state != Norm ? 0.9f : 0.8f);
            if(m_fontMS > 0.0f)
                fontSize = std::min(fontSize, m_fontMS);
            float size = 0.0f;
            float extSize = m_gfx->stringWidth(font, "...", fontSize);
            float actualWidth = m_width * 0.8f;
            if(m_state != Norm)
                actualWidth = widthE() * 0.8f;

            if(m_lext)
                size += extSize;

            /* The right part fit in */
            if(m_gfx->stringWidth(font, rtext, fontSize) + size <= actualWidth) {
                m_rbound = m_text.size();
                m_rext = false;
                return;
            }

            /* Need extension also to the right */
            m_rext = true;
            size += extSize;
            m_rbound = m_text.size();
            do {
                --m_rbound;
                rtext = m_text.substr(m_lbound, m_rbound - m_lbound);
            } while(m_gfx->stringWidth(font, rtext, fontSize) + size >= actualWidth
                    && m_lbound < m_rbound);
            return;
        }

    }
}

