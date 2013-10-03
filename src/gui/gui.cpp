
#include "gui.hpp"

namespace gui
{
    Gui::Gui(graphics::Graphics* gfx)
        : m_tofree(false), m_main(NULL), m_gfx(gfx)
    {}

    Gui::~Gui()
    {
        if(m_tofree && m_main != NULL)
            delete m_main;
    }

    Widget* Gui::main(Widget* m, bool tofree)
    {
        if(m_tofree && m_main != NULL)
            delete m_main;
        m_tofree = tofree;
        m_main = m;
        return m_main;
    }

    Widget* Gui::main() const
    {
        return m_main;
    }

    void Gui::draw()
    {
        /* TODO */
    }

    void Gui::update()
    {
        /* TODO */
    }

}



