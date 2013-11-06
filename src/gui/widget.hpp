
#ifndef DEF_GUI_WIDGET
#define DEF_GUI_WIDGET

#include "graphics/graphics.hpp"
#include "geometry/point.hpp"
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
            enum Action {
                First,
                Last,
                ScrollLeft,
                ScrollRight,
                ScrollUp,
                ScrollDown,
                Select,
                Remove,
            };
            /* The following functions do nothing,
             * If your widget uses them, override them with your own implementation
             */
            virtual void focus(bool);
            virtual void inputText(const std::string&);
            /* Return true if the action was handled */
            virtual bool action(Action);

        protected:
            graphics::Graphics* m_gfx;

        private:
            /* PRIVATE use accessors to access */
            float m_width, m_height;

    };
}

#endif

