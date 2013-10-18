
#include "gridlayout.hpp"
#include "core/logger.hpp"
#include <sstream>

namespace gui
{
    GridLayout::GridLayout(graphics::Graphics* gfx)
        : Widget(gfx), m_rows(0), m_columns(0), m_gaps(1.0f)
    {
        clear();
    }

    GridLayout::GridLayout(graphics::Graphics* gfx, unsigned int mcolumns, unsigned int mrows)
        : GridLayout(gfx)
    {
        setSize(mcolumns, mrows);
    }

    GridLayout::~GridLayout()
    {
        /* Won't free any widget */
    }

    void GridLayout::setSize(unsigned int mcolumns, unsigned int mrows)
    {
        m_rows = mrows;
        m_columns = mcolumns;
        m_map.resize(m_columns);
        for(auto& v : m_map)
            v.resize(m_rows);
        clear();
    }

    unsigned int GridLayout::rows() const
    {
        return m_rows;
    }

    unsigned int GridLayout::columns() const
    {
        return m_columns;
    }

    float GridLayout::gaps(float g)
    {
        m_gaps = g;
        updateSizes();
        return m_gaps;
    }

    float GridLayout::gaps() const
    {
        return m_gaps;
    }

    float GridLayout::width(float w)
    {
        Widget::width(w);
        updateSizes();
        return width();
    }

    float GridLayout::height(float h)
    {
        Widget::height(h);
        updateSizes();
        return height();
    }

    float GridLayout::width() const
    {
        return Widget::width();
    }

    float GridLayout::height() const
    {
        return Widget::height();
    }

    bool GridLayout::addWidget(Widget* widget, unsigned int x, unsigned int y, unsigned int w, unsigned int h)
    {
        StoredWidget st;
        st.widget = widget;
        st.rx     = x;
        st.ry     = y;
        st.w      = w;
        st.h      = h;

        /* Test if the widget can be added */
        for(unsigned int i = 0; i <= st.w; ++i) {
            for(unsigned int j = 0; j <= st.h; ++j) {
                if(isWidget(st.rx + i, st.ry + j))
                    return false;
            }
        }

        /* Add the widget */
        for(unsigned int i = 0; i <= st.w; ++i) {
            for(unsigned int j = 0; j <= st.h; ++j) {
                m_map[st.rx + i][st.ry + j] = st;
            }
        }
        return true;
    }

    bool GridLayout::isWidget(unsigned int x, unsigned int y) const
    {
        if(x >= m_columns || y >= m_rows) {
            std::ostringstream oss;
            oss << "Tryed to access to an invalid position (" << x << "x" << y << ") when maximal size is [" << m_columns << "x" << m_rows << "].";
            core::logger::logm(oss.str(), core::logger::WARNING);
            /* Returns true meaning you can't put a new widget there */
            return true;
        }

        return m_map[x][y].widget != NULL;
    }

    void GridLayout::clear()
    {
        StoredWidget sw;
        sw.widget = NULL;
        for(size_t x = 0; x < m_columns; ++x) {
            for(size_t y = 0; y < m_rows; ++y) {
                m_map[x][y] = sw;
            }
        }
    }

    bool GridLayout::removeWidget(unsigned int x, unsigned int y)
    {
        if(!isWidget(x,y))
            return false;

        StoredWidget sw = m_map[x][y];
        x = sw.rx;
        y = sw.ry;
        size_t mx = x + sw.w;
        size_t my = y + sw.h;

        StoredWidget swd;
        swd.widget = NULL;
        for(; x < mx; ++x) {
            for(; y < my; ++y) {
                m_map[x][y] = swd;
            }
        }

        return true;
    }

    void GridLayout::draw()
    {
        /* BEGIN DEBUG draw rect */
        geometry::AABB r;
        r.width = width();
        r.height = height();
        graphics::Color c(50, 50, 50);
        m_gfx->draw(r, c);
        /* END DEBUG */

        float w = (width() - m_gaps) / (float)m_columns;
        float h = (height() - m_gaps) / (float)m_rows;
        m_gfx->push();
        m_gfx->move(m_gaps, m_gaps);

        for(size_t x = 0; x < m_columns; ++x) {
            for(size_t y = 0; y < m_rows; ++y) {
                m_gfx->push();
                m_gfx->move((float)x * w, (float)y * h);
                if(m_map[x][y].widget && m_map[x][y].rx == x && m_map[x][y].ry == y)
                    m_map[x][y].widget->draw();
                m_gfx->pop();
            }
        }
        m_gfx->pop();
    }

