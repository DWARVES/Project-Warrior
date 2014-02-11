
#include "controlermenu.hpp"
#include "core/logger.hpp"

    ControlerMenu::ControlerMenu(const std::string& id)
: m_id(id), m_ctrl(id)
{
    if(m_ctrl.isOpen()) {
        if(m_id == "keyboard")
            m_joy = NULL;
    }
}

ControlerMenu::~ControlerMenu()
{
}

bool ControlerMenu::prepare()
{
}

bool ControlerMenu::update()
{
}


