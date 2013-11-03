
#ifndef DEF_GUI_CHECKBOX
#define DEF_GUI_CHECKBOX

#include "widget.hpp"
#include "geometry/aabb.hpp"
#include <string>

namespace gui
{
    class CheckBox : public Widget
    {
        public:
            CheckBox() = delete;
            CheckBox(const CheckBox&) = delete;
            CheckBox(graphics::Graphics* gfx);
            virtual ~CheckBox();

            /* State */
            bool toggle();
            bool set(bool s);
            bool get() const;

            /* Textures */
            void setTexture(bool f, const std::string& empty, const std::string& selected);

            /* Drawing */
            bool square(bool s);
            bool square() const;
            void maxSize(geometry::AABB s);
            void disableMaxSize();
            virtual void draw();

            /* Actions */
            virtual void focus(bool f);
            virtual bool action(Widget::Action a);

        private:
            int m_state;
            int m_focus;
            bool m_square;
            geometry::AABB m_maxSize;
            bool m_useSize;
            std::string m_texts[2][2];
    };
}

#endif

