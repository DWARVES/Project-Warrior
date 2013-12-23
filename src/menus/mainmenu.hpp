
#ifndef DEF_MAINMENU
#define DEF_MAINMENU

#include "menus/menu.hpp"
#include "menus/buttonmenu.hpp"
#include "gui/gridlayout.hpp"
#include "gui/button.hpp"

/** @brief Manages the main menu.
 * @todo Play music.
 */
class MainMenu : public Menu
{
    public:
        MainMenu();
        virtual ~MainMenu();

        virtual bool prepare();
        virtual bool update();

    private:
        ButtonMenu* m_play;        /**< @brief The button for the play menu. */
        ButtonMenu* m_cfg;         /**< @brief The button for the config menu. */
        ButtonMenu* m_about;       /**< @brief The button for the about menu. */
        gui::Button* m_quit;       /**< @brief The button for quitting. */
        gui::GridLayout* m_layout; /**< @brief The layout. */
        Menu* m_actual;            /**< @brief The actual submenu is in, NULL if none. */
};

#endif

