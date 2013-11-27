
#ifndef DEF_GUI_LIST
#define DEF_GUI_LIST

#include "gui/widget.hpp"
#include "gui/item.hpp"
#include "graphics/graphics.hpp"
#include "geometry/point.hpp"
#include "geometry/aabb.hpp"
#include "events/key.hpp"
#include <vector>

namespace gui
{
    /** @brief A list of elements. */
    class List : public Widget
    {
        public:
            List() = delete;
            List(const List&) = delete;
            List(graphics::Graphics* gfx);
            virtual ~List();

            /* Size */
            virtual float width(float w);
            virtual float height(float h);
            virtual float width() const;
            virtual float height() const;

            /* Items */
            /** @brief Define the id of an item of the list, hide the use of internal::Item. */
            typedef internal::Item* ItemID;
            /** @brief Add an item.
             * @param pos The position at which the item will be included.
             * @param text The text of the new item.
             * @param offs An offset along the x axis.
             * @return The unique id of the new item.
             */
            ItemID addItem(size_t pos, const std::string& text, float offx = 0.0f);
            /** @brief Change the text of an item. */
            void setItem(ItemID id, const std::string& text);
            /** @brief Remove an item form the list, id will no longer be valid. */
            void removeItem(ItemID id);
            /** @brief Returns the id of the item at a given pos, 0 if pos is outside the range. */
            ItemID item(size_t pos);
            /** @brief Returns the position of the selected item. */
            size_t selected() const;
            /** @brief Returns the text of the selected item. */
            std::string selectedText() const;
            /** @brief Returns the id of the selected item. */
            ItemID selectedID() const;
            /** @brief Set the size of the items.
             * @todo make it relative to list size.
             */
            void setItemSize(const geometry::AABB& r);

            /** @brief Textures used when drawing items. */
            enum Part : unsigned short {
                Right  = internal::Item::Right,  /**< @brief The right part of an item. */
                Left   = internal::Item::Left,   /**< @brief The left part of an item. */
                Middle = internal::Item::Middle, /**< @brief The middle part of an item. */
                Font   = internal::Item::Font,   /**< @brief The font of the items. */
                Last   = internal::Item::Last    /**< @brief Number of parts, only for internal use. */
            };
            /** @brief Set the textures to use.
             * @param sel Is the texture for the selected item.
             */
            void setPart(Part p, bool sel, const std::string& path);

            /* Drawing */
            virtual void draw();

            /* Events */
            virtual bool action(Widget::Action a);
            /** @brief Don't do anything, it's called at every change of selected.
             * It is usefull when derivating gui::List
             */
            virtual void select();
            /** @brief The same as select, but it's called when the selected item is entered. */
            virtual void enter();

        private:
            /** @brief The type of an item of the list. */
            struct StoredItem {
                internal::Item* it; /**< @brief The draw item. */
                float offx;         /**< @brief The offset along the x axis of the item. */
            };
            std::vector<StoredItem> m_items; /**< @brief The items. */
            geometry::AABB m_itemSize;       /**< @brief The size of items. */
            float m_sep;                     /**< @brief The size between each item. */
            size_t m_selected;               /**< @brief The index of the selected item. */
            size_t m_lselected;              /**< @brief The index of the previoously selected item. */
            bool m_rolling;                  /**< @brief Is it in rolling mode : can be scrolled because all items don't fit in the list height. */
            size_t m_upb;                    /**< @brief The index of the first shown item. */
            size_t m_downb;                  /**< @brief The index of the last shown item. */

            std::string m_texts[2][(unsigned short)Last]; /**< @brief The textures [selected][part]. */

            /* Internal functions */
            /** @brief Get the pos of an item. */
            size_t posFromID(ItemID id);
            /** @brief Compute everything needed and update internal members. */
            void updateState();
            /** @brief Remove an tem at a given pos. */
            void deleteItem(size_t pos);
            /** @brief Send the texture to the items. */
            void flushTexts();
            /** @brief Select the previous item. */
            bool prev();
            /** @brief Select the next item. */
            bool next();
    };
}

#endif

