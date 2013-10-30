
#ifndef DEF_GUI_INTERNAL_ITEM
#define DEF_GUI_INTERNAL_ITEM

#include "graphics/graphics.hpp"
#include <string>

namespace gui
{
    namespace internal
    {
        class Item
        {
            public:
                Item() = delete;
                Item(const Item*) = delete;
                Item(graphics::Graphics* gfx);
                ~Item();

                /* Text management */
                std::string text(const std::string& t);
                std::string text() const;

                /* Size */
                /* The returned isze is the effective one, which can be bigger than the one setted is selected */
                float width(float w);
                float height(float h);
                float width() const;
                float height() const;

                /* Selection */
                bool select(bool s);
                bool selected() const;
                bool scrollLeft();
                bool scrollRight();

                /* Will draw the item around the actual origin */
                void draw();

                /* The textures to be used */
                enum Part : unsigned short {
                    Left,
                    Right,
                    Middle,
                    Font,
                    Last
                };
                void setPart(Part p, bool state, const std::string& path);

            private:
                graphics::Graphics* m_gfx;
                float m_width, m_height;
                /* When selected, the effective size is +10% of setted size */
                bool m_selected;
                std::string m_text; /* The full text to be printed */
                size_t m_lbound, m_rbound; /* Indexes of right/left part of the text */
                bool m_lext, m_rext; /* Can be extended to right/left */
                std::string m_texts[2][(unsigned short)Last];

                /* Internal functions */
                float widthE() const;
                float heightE() const;
                void updateState(bool restart = true);
        };
    }
}

#endif

