
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
            /* More optimised than setText when adding text */
            void addText(const std::string& txt);
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
            std::vector<std::string> m_lines;
            size_t m_begin;
            size_t m_size;
            std::string m_font;
            float m_pts;

            /* Internal methods */
            void computeSize();
            std::string getPrinted();
            void shrinkLines(size_t from = 0);
            std::vector<std::string> shrinkLine(const std::string& line, float w);
            std::vector<std::string> cutToReturn(const std::string& txt);
    };
}

#endif

