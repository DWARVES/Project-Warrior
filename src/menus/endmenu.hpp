
#ifndef DEF_ENDMENU
#define DEF_ENDMENU

#include "menu.hpp"
#include "gameplay/character.hpp"
#include "gui/gridlayout.hpp"
#include "gui/button.hpp"
#include "gui/text.hpp"

/** @brief The game over screen, display scores. */
class EndMenu : public Menu
{
    public:
        EndMenu(gameplay::Character* pl[4]);
        virtual ~EndMenu();

        virtual bool prepare();
        virtual bool update();

    private:
        gameplay::Character* m_players[4]; /**< @brief The players. */
        gui::GridLayout m_layout;          /**< @brief The layout used. */
        gui::Text m_scores[4];             /**< @brief The text displaying the scores. */
        gui::Button m_end;                 /**< @brief The button to quit the menu. */
};

#endif

