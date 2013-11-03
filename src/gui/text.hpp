
#ifndef DEF_GUI_TEXT
#define DEF_GUI_TEXT

#include "widget.hpp"
#include <string>

namespace gui
{
    class Text : public Widget
    {
        public:
            Text() = delete;
            Text(const Text&) = delete;
            Text(graphics::Graphics* gfx);
            virtual ~Text();

            /* Text */
            void setText(const std::string& txt);
            std::string getText() const;

            /* Size */
            virtual float width(float w);
            virtual float height(float h);
            virtual float width() const;
            virtual float height() const;

            /* Infos (can be used to prompt a srollBar) */
            float totalHeight() const;
            float topPos() const;
            float shownHeight() const;

            /* Drawing */
            void setFont(const std::string& f);
            virtual void draw();

            /* Action */
            virtual bool action(Widget::Action a);

        private:
            std::string m_txt;
            std::string m_fmt; /* Text modified */
            size_t m_begin;
            std::string m_font;

            /* Internal methods */
            void updateState(bool rewind = true);
    };
}

#endif

