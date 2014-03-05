
#ifndef DEF_CHARASELMENU
#define DEF_CHARASELMENU

#include "menu.hpp"
#include "gui/gridlayout.hpp"
#include "gui/button.hpp"
#include "gui/text.hpp"
#include "gui/list.hpp"
#include "gui/widget.hpp"
#include "gameplay/character.hpp"

/** @brief A menu to select the characters for all players.
 * @todo Handle colors.
 */
class CharaSelMenu : public Menu
{
    /** @brief A list used to select a character. */
    class List : public gui::List
    {
        public:
            List();
            virtual ~List();

            virtual void select();
            virtual void enter();

            /** @brief Return true if the selection has changed during previous loop. */
            bool moved();
            /** @brief Returns the item entered during previous loop. */
            std::string entered();

        private:
            std::string m_entered; /**< @brief The item entered during previous loop. */
            bool m_moved;          /**< @brief Indicate if the selection has changed during previous loop. */
    };

    /** @brief A widget to display the preview of a character. */
    class CharaPrev : public gui::Widget
    {
        public:
            CharaPrev();
            /** @brief Set the character to display the preview. */
            void set(gameplay::Character* c);
            void draw();

        private:
            gameplay::Character* m_chara;
    };

    public:
        CharaSelMenu(int nb_players, std::string ctrls[4]);
        CharaSelMenu() = delete;
        virtual ~CharaSelMenu();

        virtual bool prepare();
        virtual bool update();

    private:
        int m_nb;                                  /**< @brief Number of players. */
        std::string m_ctrls[4];                    /**< @brief The controlers of the players. */
        int m_act;                                 /**< @brief The player actually in selection. */
        gameplay::Character* m_sels[4];            /**< @brief The selected characters. */
        std::vector<gameplay::Character*> m_avail; /**< @brief Available characters. */
        Menu* m_launched;                          /**< @brief The launched menu (game or rules). */

        gui::GridLayout* m_layout;                 /**< @brief The layout. */
        List* m_charas;                            /**< @brief The list of characters. */
        gui::Text* m_title;                        /**< @brief The title. */
        gui::Text* m_desc;                         /**< @brief The textual description of the character. */
        CharaPrev* m_prev;                         /**< @brief The preview of the character. */
        gui::Button* m_play;                       /**< @brief The button to launch the game. */
        gui::Button* m_cancel;                     /**< @brief The button to cancel the last selected character. */
        gui::Button* m_rules;                      /**< @brief The button to access the rules menu. */
        gui::Button* m_back;                       /**< @brief The button to get back to player selection. */

        /** @brief Update the text of title. */
        void updateTitle();
        /** @brief Update the text in the description. */
        void updateDesc();
        /** @brief Load available characters. */
        void loadCharas();
};

#endif

