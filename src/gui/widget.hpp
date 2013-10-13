
#ifndef DEF_GUI_WIDGET
#define DEF_GUI_WIDGET

#include "graphics/graphics.hpp"
#include "geometry/point.hpp"
#include "events/key.hpp"
#include <string>

namespace gui
{
    class Widget
    {
        public:
            Widget() = delete;
            Widget(const Widget&) = delete;
            Widget(graphics::Graphics* gfx);
            virtual ~Widget();

            /* Set size of the widget */
            virtual float width(float w);
            virtual float height(float h);
            /* Get size of the widget */
            virtual float width() const;
            virtual float height() const;

            /* Will be called at every loop
             * Consider the origin to be the top-left of your Widget
             */
            virtual void draw() = 0;

            /* Actions */
            /* The following functions do nothing,
             * If your widget uses them, override them with your own implementation
             */
            virtual void focus(bool);
            virtual void click(const geometry::Point&);
            virtual void mouse(const geometry::Point&);
            virtual void keyPress(events::Key);
            virtual void keyRelease(events::Key);
            virtual void inputText(const std::string&);
            /* Called when the next widget needs to be focused
             * Return true if you needs to go to the next widget outside this one
             * If this is a simple widget, not a layout, always return true (default behaviour)
             */
            virtual bool next();

        protected:
            graphics::Graphics* m_gfx;

        private:
            /* PRIVATE use accessors to access */
            float m_width, m_height;

    };
}

#endif

