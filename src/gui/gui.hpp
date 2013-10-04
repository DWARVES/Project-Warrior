
#ifndef DEF_GUI_GUI
#define DEF_GUI_GUI

#include "graphics/graphics.hpp"
#include "widget.hpp"

namespace gui
{
    class Gui
    {
        public:
            Gui(graphics::Graphics* gfx);
            Gui() = delete;
            Gui(const Gui&) = delete;
            ~Gui();

            Widget* main(Widget* m, bool tofree = true);
            Widget* main() const;

            /* Will draw everything, must be between Graphics::beginDraw and endDraw */
            void draw();

            /* Will update the gui with the events */
            void update(/* TODO event system */);

        private:
            bool m_tofree;
            Widget* m_main; /* The only one widget, taking the full screen place */
            graphics::Graphics* m_gfx; /* The graphics instance used */
    };
}

#endif

