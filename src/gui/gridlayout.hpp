
#ifndef DEF_GUI_GRIDLAYOUT
#define DEF_GUI_GRIDLAYOUT

#include "widget.hpp"
#include "graphics/graphics.hpp"
#include <vector>

namespace gui
{
    class GridLayout : public Widget
    {
        public:
            GridLayout() = delete;
            GridLayout(const GridLayout&) = delete;
            GridLayout(graphics::Graphics* gfx);
            GridLayout(graphics::Graphics* gfx, unsigned int mrows, unsigned int mcolumns);
            virtual ~GridLayout();

            /* Resizing will clear the layout */
            void setSize(unsigned int mrows, unsigned int mcolumns);
            unsigned int rows() const;
            unsigned int columns() const;

            /* You can't add a widget if the place is already taken nor outside the range */
            bool addWidget(Widget* widget, unsigned int x, unsigned int y, unsigned int w = 0, unsigned int h = 0);
            bool isWidget(unsigned int x, unsigned int y) const;
            void clear();
            bool removeWidget(unsigned int x, unsigned int y);

            virtual void draw();

            virtual void focus(bool f);
            virtual void click(const geometry::Point& p);
            virtual void mouse(const geometry::Point& p);
            virtual void keyPress(events::Key k);
            virtual void keyRelease(events::Key k);
            virtual void inputText(const std::string& str);
            virtual bool next();

        private:
            unsigned int m_rows, m_columns;
            struct StoredWidget {
                Widget* widget;
                unsigned int rx;
                unsigned int ry;
                unsigned int w;
                unsigned int h;
            };
            std::vector<std::vector<StoredWidget>> m_map;
            Widget* m_focused;
    };
}

#endif

