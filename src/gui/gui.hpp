
#ifndef DEF_GUI_GUI
#define DEF_GUI_GUI

#include "graphics/graphics.hpp"
#include "geometry/point.hpp"
#include "events/events.hpp"
#include "widget.hpp"

/** @brief Contains all the gui class and methods. */
namespace gui
{
    /** @brief Represents the gui and send the events to the widgets. */
    class Gui
    {
        public:
            Gui(graphics::Graphics* gfx);
            Gui() = delete;
            Gui(const Gui&) = delete;
            ~Gui();

            /** @brief Set the size, position and content of the gui.
             *
             * If m_tofree was true, the previous main widget will be deleted.
             * @param m      The main widget : it's the only widget directly printed by gui, and it takes all the place given to gui.
             * @param pos    The pos of the gui.
             * @param width  The width of the gui part.
             * @param height The height of the gui part.
             * @param tofree If true, will free the main widget on destruction.
             */
            Widget* main(Widget* m, const geometry::Point& pos, float width, float height, bool tofree = false);
            /** @brief Returns the actual main widget. */
            Widget* main() const;
            /** @brief Change the part of the window given to the gui.
             * @param p      The pos of the gui.
             * @param width  The width of the gui part.
             * @param height The height of the gui part.
             */
            void setRect(const geometry::Point& p, float width, float height);

            /** @brief Will draw everything. It must be called between Graphics::beginDraw and endDraw. */
            void draw();

            /** @brief Will update the gui with the events and send the right action (gui::Widget::Action) to the main widget. */
            void update(const events::Events& ev);
            /** @brief Sets the focus to the gui. */
            void focus(bool f);
            /** @brief Indicates if the gui has focus. */
            bool focus() const;

        private:
            bool m_tofree;             /**< @brief Indicates if the main widget must be free'd. */
            bool m_focus;              /**< @brief Indicates if the gui has focus. */
            Widget* m_main;            /**< @brief The main widget. */
            geometry::Point m_pos;     /**< @brief The ppos of the gui part. */
            graphics::Graphics* m_gfx; /**< @brief The graphics instance used. */
            std::map<std::pair<int,events::Joystick*>, std::pair<bool,bool>> m_joys; /**< @brief Is a joystick's axis inclined or not to right (first) and left (second). */
    };
}

#endif

