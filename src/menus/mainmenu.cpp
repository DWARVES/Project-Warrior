
#include "mainmenu.hpp"
#include "gamemenu.hpp"
#include "global.hpp"
#include "core/i18n.hpp"
#include "lua/charaExposure.hpp"
#include <sstream>

const char* ctrl0 = "keyboard";
const char* ctrl1 = "Mega World Thrustmaster dual analog 3.2";

MainMenu::MainMenu()
    : Menu(), m_play(NULL), m_quit(NULL), m_layout(NULL), m_actual(NULL)
{
    for(int i = 0; i < 4; ++i)
        m_ctrls[i] = NULL;
}

MainMenu::~MainMenu()
{
    if(m_layout)
        delete m_layout;
    if(m_play)
        delete m_play;
    if(m_quit)
        delete m_quit;
    if(m_st)
        delete m_st;
    for(int i = 0; i < 4; ++i) {
        if(m_ctrls[i])
            delete m_ctrls[i];
    }
}

bool MainMenu::prepare()
{
    /* Load the music if necessary. */
    if(!global::audio->existsNamespace("/menus")) {
        if(!global::audio->createNamespace("/menus"))
            return false;
        global::audio->enterNamespace("/menus");

        std::ostringstream path;
        path << global::cfg->get<std::string>("rcs") << "/sounds/menu.music";
        if(!global::audio->loadMusic("mus", path.str()))
            return false;
    }
    global::audio->enterNamespace("/menus");
    global::audio->play("mus", false);

    if(!loadRcs())
        return false;

    /* Create the widgets if necessary. */
    if(!m_layout) {
        m_play = new gui::Button(global::gfx);
        m_play->text(_i("Play"));
        global::theme->apply(m_play);

        m_quit = new gui::Button(global::gfx);
        m_quit->text(_i("Quit"));
        global::theme->apply(m_quit);

        m_layout = new gui::GridLayout(global::gfx, 3, 3);
        global::theme->apply(m_layout);
        m_layout->addWidget(m_play,  0, 0, 2, 1);
        m_layout->addWidget(m_quit,  0, 2, 2, 0);
    }

    /* Set up the gui. */
    geometry::Point pos;
    global::gfx->disableVirtualSize();
    pos.x = global::gfx->getVirtualWidth() * 0.1f;
    pos.y = global::gfx->getVirtualHeight() * 0.2f;
    global::gui->main(m_layout, pos, global::gfx->getVirtualWidth() * 0.8f, global::gfx->getVirtualHeight() * 0.7f);

    return true;
}
        
bool MainMenu::loadRcs()
{
    /* Load the textures if necessary. */
    if(!global::gfx->existsNamespace("/mainmenu")) {
        if(!global::gfx->createNamespace("/mainmenu"))
            return false;
        global::gfx->enterNamespace("/mainmenu");

        std::ostringstream path;
        path << global::cfg->get<std::string>("rcs") << "/mainmenu/";
        if(!global::gfx->loadTexture("bg", path.str() + "bg.png"))
            return false;
        if(!global::gfx->loadTexture("title", path.str() + "title.png"))
            return false;
    }

    return true;
}

bool MainMenu::update()
{
    if(m_actual) {
        if(m_actual->update())
            return true;
        prepare();
        m_actual = NULL;
        return true;
    }

    if(global::evs->keyJustPressed(events::KeyMap::Escape)
            || m_quit->clicked())
        return false;

    if(m_play->clicked()) {
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

    /* Drawing */
    global::gfx->enterNamespace("/mainmenu");
    geometry::AABB rect(global::gfx->getVirtualWidth(), global::gfx->getVirtualHeight());

    global::gfx->beginDraw();
    global::gfx->push();
    global::gfx->draw(rect, "bg");

    rect.width *= 0.8f;
    rect.height *= 0.1f;
    global::gfx->move(global::gfx->getVirtualWidth() * 0.1f, 0.0f);
    global::gfx->draw(rect, "title");
    global::gfx->pop();

    global::theme->guiNamespace();
    global::gui->draw();
    global::gfx->endDraw();

    return true;
}


