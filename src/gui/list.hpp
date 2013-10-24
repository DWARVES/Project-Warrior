
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
    class List : public Widget
    {
        /* TODO setting textures to items */
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
            typedef internal::Item* ItemID;
            /* Returns the unique id of the new item */
            ItemID addItem(size_t pos, const std::string& text, float offx);
            void setItem(ItemID id, const std::string& text);
            void removeItem(ItemID id);
            /* Returns 0 if the item doesn't exists */
            ItemID item(size_t pos);
            size_t selected() const;
            std::string selectedText() const;
            ItemID selectedID() const;
            void setItemSize(const geometry::AABB& r);

            /* Drawing */
            virtual void draw();

            /* Events */
            virtual void focus(bool f);
            virtual void click(const geometry::Point& p);
            virtual void keyPress(events::Key k);
            virtual bool next();

        private:
            struct StoredItem {
                internal::Item* it;
                float offx;
            };
            std::vector<StoredItem> m_items;
            geometry::AABB m_itemSize;
            float m_sep;
            size_t m_selected;
            bool m_rolling;
            /* If rolling, the bounds */
            size_t m_upb;
            size_t m_downb;

            /* Internal functions */
            size_t posFromID(ItemID id);
            void updateState();
            void deleteItem(size_t pos);
    };
}

#endif

