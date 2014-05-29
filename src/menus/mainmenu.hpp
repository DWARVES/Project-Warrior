
#ifndef DEF_MAINMENU
#define DEF_MAINMENU

#include "menus/menu.hpp"
#include "gameplay/controler.hpp"
#include "gameplay/character.hpp"
#include "gameplay/stage.hpp"

/** @brief Manages the main menu. */
class MainMenu : public Menu
{
    public:
        MainMenu();
        virtual ~MainMenu();

        virtual bool prepare();
        virtual bool update();
        /** @brief Loads the texture of the mainmenu. */
        static bool loadRcs();

    private:
        Menu* m_actual;            /**< @brief The actual submenu is in, NULL if none. */
        gameplay::Controler* m_ctrls[4];
        gameplay::Stage* m_st;
};

#endif

