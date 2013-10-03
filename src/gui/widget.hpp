
#ifndef DEF_GUI_WIDGET
#define DEF_GUI_WIDGET

#include "graphics/graphics.hpp"
#include "geometry/point.hpp"

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
            virtual int width(int w);
            virtual int height(int h);
            /* Get size of the widget */
            virtual int width() const;
            virtual int height() const;

            virtual void draw() = 0;

            /* Actions */
            virtual void focus() = 0;
            virtual void mouseIn() = 0;
            virtual void mouseOut() = 0;
            virtual void click(const geometry::Point& p) = 0;
            virtual void keyPress(char k) = 0;
            virtual void keyReleased(char k) = 0;

        protected:
            graphics::Graphics* m_gfx;

        private:
            /* PRIVATE use accesors to access */
            int m_width, m_height;

    };
}

#endif

