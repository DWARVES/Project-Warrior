
#ifndef DEF_PLAYERSELMENU
#define DEF_PLAYERSELMENU

#include "menu.hpp"
#include "gui/gridlayout.hpp"
#include "gui/text.hpp"
#include "gui/button.hpp"
#include "gui/list.hpp"
#include <string>

/** @brief The menu to select players and their controlers. */
class PlayerSelMenu : public Menu
{
    /** @brief The list used to select the controlers for players. */
    class List : public gui::List
    {
        public:
            List();
            virtual ~List();

            virtual void select();
            virtual void enter();
            /** @brief Get the entry entered (empty if none). */
            std::string entered();

        private:
            std::string m_enter; /**< @brief The entry last entered. */
    };

    public:
        PlayerSelMenu();
        virtual ~PlayerSelMenu();

        virtual bool prepare();
        virtual bool update();

    private:
        std::string m_players[4];  /**< @brief The players controlers (by id). Empty if none. */
        int m_act;                 /**< @brief The id of the actual button. */
        gui::GridLayout* m_layout; /**< @brief The layout. */
        List* m_list;              /**< @brief The list of controlers. */
        gui::Text* m_text;         /**< @brief The recap text. */
        gui::Button* m_cancel;     /**< @brief Cancel the last selected controler. */
        gui::Button* m_play;       /**< @brief Launch the game with the characters selected. */
        gui::Button* m_back;       /**< @brief Get back to main menu. */

        /** @brief Update the text printed. */
        void updateText();
};

#endif

