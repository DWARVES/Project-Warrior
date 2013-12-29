
#ifndef DEF_MAINMENU
#define DEF_MAINMENU

#include "menus/menu.hpp"
#include "menus/buttonmenu.hpp"
#include "menus/videomenu.hpp"
#include "gui/gridlayout.hpp"
#include "gui/button.hpp"

/** @brief Manages the main menu. */
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
        VideoMenu* m_intro;        /**< @brief The introductive video. */
        bool m_first;              /**< @brief Is it the first time preparing ? */
};

#endif

