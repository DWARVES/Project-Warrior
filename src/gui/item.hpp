
#ifndef DEF_GUI_INTERNAL_ITEM
#define DEF_GUI_INTERNAL_ITEM

#include "graphics/graphics.hpp"
#include <string>
#include <SDL.h>

namespace gui
{
    /** @brief Contains class and functions internally used by gui namespace classes. */
    namespace internal
    {
        /** @brief It represents an item, which mean a single framed text (with text scrolling). */
        class Item
        {
            public:
                Item() = delete;
                Item(const Item*) = delete;
                Item(graphics::Graphics* gfx);
                ~Item();

                /* Text management */
                /** @brief Set the text. */
                std::string text(const std::string& t);
                /** @brief Get the text. */
                std::string text() const;

                /* Size */
                /** @brief Enable resizing when selected : if true, the size will be aumented by 10% when selected. */
                void resize(bool r);
                /** @brief Set the width.
                 * @return The effective width, which may be 10% bigger than the one setted if the item is selected and resizing is enabled.
                 */
                float width(float w);
                /** @brief Set the height.
                 * @return The effective height, which may be 10% bigger than the one setted if the item is selected and resizing is enabled.
                 */
                float height(float h);
                /** @brief Get the effective width. */
                float width() const;
                /** @brief Get the effective height. */
                float height() const;

                /* Selection */
                /** @brief Select the item. */
                bool select(bool s);
                /** @brief Is the item selected. */
                bool selected() const;
                /** @brief Scroll the text to the left. */
                bool scrollLeft();
                /** @brief Scroll the text to the right. */
                bool scrollRight();

                /** @brief Draw the item around the actual origin. */
                void draw();

                /** @brief The textures used. */
                enum Part : unsigned short {
                    Left,   /**< @brief The left part. */
                    Right,  /**< @brief The right part. */
                    Middle, /**< @brief The middle part. */
                    Font,   /**< @brief The font used. */
                    Last    /**< @brief The number of textures, for internal use. */
                };
                /** @brief Define a texture to be used.
                 * @param state Is this texture a selected one.
                 */
                void setPart(Part p, bool state, const std::string& path);

            private:
                graphics::Graphics* m_gfx; /**< @brief The graphics::Graphics instance used. */
                float m_width,             /**< @brief The setted width of the item. */
                      m_height;            /**< @brief The setted height of the item. */
                bool m_selected;           /**< @brief Is the item selected. */
                std::string m_text;        /**< @brief The text of the item. */
                size_t m_lbound,           /**< @brief The index of the left letter of the shown text in m_text. */
                       m_rbound;           /**< @brief The index of the right letter of the shown text in m_text. */
                bool m_lext,               /**< @brief Is the text scrollable to the left. */
                     m_rext;               /**< @brief Is the text scrollable to the right. */
                std::string m_texts[2][(unsigned short)Last]; /**< @brief The textures [selected][part]. */
                bool m_resize;             /**< @brief Is the resizing mode enabled. */

                /** @brief Time when it got selected the last time, for automatic scrolling. */
                Uint32 m_lastSel;

                /* Internal functions */
                /** @brief Compute the effective width, taking in account the resizing mode. */
                float widthE() const;
                /** @brief Compute the effective update, taking in account the resizing mode. */
                float heightE() const;
                /** @brief Update all members.
                 * @param restart If true, the text is put to its beggining.
                 */
                void updateState(bool restart = true);
        };
    }
}

#endif

