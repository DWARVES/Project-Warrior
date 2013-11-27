
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

            /* State */
            /** @brief Toggle selection of the checkbox. */
            bool toggle();
            /** @brief Set the selection of the checkbox. */
            bool set(bool s);
            /** @brief Returns the selection of the checkbox. */
            bool get() const;

            /* Textures */
            /** @brief Set the textures used.
             * @param f Textures for focus or not.
             * @param empty The texture used when not selected.
             * @param selected The texture used when selected.
             */
            void setTexture(bool f, const std::string& empty, const std::string& selected);

            /* Drawing */
            /** @brief If setted to true, the size will be the biggest square possible in the size. */
            bool square(bool s);
            /** @brief Is the checkbox drawn as a square. */
            bool square() const;
            /** @brief Set a maximum size for the checkbox. */
            void maxSize(geometry::AABB s);
            /** @brief Disable the use of a max size. */
            void disableMaxSize();
            virtual void draw();

            /* Actions */
            virtual void focus(bool f);
            virtual bool action(Widget::Action a);

        private:
            int m_state;               /**< @brief Is it selected or not. */
            int m_focus;               /**< @brief Is it focused or not. */
            bool m_square;             /**< @brief Must it be drawn as a square. */
            geometry::AABB m_maxSize;  /**< @brief The max size. */
            bool m_useSize;            /**< @brief Is the max size used. */
            std::string m_texts[2][2]; /**< @brief The textures, [focus][state]. */
    };
}

#endif

