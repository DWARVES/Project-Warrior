
#ifndef DEF_GUI_FILLBAR
#define DEF_GUI_FILLBAR

#include "widget.hpp"
#include <string>

namespace gui
{
    /** @brief A widget represinting a fill bar. */
    class FillBar : public Widget
    {
        public:
            FillBar() = delete;
            FillBar(const FillBar&) = delete;
            FillBar(graphics::Graphics* gfx);
            virtual ~FillBar();

            /* Values */
            /** @brief Set the total size. */
            float size(float s);
            /** @brief Get the total size. */
            float size() const;
            /** @brief Set the filled size. */
            float set(float f);
            /** @brief Get the filled size. */
            float get() const;
            /** @brief Get the filled percent. */
            float percent() const;
            /** @brief Set the steps used in interactive mode. */
            float step(float s);
            /** @brief Get the steps used in interactive mode. */
            float step() const;
            /** @brief Set the ratio, height/width. */
            float ratio(float r);
            /** @brief Get the ratio. */
            float ratio() const;
            /** @brief Set the direction, true meaning vertical. */
            bool dir(bool v);
            /** @brief Is the bar drawn vertically. */
            bool vert() const;
            /** @brief Set the interactive mode, if false, al Widget::Action are ignored. */
            bool interactive(bool i);
            /** @brief Is in interactive mode. */
            bool interactive() const;

            /* Textures */
            /** @brief Set the textures used.
             * @param f Are the texture to be used when focused.
             * @param empty The texture of the empty fillbar.
             * @param filled The texture of the filled fillbar.
             */
            void setTexture(bool f, const std::string& empty, const std::string& filled);

            /* Drawing */
            virtual void draw();

            /* Events */
            virtual void focus(bool f);
            virtual bool action(Widget::Action a);

        private:
            unsigned int m_focus;      /** @brief The index of the focused texture. */
            bool m_vert;               /** @brief Must it be drawn vertically. */
            bool m_interact;           /** @brief Is the interactive mode enabled. */
            float m_size;              /** @brief The total size. */
            float m_filled;            /** @brief The size of the filled part. */
            float m_step;              /** @brief The step used in interactive mode. */
            float m_ratio;             /** @brief The ration, height/width. */
            std::string m_texts[2][2]; /** @brief The textures, [focus][empty/filled]. */
    };
}

#endif

