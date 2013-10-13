
#include "gridlayout.hpp"
#include "core/logger.hpp"
#include <sstream>

namespace gui
{
    GridLayout::GridLayout(graphics::Graphics* gfx)
        : Widget(gfx), m_rows(0), m_columns(0)
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

    bool GridLayout::addWidget(Widget* widget, unsigned int x, unsigned int y, unsigned int w, unsigned int h)
    {
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
        size_t x = sw.rx;
        size_t y = sw.ry;
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
        /* TODO */
    }

    void GridLayout::click(const geometry::Point& p)
    {
        /* TODO */
    }

    void GridLayout::mouse(const geometry::Point& p)
    {
        /* TODO */
    }

    void GridLayout::keyPress(events::Key k)
    {
        /* TODO */
    }

    void GridLayout::keyRelease(events::Key k)
    {
        /* TODO */
    }

    void GridLayout::inputText(const std::string& str)
    {
        /* TODO */
    }

    bool GridLayout::next()
    {
        /* TODO */
    }

}
