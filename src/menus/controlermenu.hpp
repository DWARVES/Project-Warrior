
#ifndef DEf_CONTROLERMENU
#define DEF_CONTROLERMENU

#include "menu.hpp"
#include "events/joystick.hpp"
#include "gameplay/controler.hpp"
#include <string>

/** @brief A menu allowing to change the keybinds of a controler. */
class ControlerMenu : public Menu
{
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
};

#endif

