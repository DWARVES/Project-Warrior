
#ifndef DEF_GUI_FRAME
#define DEF_GUI_FRAME

#include "gui/widget.hpp"
#include "graphics/graphics.hpp"
#include <string>

namespace gui
{
    /** @brief A layout that draw a frame around a widget. */
    class Frame : public Widget
    {
        public:
            Frame() = delete;
            Frame(const Frame&) = delete;
            Frame(graphics::Graphics* gfx);
            Frame(graphics::Graphics* gfx, Widget* in);
            ~Frame();

            /* Size */
            virtual float width(float w);
            virtual float height(float h);
            virtual float width() const;
            virtual float height() const;
            /** @brief The the width of the border of the frame. */
            float border(float size);
            /** @brief Get the width of the border of the frame. */
            float border() const;

            /* Pictures */
            /** @brief The parts of the frame. */
            enum Border : unsigned short {
                TopLeft,     /**< @brief The top left part. */
                TopRight,    /**< @brief The top right part. */
                BottomLeft,  /**< @brief The bottom left part. */
                BottomRight, /**< @brief The bottom right part. */
                Top,         /**< @brief The top part. */
                Left,        /**< @brief The left part. */
                Bottom,      /**< @brief The bottom part. */
                Right,       /**< @brief The right part. */
                Last,        /**< @brief The number of parts, for internal use. */
            };
            /** @brief Set the texture of a part. */
            void set(Border bd, const std::string& value);

            /* Background */
            /** @brief Set the background texture.
             * @param strict If false, the background will take the whole width and height, event the borders.
             */
            void setBg(const std::string& value, bool strict = true);

            /* Widget in */
            /** @brief The the widget to put in the frame. */
            void set(Widget* in);
            /** @brief Get the widget in the frame. */
            Widget* getWidget() const;

            virtual void draw();

            /* Events */
            virtual void focus(bool f);
            virtual void inputText(const std::string& in);
            virtual bool action(Widget::Action a);

        private:
            std::string m_borders[(unsigned short)Last]; /**< @brief The textures used. */
            Widget* m_in;                                /**< @brief The widget in the frame. */
            float m_border;                              /**< @brief The width of the border. */
            std::string m_bg;                            /**< @brief The background picture. */
            bool m_strict;                               /**< @brief Must the background only take the inside of the frame, or the whole thing. */

            /* Internal functions */
            /** @brief Recompute the size of m_in. */
            void updateSize();
    };
}

#endif

