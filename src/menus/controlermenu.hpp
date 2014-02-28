
#ifndef DEf_CONTROLERMENU
#define DEF_CONTROLERMENU

#include "menu.hpp"
#include "events/joystick.hpp"
#include "gameplay/controler.hpp"
#include "gui/list.hpp"
#include "gui/button.hpp"
#include "gui/text.hpp"
#include "gui/gridlayout.hpp"
#include <string>

/** @brief A menu allowing to change the keybinds of a controler. */
class ControlerMenu : public Menu
{
    private:
        /** @brief The list widget used to configure the events. */
        class List : public gui::List
        {
            public:
                List(gui::Text* text, gameplay::Controler* ctrl, events::Joystick* joy);
                virtual ~List();

                virtual void select();
                virtual void enter();

            private:
                gui::Text* m_text;           /**< @brief The text widget to which the description of the selected event will be printed. */
                gameplay::Controler* m_ctrl; /**< @brief The controler used to save the entered events. */
                events::Joystick* m_joy;     /**< @brief The joystick to configure (NULL if keyboard). */

                events::EvSave* getEvent();
        };

    public:
        ControlerMenu() = delete;
        /** @param id The id of the controler to configure. */
        ControlerMenu(const std::string& id);
        virtual ~ControlerMenu();

        virtual bool prepare();
        virtual bool update();

    private:
        std::string m_id;           /**< @brief The id of the controler. */
        events::Joystick* m_joy;    /**< @brief The joystick corresponding to the id, NULL if the controler is the keyboard. */
        gameplay::Controler m_ctrl; /**< @brief The corresponding controler, storing the configuration. */
        bool m_plugged;             /**< @brief Is the joystick plugged. */

        gui::List* m_ctrls;         /**< @brief The list used to select the control to configure (only if the controler is plugged). */
        gui::Button* m_back;        /**< @brief The button to get back to main menu. */
        gui::Text* m_text;          /**< @brief If the controler is plugged, a help message, else an error message. */
        gui::GridLayout* m_layout;  /**< @brief The layout. */

        /** @brief Update the name of all the items of the list according to the config. */
        void updatePrinted() const;
};

#endif

