
#ifndef DEF_GUI_WIDGET
#define DEF_GUI_WIDGET

#include "graphics/graphics.hpp"
#include "geometry/point.hpp"
#include <string>

namespace gui
{
    /** @brief Pure virtual class representing an element of the gui (a widget). */
    class Widget
    {
        public:
            Widget() = delete;
            Widget(const Widget&) = delete;
            /** @brief Every widgets needs an instance of graphics::Graphics to draw themselves. */
            Widget(graphics::Graphics* gfx);
            virtual ~Widget();

            /** @brief Set width of the widget. */
            virtual float width(float w);
            /** @brief Set height of the widget. */
            virtual float height(float h);
            /** @brief Get width of the widget. */
            virtual float width() const;
            /** @brief Get height of the widget. */
            virtual float height() const;

            /** @brief Draw the widget.
             *
             * Before call, the origin is placed at the top-left corner of the widget.
             * This call shouldn't leave the origin changed.
             */
            virtual void draw() = 0;

            /** @brief Events handled by widgets. */
            enum Action {
                First,       /**< @brief Request to go to the beginning/first part. */
                Last,        /**< @brief Request to go to the end/last part. */
                ScrollLeft,  /**< @brief Request to go to the left. */
                ScrollRight, /**< @brief Request to go to the right. */
                ScrollUp,    /**< @brief Request to go upward. */
                ScrollDown,  /**< @brief Request to go downward. */
                Select,      /**< @brief Request selection from the widget. */
                Remove,      /**< @brief Request removal from the widget. */
            };

            /** @brief Change the focus state of the widget. */
            virtual void focus(bool);
            /** @brief Text typed while the widget was focused. */
            virtual void inputText(const std::string&);
            /** @brief Widget::Action sended to the widget.
             * @return True if the action was handled. */
            virtual bool action(Action);

        protected:
            /** @brief The graphics::Graphics instance used by the widget. */
            graphics::Graphics* m_gfx;

        private:
            /* PRIVATE use accessors to access */
            float m_width, m_height;

    };
}

#endif

