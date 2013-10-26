
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
            ItemID addItem(size_t pos, const std::string& text, float offx = 0.0f);
            void setItem(ItemID id, const std::string& text);
            void removeItem(ItemID id);
            /* Returns 0 if the item doesn't exists */
            ItemID item(size_t pos);
            size_t selected() const;
            std::string selectedText() const;
            ItemID selectedID() const;
            void setItemSize(const geometry::AABB& r);

            /* Textures */
            enum Part : unsigned short {
                Right  = internal::Item::Right,
                Left   = internal::Item::Left,
                Middle = internal::Item::Middle,
                Font   = internal::Item::Font,
                Last   = internal::Item::Last
            };
            void setPart(Part p, bool sel, const std::string& path);

            /* Drawing */
            virtual void draw();

            /* Events */
            virtual void focus(bool f);
            virtual void click(const geometry::Point& p);
            virtual void keyPress(events::Key k);
            virtual bool next();
            /* Won't do anything, it's called at every change of selected.
             * Is usefull when derivating gui::List
             */
            virtual void select();

        private:
            struct StoredItem {
                internal::Item* it;
                float offx;
            };
            std::vector<StoredItem> m_items;
            geometry::AABB m_itemSize;
            float m_sep;
            size_t m_selected;
            size_t m_lselected;
            bool m_rolling;
            /* If rolling, the bounds */
            size_t m_upb;
            size_t m_downb;

            /* Textures */
            std::string m_texts[2][(unsigned short)Last];

            /* Internal functions */
            size_t posFromID(ItemID id);
            void updateState();
            void deleteItem(size_t pos);
            void flushTexts();
            void prev();
    };
}

#endif

