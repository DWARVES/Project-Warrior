
#ifndef DEF_GUI_INPUT
#define DEF_GUI_INPUT

#include "widget.hpp"
#include "text.hpp"
#include <string>

namespace gui
{
    /** @brief A widget allowing the user to enter text. */
    class Input : public Widget
    {
        public:
            Input() = delete;
            Input(const Input&) = delete;
            Input(graphics::Graphics* gfx);
            virtual ~Input();

            virtual float width(float w);
            virtual float height(float h);
            virtual float width() const;
            virtual float height() const;

            /** @brief Set a maximum number of characters. */
            void setMaxLen(size_t len);
            /** @brief Get the actual entered text. */
            std::string getText() const;

            void setFont(const std::string& font, float size = -1.0f);
            virtual void draw();

            virtual void focus(bool f);
            virtual void inputText(const std::string& in);
            virtual bool action(Action a);

        private:
            /** @brief The maximal length of the entered text. */
            size_t m_maxLen;
            /** @brief The gui::Text used for drawing the widget. */
            Text m_txt;
    };
}

#endif

