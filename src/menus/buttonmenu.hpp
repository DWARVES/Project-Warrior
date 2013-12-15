
#ifndef DEF_BUTTONMENU
#define DEF_BUTTONMENU

#include "gui/button.hpp"
#include "menus/menu.hpp"

/** @brief A button used to launch a menu. */
class ButtonMenu : public gui::Button
{
    public:
        /** @brief Only one constructor.
         * @param feedback When the button is selected, *feedback will be set to menu.
         * @param menu The menu handled by this button : when selected, the menu is prepared.
         * @param tofree If true, the menu will be free'd when the button is deleted.
         */
        ButtonMenu(graphics::Graphics* gfx, Menu** feedback, Menu* menu, bool tofree = false);
        ButtonMenu() = delete;
        ButtonMenu(const ButtonMenu&) = delete;
        ~ButtonMenu();

        /** @todo Add sounds. */
        virtual void select();

    private:
        Menu** m_feed;   /**< @brief The feedback pointer. */
        Menu*  m_menu;   /**< @brief The menu handled. */
        bool   m_tofree; /**< @brief Indicates if m_menu must be free'd. */
};

#endif

