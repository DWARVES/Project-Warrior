
#ifndef DEF_GUI_GUI
#define DEF_GUI_GUI

#include "graphics/graphics.hpp"
#include "geometry/point.hpp"
#include "events/events.hpp"
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

            Widget* main(Widget* m, const geometry::Point& pos, bool tofree = true);
            Widget* main() const;

            /* Will draw everything, must be between Graphics::beginDraw and endDraw */
            void draw();

            /* Will update the gui with the events */
            void update(const events::Events& ev);

        private:
            bool m_tofree;
            Widget* m_main; /* The only one widget, taking the full screen place */
            geometry::Point m_pos;
            graphics::Graphics* m_gfx; /* The graphics instance used */
    };
}

#endif

