
#ifndef DEF_GUI_GRIDLAYOUT
#define DEF_GUI_GRIDLAYOUT

#include "widget.hpp"
#include "graphics/graphics.hpp"
#include <vector>

namespace gui
{
    /** @brief A grid layout : organize widgets along a grid. */
    class GridLayout : public Widget
    {
        public:
            GridLayout() = delete;
            GridLayout(const GridLayout&) = delete;
            GridLayout(graphics::Graphics* gfx);
            GridLayout(graphics::Graphics* gfx, unsigned int mcolumns, unsigned int mrows);
            virtual ~GridLayout();

            /** @brief Change the size of the layout. Will clear it.
             * @param mcolumns The number of columns.
             * @param mrows The number of rows.
             */
            void setSize(unsigned int mcolumns, unsigned int mrows);
            /** @brief Get the number of rows. */
            unsigned int rows() const;
            /** @brief Get the number of columns. */
            unsigned int columns() const;
            /** @brief Set the gap between each widget. */
            float gaps(float g);
            /** @brief get the gap between each widget. */
            float gaps() const;
            virtual float width(float w);
            virtual float height(float h);
            virtual float width() const;
            virtual float height() const;

            /** @brief Add a widget to the grid.
             * 
             * It will fail if one the cases is already taken, of outside range.
             * @param widget The widget to add.
             * @param x The x position in the grid from top-left.
             * @param y The y position in the grid from top-left.
             * @param w The width of the widget in number of cases. You must add one to know the real amount of columns used (0 means a width of 1).
             * @param w The height of the widget in number of cases. You must add one to know the real amount of rows used (0 means a height of 1).
             * @return False if something appened.
             */
            bool addWidget(Widget* widget, unsigned int x, unsigned int y, unsigned int w = 0, unsigned int h = 0);
            /** @brief Check the existence of a widget at a given position. If outside range, it returns true. */
            bool isWidget(unsigned int x, unsigned int y) const;
            /** @brief Remove all the widgets, but don't change the size. */
            void clear();
            /** @brief Remove a widget at a given position. */
            bool removeWidget(unsigned int x, unsigned int y);

            virtual void draw();

            virtual void focus(bool f);
            virtual void inputText(const std::string& str);
            virtual bool action(Widget::Action a);

        private:
            unsigned int m_rows,    /**< @brief Number of rows. */
                         m_columns; /**< @brief Number of columns. */
            float m_gaps;           /**< @brief Gap between widgets. */
            /** @brief Represent a case */
            struct StoredWidget {
                Widget* widget;  /**< @brief The widget in the case, if NULL there is no one. */
                unsigned int rx; /**< @brief The x top-left position of the widget in the grid. */
                unsigned int ry; /**< @brief The y top-left position of the widget in the grid. */
                unsigned int w;  /**< @brief The width of the widget in cases, minus 1. */
                unsigned int h;  /**< @brief The height of the widget in cases, minus 1. */
            };
            std::vector<std::vector<StoredWidget>> m_map; /**< @brief The map, an array of rows. */
            StoredWidget m_focused;                       /**< @brief The focused widget. */

            /* Internal functions */
            /** @brief Set pos to the position in pixels of the case of index (x;y). */
            void getPos(unsigned int x, unsigned int y, geometry::Point& pos);
            /** @brief Compute the size of the widgets in the grid. */
            void updateSizes();
            /** @brief Return the width of a case. */
            float caseWidth(bool wg);
            /** @brief Return the height of a case. */
            float caseHeight(bool wg);
            /** @brief Move the selection to the left. */
            bool left();
            /** @brief Move the selection to the right. */
            bool right();
            /** @brief Move the selection upward. */
            bool up();
            /** @brief Move the selection downward. */
            bool down();
    };
}

#endif

