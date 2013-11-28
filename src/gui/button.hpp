
#ifndef DEF_GUI_BUTTON
#define DEF_GUI_BUTTON

#include "widget.hpp"
#include "item.hpp"
#include "geometry/aabb.hpp"
#include <string>
#include <SDL.h>

namespace gui
{
    /** @brief A button widget. */
    class Button : public Widget
    {
        public:
            Button() = delete;
            Button(const Button&) = delete;
            Button(graphics::Graphics* gfx);
            virtual ~Button();

            /** @brief Set the text of the button. */
            void text(const std::string& t);
            /** @brief Get the text of the button. */
            std::string text() const;
            /** @brief Set a maximum size for the button. */
            void maxSize(const geometry::AABB& msize);

            /* Size */
            float width(float w);
            float height(float h);
            float width() const;
            float height() const;

            /* Drawing */
            /** @brief The different parts of the texture. */
            enum Texture : unsigned short {
                Left   = internal::Item::Left,   /**< @brief The left part. */
                Right  = internal::Item::Right,  /**< @brief The right part. */
                Middle = internal::Item::Middle, /**< @brief The middle part. */
                Font   = internal::Item::Font,   /**< @brief The font for the text. */
                Last   = internal::Item::Last    /**< @brief Number of parts, used internally. */
            };
            /** @brief Set a texture to use when not clicked.
             * @param f Is this texture to be used when focused.
             */
            void setTexture(Texture p, bool f, const std::string& name);
            /** @brief Set a texture to be used when clicked. */
            void setSel(Texture p, const std::string& name);
            virtual void draw();

            /* Action */
            virtual void focus(bool f);
            virtual bool action(Widget::Action a);

            /** @brief It's called every time the button is clicked, but do nothing. It's useful when overriding it. */
            virtual void select();

        private:
            internal::Item m_it;                          /**< @brief Used to draw the button. */
            Uint32 m_last;                                /**< @brief The last time it was clicked. */
            bool m_focus;                                 /**< @brief Is the button focused. */
            bool m_changed;                               /**< @brief Must the focus texture be sent to m_it. */
            geometry::AABB m_msize;                       /**< @brief The maximum size to use. */
            std::string m_texts[2][(unsigned short)Last]; /**< @brief The textures [clicked][part]. */

            /* Internal functions */
            void setSel(); /**< @brief Mark it as clicked and send the clicked texture to m_it. */
            void setFoc(); /**< @brief Mark it as focused and send the focused textures to m_it if necessary. */
    };
}

#endif

