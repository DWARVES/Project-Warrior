
#ifndef DEF_GAMEMENU
#define DEF_GAMEMENU

#include "menu.hpp"
#include "gameplay/stage.hpp"

/** @brief The game.
 * @todo Draw status and add ending conditions.
 */
class GameMenu : public Menu
{
    public:
        GameMenu(gameplay::Stage* st);
        virtual ~GameMenu();

        virtual bool prepare();
        virtual bool update();

    private:
        gameplay::Stage* m_stage; /**< @brief The stage of the game. */
};

#endif

