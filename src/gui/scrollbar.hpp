
#ifndef DEF_GUI_SCROLLBAR
#define DEF_GUI_SCROLLBAR

#include "gui/widget.hpp"
#include "graphics/graphics.hpp"
#include <string>

namespace gui
{
    class ScrollBar : public Widget
    {
        public:
            ScrollBar() = delete;
            ScrollBar(const ScrollBar&) = delete;
            ScrollBar(graphics::Graphics* gfx);
            virtual ~ScrollBar();

            /* Parts */
            float size(float s);
            float size() const;
            void selected(float p, float s);
            void moveTo(float p);
            float selPos() const;
            float selSize() const;
            float ratio(float r);
            float ratio() const;
            float step(float s);
            float step() const;
            bool interactive(bool i);
            bool interactive() const;
            bool dir(bool v);
            bool vert() const;
            /* Return the percent, from 0 to 100 */
            float percent() const;

            /* Textures */
            enum Text : unsigned short {
                Up,
                Down,
                Norm,
                Selected,
                Last
            };
            void setTexture(Text t, bool f, const std::string& path);

            /* Drawing */
            virtual void draw();

            /* Events */
            virtual void focus(bool f);
            virtual bool action(Widget::Action a);

        private:
            float m_size;
            float m_selSize;
            float m_selPos;
            float m_ratio;
            float m_step;
            bool m_interact;
            bool m_vert;
            unsigned int m_focus;
            std::string m_texts[2][(unsigned short)Last];
    };
}

#endif