    void GridLayout::focus(bool f)
    {
        /* Unfocus all */
        for(size_t x = 0; x < m_columns; ++x) {
            for(size_t y = 0; y < m_rows; ++y) {
                if(m_map[x][y].widget)
                    m_map[x][y].widget->focus(false);
            }
        }
        m_focused.widget = NULL;

        /* Focus the first one found */
        if(f) {
            Widget* first = NULL;
            for(size_t x = 0; x < m_columns; ++x) {
                for(size_t y = 0; y < m_rows; ++y) {
                    if(m_map[x][y].widget) {
                        first = m_map[x][y].widget;
                        m_focused = m_map[x][y];
                        break;
                    }
                }
                if(first)
                    break;
            }
            if(first)
                first->focus(true);
        }
    }

    void GridLayout::click(const geometry::Point& p)
    {
        if(!m_focused.widget)
            return;

        geometry::Point pos;
        getPos(m_focused.rx, m_focused.ry, pos);
        pos.x = p.x - pos.x;
        pos.y = p.y - pos.y;
        m_focused.widget->click(pos);
    }

    void GridLayout::mouse(const geometry::Point& p)
    {
        focus(false);
        m_focused = widUnderPoint(p);
        if(m_focused.widget)
            m_focused.widget->focus(true);
    }

    void GridLayout::keyPress(events::Key k)
    {
        if(!m_focused.widget)
            return;
        
        switch(k.getSym()) {
            case (events::KeyType)events::KeyMap::Tab:
            case (events::KeyType)events::KeyMap::Right:
                next();
                break;
            default:
                m_focused.widget->keyPress(k);
                break;
        }
    }

    void GridLayout::keyRelease(events::Key k)
    {
        if(!m_focused.widget)
            return;
        m_focused.widget->keyRelease(k);
    }

    void GridLayout::inputText(const std::string& str)
    {
        if(!m_focused.widget)
            return;
        m_focused.widget->inputText(str);
    }

    bool GridLayout::next()
    {
        /* Get first */
        if(!m_focused.widget) {
            focus(true);
            return false;
        }
        /* If the focused widget has inside focusing */
        else if(!m_focused.widget->next()) {
            return false;
        }
        /* Get the next widget */
        else {
            StoredWidget n;
            n.widget = NULL;
            for(size_t x = 0; x < m_columns; ++x) {
                for(size_t y = 0; y < m_rows; ++y) {
                    /* Before the focused one, continue */
                    if(y < m_focused.ry
                            || (y == m_focused.ry && x <= m_focused.rx))
                        continue;
                    if(m_map[x][y].widget
                            && m_map[x][y].rx == x
                            && m_map[x][y].ry == y) {
                        n = m_map[x][y];
                        break;
                    }
                }
                if(n.widget)
                    break;
            }

            focus(false);
            if(!n.widget)
                return true;
            else {
                m_focused = n;
                m_focused.widget->focus(true);
                return false;
            }
        }
    }

    void GridLayout::getPos(unsigned int x, unsigned int y, geometry::Point& pos)
    {
        if(!isWidget(x,y)) {
            pos.x = pos.y = 0.0f;
            return;
        }

        pos.x = float(x+1) * m_gaps + (float)x * caseWidth(true);
        pos.y = float(y+1) * m_gaps + (float)y * caseHeight(true);
    }

    void GridLayout::updateSizes()
    {
        float w = caseWidth(true);
        float h = caseHeight(true);;

        for(size_t x = 0; x < m_columns; ++x) {
            for(size_t y = 0; y < m_rows; ++y) {
                Widget* widget = m_map[x][y].widget;
                if(!widget)
                    continue;
                /* Avoid setting twice the size to a widget */
                if(m_map[x][y].rx != x || m_map[x][y].ry != y)
                    continue;

                float wth  = w * (float)(m_map[x][y].w + 1) + m_gaps * (float)m_map[x][y].w;
                float hght = h * (float)(m_map[x][y].h + 1) + m_gaps * (float)m_map[x][y].h;
                widget->width(wth);
                widget->height(hght);
            }
        }
    }

    GridLayout::StoredWidget GridLayout::widUnderPoint(const geometry::Point& p)
    {
        float mx = caseWidth(false);
        float my = caseHeight(false);;
        size_t px = size_t(p.x / mx);
        size_t py = size_t(p.y / my);

        /* Outside of map dimension */
        if(px >= m_columns || py >= m_rows) {
            StoredWidget sw;
            sw.widget = NULL;
            return sw;
        }
        else {
            /* On the beggining gap */
            if(m_map[px][py].widget) {
                if((m_map[px][py].rx == px && std::fmod(p.x, mx) < m_gaps)
                        || (m_map[px][py].ry == py && std::fmod(p.y, my) < m_gaps)) {
                    StoredWidget sw;
                    sw.widget = NULL;
                    return sw;
                }
            }
            return m_map[px][py];
        }
    }
            
    float GridLayout::caseWidth(bool wg)
    {
        float w = (width() - m_gaps) / (float)m_columns;
        if(wg)
            w -= m_gaps;
        return w;
    }

    float GridLayout::caseHeight(bool wg)
    {
        float h = (height() - m_gaps) / (float)m_rows;
        if(wg)
            h -= m_gaps;
        return h;
    }
}
