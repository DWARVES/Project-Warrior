
#ifndef DEF_ADDCONTROLERMENU
#define DEF_ADDCONTROLERMENU

#include "menu.hpp"
#include "gui/list.hpp"
#include "gui/button.hpp"
#include "gui/gridlayout.hpp"
#include <vector>
#include <string>

/** @brief The menu to add a new controler. */
class AddControlerMenu : public Menu
{
    private:
        /** @brief The list used by AddControlerMenu to select a controler to add. */
        class List : public gui::List {
            public:
                List() = delete;
                List(Menu** act);
                virtual ~List();
                virtual void enter();

            private:
                Menu** m_act; /**< @brief Setted to a ControlerMenu when a new controler is selected. */
        };

    public:
        AddControlerMenu();
        virtual ~AddControlerMenu();

        virtual bool prepare();
        virtual bool update();

    private:
        gui::GridLayout* m_layout; /**< @brief The layout. */
        gui::Button* m_back;       /**< @brief The button to get back to the config menu. */
        List* m_list;              /**< @brief The list to select a controler. */
        Menu* m_actual;            /**< @brief The ControlerMenu launched to configure the selected new one. */
};

#endif

