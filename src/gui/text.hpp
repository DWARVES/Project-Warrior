
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
            void setFont(const std::string& f, float size = -1.0f);
            virtual void draw();

            /* Action */
            virtual bool action(Widget::Action a);

        private:
            std::string m_txt;
            std::string m_fmt; /* Text modified */
            std::string m_font;
            size_t m_begin;
            size_t m_end;
            float m_size;

            /* Internal methods */
            void updateState(bool rewind = true);
            void shrinkLine(std::string& line, float w);
    };
}

#endif

