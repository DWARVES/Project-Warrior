
#ifndef DEF_CONFIGMENU
#define DEF_CONFIGMENU

#include "menu.hpp"
#include "gui/list.hpp"
#include "gui/button.hpp"
#include "gui/gridlayout.hpp"

/** @brief A widget used to select a Menu in a list. */
class MenuList : public gui::List
{
    public:
        MenuList() = delete;
        MenuList(Menu** feed);
        virtual ~MenuList();
        /** @brief Enter the menu attached to the current item. */
        virtual void enter();

    private:
        Menu** m_feed;
};

/** @brief The main config menu : contains a list of all config menus. */
class ConfigMenu : public Menu
{
    public:
        ConfigMenu();
        virtual ~ConfigMenu();

        virtual bool prepare();
        virtual bool update();

    private:
        gui::GridLayout* m_layout; /**< @brief The layout. */
        gui::Button* m_back;       /**< @brief The button to return to main menu. */
        MenuList* m_list;          /**< @brief The list of all sub-menus. */
        Menu* m_actual;            /**< @brief The actual sub menu. */
};

#endif

