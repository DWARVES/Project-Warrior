
#ifndef DEF_GUI_BUTTON
#define DEF_GUI_BUTTON

#include "widget.hpp"
#include <string>
#include <SDL.h>

namespace gui
{
    class Button : public Widget
    {
        public:
            Button() = delete;
            Button(const Button&) = delete;
            Button(graphics::Graphics* gfx);
            virtual ~Button();

            /* Drawing */
            void setTextures(const std::string& un, const std::string& foc, const std::string& sel);
            virtual void draw();

            /* Action */
            virtual void focus(bool f);
            virtual bool action(Widget::Action a);

            virtual void select(); /* Actually do othing, must be overriden */

        private:
            int m_focus;
            std::string m_un, m_sel, m_foc;
            Uint32 m_last;
    };
}

#endif

