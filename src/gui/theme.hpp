
#ifndef DEF_GUI_THEME
#define DEF_GUI_THEME

#include <string>
#include "fillbar.hpp"
#include "checkbox.hpp"
#include "gridlayout.hpp"
#include "list.hpp"
#include "scrollbar.hpp"
#include "text.hpp"
#include "input.hpp"
#include "frame.hpp"
#include "button.hpp"
#include "radio.hpp"
#include "graphics/graphics.hpp"
#include "core/fakefs.hpp"

namespace gui
{
    /** @brief Load and apply a theme to widgets. */
    class Theme
    {
        public:
            Theme() = delete;
            Theme(const Theme&) = delete;
            /** @brief Only one constructor. Prepare the class, but doesn't load anything.
             * @param gfx The graphics::Graphics instance used to load things. Everything will be set in the Theme::guiNamespace namespace.
             * @param path The path to the theme directory.
             */
            Theme(graphics::Graphics* gfx, const std::string& path);
            /** @brief Will load everything necessary. Returns false if an error happen. */
            bool load();
            ~Theme();

            /**
             * @name Applying theme to widgets.
             * @brief Apply the theme to the widget passed in parameter.
             * @{
             */
            void apply(FillBar* fb);
            void apply(CheckBox* cb);
            void apply(GridLayout* gl);
            void apply(List* l);
            void apply(ScrollBar* sb);
            void apply(Text* t);
            void apply(Input* in);
            void apply(Frame* fr);
            void apply(Button* b);
            void apply(Radio* r);
            /**
             * @}
             */

            /** @brief Indicates which namespace in the graphics instance is used by the theme to store textures. Should be /gui. */
            void guiNamespace();

        private:
            graphics::Graphics* m_gfx;  /**< @brief The graphics::Graphics instance used to load and store textures and fonts. */
            core::FakeFS<float> m_data; /**< @brief Structure used to store and manage data which isn't neither texture nor font. */
            std::string m_path;         /**< @brief The path to the gui theme directory. */

            bool m_cbMs; /**< @brief Uses maxSize for checkbox.
                           * @todo Should be stored into m_data. */
            bool m_rdMs; /**< @brief Uses maxSize for radio button.
                           * @todo Should be stored into m_data. */

            /* Internal functions */
            /** @brief Checks if what is loaded into m_data is correct and complete. */
            bool checkData();
    };
}

#endif

