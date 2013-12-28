
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
             * @param data The user data for this item.
             * @return The unique id of the new item.
             */
            ItemID addItem(size_t pos, const std::string& text, float offx = 0.0f, void* data = NULL);
            /** @brief Change the text of an item. */
            void setItem(ItemID id, const std::string& text);
            /** @brief Change the data associated to an item. */
            void setData(ItemID id, void* data);
            /** @brief Get the data associated with an item. */
            void* getData(ItemID id) const;
            /** @brief Remove an item form the list, id will no longer be valid. */
            void removeItem(ItemID id);
            /** @brief Removes all the items. */
            void clear();
            /** @brief Free and nullify all non NULL data with fr. */
            template <typename FR> void freeData(FR fr);
            /** @brief Returns the id of the item at a given pos, 0 if pos is outside the range. */
            ItemID item(size_t pos);
            /** @brief Returns the position of the selected item. */
            size_t selected() const;
            /** @brief Returns the text of the selected item. */
            std::string selectedText() const;
            /** @brief Returns the id of the selected item. */
            ItemID selectedID() const;
            /** @brief Returns the data of the selected item. */
            void* selectedData() const;
            /** @brief Set the size of the items.
             * Its width is relative to list width, will the height is used as it is.
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
            /** @brief State in which the textures have to be used. */
            enum State : unsigned short {
                Selected = internal::Item::Selected, /**< @brief Textures have to be used when selected. */
                Focused  = internal::Item::Focused,  /**< @brief Textures have to be used when focused. */
                Norm     = internal::Item::Norm,     /**< @brief Textures have to be used when neither selected nor focused. */
                NB       = internal::Item::NB,       /**< @brief The number of states, for internal use. */
            };
            /** @brief Set the textures to use.
             * @param sel Is the texture for the selected item.
             */
            void setPart(Part p, State st, const std::string& path);

            /* Drawing */
            virtual void draw();

            /* Events */
            virtual bool action(Widget::Action a);
            virtual void focus(bool f);
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
                void* data;         /**< @brief User data of the item. */
            };
            std::vector<StoredItem> m_items; /**< @brief The items. */
            geometry::AABB m_itemSize;       /**< @brief The size of items. */
            float m_sep;                     /**< @brief The size between each item. */
            size_t m_selected;               /**< @brief The index of the selected item. */
            size_t m_lselected;              /**< @brief The index of the previoously selected item. */
            bool m_rolling;                  /**< @brief Is it in rolling mode : can be scrolled because all items don't fit in the list height. */
            size_t m_upb;                    /**< @brief The index of the first shown item. */
            size_t m_downb;                  /**< @brief The index of the last shown item. */
            bool m_focused;                  /**< @brief The list is focused. */

            std::string m_texts[(unsigned short)NB][(unsigned short)Last]; /**< @brief The textures [state][part]. */

            /* Internal functions */
            /** @brief Get the pos of an item. */
            size_t posFromID(ItemID id) const;
            /** @brief Compute everything needed and update internal members. */
            void updateState();
            /** @brief Remove an item at a given pos. */
            void deleteItem(size_t pos);
            /** @brief Send the texture to the items. */
            void flushTexts();
            /** @brief Select the previous item. */
            bool prev();
            /** @brief Select the next item. */
            bool next();
    };

    /* Template methods */
    template <typename FR> void List::freeData(FR fr)
    {
        for(StoredItem st : m_items) {
            if(st.data)
                fr(st.data);
            st.data = NULL;
        }
    }
}

#endif

