
#ifndef DEF_GUI_THEME
#define DEF_GUI_THEME

#include <string>
#include "fillbar.hpp"
#include "checkbox.hpp"
#include "gridlayout.hpp"
#include "list.hpp"
#include "scrollbar.hpp"
#include "text.hpp"
#include "input.hpp"
#include "frame.hpp"
#include "button.hpp"
#include "radio.hpp"
#include "graphics/graphics.hpp"

namespace gui
{
    class Theme
    {
        public:
            Theme() = delete;
            Theme(const Theme&) = delete;
            Theme(graphics::Graphics* gfx, const std::string& path);
            bool load(); /* Must be called only once */
            ~Theme();

            void apply(FillBar* fb);
            void apply(CheckBox* cb);
            void apply(GridLayout* gl);
            void apply(List* l);
            void apply(ScrollBar* sb);
            void apply(Text* t);
            void apply(Input* in);
            void apply(Frame* fr, bool bg = false, bool fill = true);
            void apply(Button* b);
            void apply(Radio* r);
            void guiNamespace();

        private:
            graphics::Graphics* m_gfx;
            std::string m_path;
    };
}

#endif

