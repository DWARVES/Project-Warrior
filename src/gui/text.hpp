
#ifndef DEF_GUI_TEXT
#define DEF_GUI_TEXT

#include "widget.hpp"
#include <string>

namespace gui
{
    /** @brief Show a text. */
    class Text : public Widget
    {
        public:
            Text() = delete;
            Text(const Text&) = delete;
            Text(graphics::Graphics* gfx);
            virtual ~Text();

            /* Text */
            /** @brief Set the text. May lag if the text is long. */
            void setText(const std::string& txt);
            /** @brief Add text to be printed. More optimised than setText. */
            void addText(const std::string& txt);
            /** @brief Get the actual text. */
            std::string getText() const;

            /* Size */
            virtual float width(float w);
            virtual float height(float h);
            virtual float width() const;
            virtual float height() const;

            /* Infos (can be used to prompt a srollBar) */
            /** @brief The total height. */
            float totalHeight() const;
            /** @brief The top of the shown text. */
            float topPos() const;
            /** @brief The height of the shown text. */
            float shownHeight() const;

            /* Drawing */
            /** @brief Set the font used. */
            void setFont(const std::string& f, float size = -1.0f);
            virtual void draw();

            /* Action */
            virtual bool action(Widget::Action a);

        private:
            std::string m_txt;                /**< @brief The raw text. */
            std::vector<std::string> m_lines; /**< @brief The lines. */
            size_t m_begin;                   /**< @brief The idx of the first line shown. */
            size_t m_size;                    /**< @brief The number of lines shown. */
            std::string m_font;               /**< @brief The font used. */
            float m_pts;                      /**< @brief The size of the font. */

            /* Internal methods */
            /** @brief Compute the number of lines that can be shown in the height of the widget. */
            void computeSize();
            /** @brief Get the text shown. */
            std::string getPrinted();
            /** @brief Shrink all the lines to make sure they doesn't exceed the width.
             * @param from The first line to shrink.
             * @todo If a word exceed the width of the line it won't be cutted.
             */
            void shrinkLines(size_t from = 0);
            /** @brief Cut a line in multiple lines, prevent exceeding w. */
            std::vector<std::string> shrinkLine(const std::string& line, float w);
            /** @brief Cut txt at its newlines. */
            std::vector<std::string> cutToReturn(const std::string& txt);
    };
}

#endif

