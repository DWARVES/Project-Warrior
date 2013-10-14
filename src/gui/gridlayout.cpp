
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

    GridLayout::GridLayout(graphics::Graphics* gfx, unsigned int mrows, unsigned int mcolumns)
        : GridLayout(gfx)
    {
        m_rows = mrows;
        m_columns = mcolumns;
    }

    GridLayout::~GridLayout()
    {
        /* Won't free any widget */
    }

    void GridLayout::setSize(unsigned int mrows, unsigned int mcolumns)
    {
        m_rows = mrows;
        m_columns = mcolumns;
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
        /* TODO */

        /* Add the widget */
        /* TODO */
    }

    bool GridLayout::isWidget(unsigned int x, unsigned int y) const
    {
        if(x >= m_rows || y >= m_columns) {
            std::ostringstream oss;
            oss << "Tryed to access to an invalid position (" << x << "x" << y << ") when maximal size is [" << m_rows << "x" << m_columns << "].";
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
        for(size_t x = 0; x < m_rows; ++x) {
            for(size_t y = 0; y < m_columns; ++y) {
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
        /* TODO */
    }

    void GridLayout::focus(bool f)
    {
        /* Unfocus all */
        for(size_t x = 0; x < m_rows; ++x) {
            for(size_t y = 0; y < m_columns; ++y) {
                if(m_map[x][y].widget)
                    m_map[x][y].widget->focus(false);
            }
        }
        m_focused.widget = NULL;

        /* Focus the first one found */
        if(f) {
            Widget* first;
            for(size_t x = 0; x < m_rows; ++x) {
                for(size_t y = 0; y < m_columns; ++y) {
                    if(m_map[x][y].widget) {
                        first = m_map[x][y].widget;
                        m_focused = m_map[x][y];
                        break;
                    }
                }
            }
            if(first != NULL)
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
        m_focused = widUnderPoint(p);
    }

    void GridLayout::keyPress(events::Key k)
    {
        if(!m_focused.widget)
            return;
        /* TODO handle keys for the layout */
        m_focused.widget->keyPress(k);
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
        /* TODO */
    }

    void GridLayout::getPos(unsigned int x, unsigned int y, geometry::Point& pos)
    {
        if(!isWidget(x,y)) {
            pos.x = pos.y = 0.0f;
            return;
        }

        /* All widgets have same width and height, just uses one of them to get this values */
        pos.x = float(x+1) * m_gaps + (float)x * m_map[x][y].widget->width();
        pos.y = float(y+1) * m_gaps + (float)y * m_map[x][y].widget->height();
    }

    void GridLayout::updateSizes()
    {
        float w = (width()  - m_gaps) / (float)m_rows;
        w -= m_gaps;
        float h = (height() - m_gaps) / (float)m_columns;
        h -= m_gaps;

        for(size_t x = 0; x < m_rows; ++x) {
            for(size_t y = 0; y < m_columns; ++y) {
                Widget* widget = m_map[x][y].widget;
                if(!widget)
                    continue;
                /* Avoid setting twice the size to a widget */
                if(m_map[x][y].rx != x || m_map[x][y].ry != y)
                    continue;
                widget->width(w);
                widget->height(h);
            }
        }
    }

    GridLayout::StoredWidget GridLayout::widUnderPoint(const geometry::Point& p)
    {
        /* FIXME handle better gaps */
        float mx = (width() - m_gaps) / (float)m_rows;
        float my = (height() - m_gaps) / (float)m_columns;
        size_t px = size_t(p.x / mx);
        size_t py = size_t(p.y / my);
        return m_map[px][py];
    }
}
