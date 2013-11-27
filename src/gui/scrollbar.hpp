
#ifndef DEF_GUI_SCROLLBAR
#define DEF_GUI_SCROLLBAR

#include "gui/widget.hpp"
#include "graphics/graphics.hpp"
#include <string>

namespace gui
{
    /** @brief Print a scrollbar. */
    class ScrollBar : public Widget
    {
        public:
            ScrollBar() = delete;
            ScrollBar(const ScrollBar&) = delete;
            ScrollBar(graphics::Graphics* gfx);
            virtual ~ScrollBar();

            /* Parts */
            /** @brief Sets the total size of the scrollbar. */
            float size(float s);
            /** @brief Returns the total size of the scrollbar. */
            float size() const;
            /** @brief Set the selected part.
             * @param p The top pos of the selection.
             * @param p The size of the selection.
             */
            void selected(float p, float s);
            /** @brief Change the position of the selection. */
            void moveTo(float p);
            /** @brief Returns the position of the selection. */
            float selPos() const;
            /** @brief Returns the size of the selection. */
            float selSize() const;
            /** @brief Set the ratio when drawing : r = height/width. */
            float ratio(float r);
            /** @brief Returns the ratio used when drawing. */
            float ratio() const;
            /** @brief The steps when in interactive mode. */
            float step(float s);
            /** @brief The steps when in interactive mode. */
            float step() const;
            /** @brief Enbale or disable the interactive mode : if interactive, the pos can be changed with Widget::Action. */
            bool interactive(bool i);
            /** @brief Is the interactive mode set. */
            bool interactive() const;
            /** @brief Set the direction : if v=true, it will be drawn vertically. */
            bool dir(bool v);
            /** @brief Indicate if it is drawn vertically. */
            bool vert() const;
            /** @brief Returns the percentage of the position, from 0 to 100. */
            float percent() const;

            /** @brief The textures used. */
            enum Text : unsigned short {
                Up,       /**< @brief The texture of the up part. */
                Down,     /**< @brief The texture of the down part. */
                Norm,     /**< @brief The texture of the middle part, not selected. */
                Selected, /**< @brief The texture of the middle part, when selected. */
                Last      /**< @brief Internally used. */
            };
            /** @brief Sets a texture.
             * @param t The texture.
             * @param f If the texture is for focusing mode.
             * @param path The path of the texture in the graphics::Graphics instance.
             */
            void setTexture(Text t, bool f, const std::string& path);

            /* Drawing */
            virtual void draw();

            /* Events */
            virtual void focus(bool f);
            virtual bool action(Widget::Action a);

        private:
            /** @brief The total size. */
            float m_size;
            /** @brief The size of the selection. */
            float m_selSize;
            /** @brief The pos of the selection. */
            float m_selPos;
            /** @brief The ratio height/width. */
            float m_ratio;
            /** @brief The step when dynamicly changing value. */
            float m_step;
            /** @brief Is in interact mode. */
            bool m_interact;
            /** @brief Must the scorebar be drawn vertically. */
            bool m_vert;
            /** @brief The index of the texture to be used, depending on the focus. */
            unsigned int m_focus;
            /** @brief The textures, [focus][part] */
            std::string m_texts[2][(unsigned short)Last];
    };
}

#endif

