
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

            /* Decide the rect of the gui */
            Widget* main(Widget* m, const geometry::Point& pos, float width, float height, bool tofree = false);
            Widget* main() const;
            void setRect(const geometry::Point& p, float width, float height);

            /* Will draw everything, must be between Graphics::beginDraw and endDraw */
            void draw();

            /* Will update the gui with the events */
            void update(const events::Events& ev);
            void focus(bool f);
            bool focus() const;

        private:
            bool m_tofree;
            bool m_focus;
            Widget* m_main; /* The only one widget, taking the full screen place */
            geometry::Point m_pos;
            graphics::Graphics* m_gfx; /* The graphics instance used */

            /* Internal functions */
            bool isInRect(const geometry::Point& p, const geometry::Point& o, float width, float height);
    };
}

#endif

