
#ifndef DEF_GUI_FILLBAR
#define DEF_GUI_FILLBAR

#include "widget.hpp"
#include <string>

namespace gui
{
    class FillBar : public Widget
    {
        public:
            FillBar() = delete;
            FillBar(const FillBar&) = delete;
            FillBar(graphics::Graphics* gfx);
            virtual ~FillBar();

            /* Values */
            float size(float s);
            float size() const;
            float set(float f);
            float get() const;
            float percent() const;
            float step(float s);
            float step() const;
            float ratio(float r);
            float ratio() const;
            bool dir(bool v);
            bool vert() const;
            bool interactive(bool i);
            bool interactive() const;

            /* Textures */
            void setTexture(bool f, const std::string& empty, const std::string& filled);

            /* Drawing */
            virtual void draw();

            /* Events */
            virtual void focus(bool f);
            virtual bool action(Widget::Action a);

        private:
            unsigned int m_focus;
            bool m_vert;
            bool m_interact;
            float m_size;
            float m_filled;
            float m_step;
            float m_ratio;
            std::string m_texts[2][2];
    };
}

#endif

