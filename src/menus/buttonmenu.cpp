
#include "buttonmenu.hpp"

ButtonMenu::ButtonMenu(graphics::Graphics* gfx, Menu** feedback, Menu* menu, bool tofree)
    : Button(gfx), m_feed(feedback), m_menu(menu), m_tofree(tofree)
{}

ButtonMenu::~ButtonMenu()
{
    if(m_tofree)
        delete m_menu;
}

void ButtonMenu::select()
{
    m_menu->prepare();
    *m_feed = m_menu;
}

