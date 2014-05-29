
#ifndef DEF_GAMEMENU
#define DEF_GAMEMENU

#include "menu.hpp"
#include "gameplay/stage.hpp"

/** @brief The game. */
class GameMenu : public Menu
{
    public:
        GameMenu(gameplay::Stage* st);
        virtual ~GameMenu();

        virtual bool prepare();
        virtual bool update();

    private:
        gameplay::Stage* m_stage;     /**< @brief The stage of the game. */
        gameplay::Character* m_pl[4]; /**< @brief The characters. */
        Uint32 m_duration;            /**< @brief The duration of the game. */
        Uint32 m_begin;               /**< @brief Timestamp of the begginig of the game. */
        bool m_first;                 /**< @brief Is this the first time update is called. */
};

#endif

