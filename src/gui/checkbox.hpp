
#ifndef DEF_GUI_CHECKBOX
#define DEF_GUI_CHECKBOX

#include "widget.hpp"
#include "geometry/aabb.hpp"
#include <string>

namespace gui
{
    /** @brief A check box widget. */
    class CheckBox : public Widget
    {
        public:
            CheckBox() = delete;
            CheckBox(const CheckBox&) = delete;
            CheckBox(graphics::Graphics* gfx);
            virtual ~CheckBox();

            /* Size */
            virtual float width(float w);
            virtual float height(float h);
            virtual float width() const;
            virtual float height() const;

            /* State */
            /** @brief Toggle selection of the checkbox. */
            bool toggle();
            /** @brief Set the selection of the checkbox. */
            bool set(bool s);
            /** @brief Returns the selection of the checkbox. */
            bool get() const;
            /** @brief Set the label of the checkbox. */
            void label(const std::string& lab);
            /** @brief Get the label of the checkbox. */
            std::string label() const;

            /* Textures */
            /** @brief Set the textures used.
             * @param f Textures for focus or not.
             * @param empty The texture used when not selected.
             * @param selected The texture used when selected.
             */
            void setTexture(bool f, const std::string& empty, const std::string& selected);
            /** @brief Set the font to be used.
             * @param f Font for focus or not.
             * @param font The font to use.
             */
            void setFont(bool f, const std::string& font);

            /* Drawing */
            /** @brief Set a maximum size for the checkbox. */
            void maxSize(float s);
            /** @brief Disable the use of a max size. */
            void disableMaxSize();
            virtual void draw();

            /* Actions */
            virtual void focus(bool f);
            virtual bool action(Widget::Action a);

        private:
            int m_state;               /**< @brief Is it selected or not. */
            int m_focus;               /**< @brief Is it focused or not. */
            float m_maxSize;           /**< @brief The max size. */
            bool m_useSize;            /**< @brief Is the max size used. */
            std::string m_texts[2][2]; /**< @brief The textures, [focus][state]. */
            std::string m_fonts[2];    /**< @brief The fonts, [focus]. */
            std::string m_label;       /**< @brief The label. */
            float m_fontSize;          /**< @brief The size of the font. */
            float m_size;              /**< @brief The size of the checkbox. */
            geometry::Point m_offset;  /**< @brief The offset when drawing the checkbox. */

            /** @brief Update m_fontSize, m_size and m_offset. */
            void updateSizes();
    };
}

#endif

