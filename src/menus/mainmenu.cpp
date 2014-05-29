
#include "mainmenu.hpp"
#include "gamemenu.hpp"
#include "global.hpp"
#include "core/i18n.hpp"
#include "lua/charaExposure.hpp"
#include <sstream>

const char* ctrl0 = "Keyboard";
const char* ctrl1 = "Mega World Thrustmaster dual analog 3.2";

    MainMenu::MainMenu()
: Menu(), m_actual(NULL)
{
    for(int i = 0; i < 4; ++i)
        m_ctrls[i] = NULL;
}

MainMenu::~MainMenu()
{
    if(m_st)
        delete m_st;
    for(int i = 0; i < 4; ++i) {
        if(m_ctrls[i])
            delete m_ctrls[i];
    }
}

bool MainMenu::prepare()
{
    global::gui->focus(false);
    return true;
}

bool MainMenu::update()
{
    if(m_actual)
        return m_actual->update();
    else {
        /* Creating the controlers. */
        for(int i = 0; i < 4; ++i) {
            if(m_ctrls[i])
                delete m_ctrls[i];
            m_ctrls[i] = NULL;
        }
        m_ctrls[0] = new gameplay::Controler(ctrl0);
        m_ctrls[1] = new gameplay::Controler(ctrl1);

        /* Creating the characters. */
        std::string path = global::cfg->get<std::string>("rcs") + "/chara/";
        gameplay::Character* ch;
        ch = new gameplay::Character(path + "shadow");
        lua::exposure::characters[0] = ch;
        if(!ch->preload() || !ch->load(gameplay::Character::Color::None, 0)) {
            return false;
        }
        m_ctrls[0]->attach(ch);

        ch = new gameplay::Character(path + "fighter");
        lua::exposure::characters[1] = ch;
        if(!ch->preload() || !ch->load(gameplay::Character::Color::None, 1)) {
            return false;
        }
        m_ctrls[1]->attach(ch);

        /* Creating the stage. */
        path = global::cfg->get<std::string>("rcs") + "/stages/default";
        m_st = new gameplay::Stage(path);
        if(!m_st->load(m_ctrls))
            return false;

        /* Creating the menu */
        m_actual = new GameMenu(m_st);
        if(!m_actual->prepare())
            return false;
    }

    return true;
}


