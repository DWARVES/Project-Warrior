
#include "mainmenu.hpp"
#include "gamemenu.hpp"
#include "global.hpp"
#include "core/i18n.hpp"
#include "lua/charaExposure.hpp"
#include <sstream>

    MainMenu::MainMenu()
: Menu(), m_actual(NULL), m_st(NULL)
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
        m_ctrls[0] = new gameplay::Controler("Keyboard");
        m_ctrls[1] = new gameplay::Controler("Keyboard");
        //if(!m_ctrls[1]) {
            //core::logger::logm("No joystick to play with.", core::logger::FATAL);
            //return false;
        //}

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

gameplay::Controler* MainMenu::loadJoystick()
{
    std::string name;
    std::vector<std::string> ids   = gameplay::Controler::getIDs();
    std::vector<std::string> names = global::evs->joyNames();

    /* Check if there is one already configured. */
    if(ids.size() > 1) {
        unsigned int i = 0;
        while(ids[i] == "Keyboard") {
            ++i;
            if(i >= ids.size())
                return NULL;
        }
        name = ids[i];
    }
    /* Add a new one */
    else if(!names.empty())
        name = names[0];
    /* Fail */
    else
        return NULL;

    /* Loading the controler. */
    gameplay::Controler::create(name);
    return new gameplay::Controler(name);
}

