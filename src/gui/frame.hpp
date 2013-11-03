
#ifndef DEF_GUI_FRAME
#define DEF_GUI_FRAME

#include "gui/widget.hpp"
#include "graphics/graphics.hpp"
#include <string>

namespace gui
{
    class Frame : public Widget
    {
        public:
            Frame() = delete;
            Frame(const Frame&) = delete;
            Frame(graphics::Graphics* gfx);
            Frame(graphics::Graphics* gfx, Widget* in);
            ~Frame();

            /* Size */
            virtual float width(float w);
            virtual float height(float h);
            virtual float width() const;
            virtual float height() const;
            float border(float size);
            float border() const;

            /* Pictures */
            enum Border : unsigned short {
                TopLeft,
                TopRight,
                BottomLeft,
                BottomRight,
                Top,
                Left,
                Bottom,
                Right,
                Last,
            };
            void set(Border bd, const std::string& value);

            /* Background */
            /* If !strict, the bg will take the whole width and height, event the borders */
            void setBg(const std::string& value, bool strict = true);

            /* Widget in */
            void set(Widget* in);
            Widget* getWidget() const;

            virtual void draw();

            /* Events */
            virtual void focus(bool f);
            virtual void inputText(const std::string& in);
            virtual bool action(Widget::Action a);

        private:
            /* The names of the pictures in the Graphics instance */
            std::string m_borders[(unsigned short)Last];
            Widget* m_in;
            float m_border;
            std::string m_bg;
            bool m_strict;

            /* Internal functions */
            void updateSize();
    };
}

#endif

