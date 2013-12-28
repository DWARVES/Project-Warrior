
#ifndef DEF_GRAPHICSMENU
#define DEF_GRAPHICSMENU

#include "menu.hpp"
#include "gui/gridlayout.hpp"
#include "gui/checkbox.hpp"
#include "gui/list.hpp"
#include "gui/button.hpp"
#include "gui/text.hpp"

/** @brief The instance of the graphics config menu. */
class GraphicsMenu : public Menu
{
    public:
        GraphicsMenu();
        virtual ~GraphicsMenu();

        virtual bool prepare();
        virtual bool update();

    private:
        gui::List* m_res;          /**< @brief The resolutions available. */
        gui::CheckBox* m_fs;       /**< @brief Must the window be fullscreen. */
        gui::Button* m_back;       /**< @brief Get back to config menu discarding the changes. */
        gui::Button* m_apply;      /**< @brief Apply the changes. */
        gui::GridLayout* m_layout; /**< @brief The layout of the menu. */
};

#endif

