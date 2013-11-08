
#ifndef DEF_GUI_BUTTON
#define DEF_GUI_BUTTON

#include "widget.hpp"
#include "item.hpp"
#include "geometry/aabb.hpp"
#include <string>
#include <SDL.h>

namespace gui
{
    class Button : public Widget
    {
        public:
            Button() = delete;
            Button(const Button&) = delete;
            Button(graphics::Graphics* gfx);
            virtual ~Button();

            void text(const std::string& t);
            std::string text() const;
            void maxSize(const geometry::AABB& msize);

            /* Size */
            float width(float w);
            float height(float h);
            float width() const;
            float height() const;

            /* Drawing */
            enum Texture : unsigned short {
                Left   = internal::Item::Left,
                Right  = internal::Item::Right,
                Middle = internal::Item::Middle,
                Font   = internal::Item::Font,
                Last   = internal::Item::Last
            };
            void setTexture(Texture p, bool f, const std::string& name);
            void setSel(Texture p, const std::string& name);
            virtual void draw();

            /* Action */
            virtual void focus(bool f);
            virtual bool action(Widget::Action a);

            virtual void select(); /* Actually do othing, must be overriden */

        private:
            internal::Item m_it;
            Uint32 m_last;
            bool m_focus;
            bool m_changed;
            geometry::AABB m_msize;
            std::string m_texts[2][(unsigned short)Last];

            /* Internal functions */
            void setSel();
            void setFoc();
    };
}

#endif

