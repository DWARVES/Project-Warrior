
#include "mainmenu.hpp"
#include "aboutmenu.hpp"
#include "configmenu.hpp"
#include "playerselmenu.hpp"
#include "global.hpp"
#include "core/i18n.hpp"
#include <sstream>

MainMenu::MainMenu()
    : Menu(), m_play(NULL), m_cfg(NULL), m_about(NULL), m_quit(NULL), m_layout(NULL), m_actual(NULL)
{}

MainMenu::~MainMenu()
{
    if(m_layout)
        delete m_layout;
    if(m_play)
        delete m_play;
    if(m_cfg)
        delete m_cfg;
    if(m_about)
        delete m_about;
    if(m_quit)
        delete m_quit;
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
        m_play = new ButtonMenu(global::gfx, &m_actual, new PlayerSelMenu, true);
        m_play->text(_i("Play"));
        m_play->applyTheme(global::theme);

        m_cfg = new ButtonMenu(global::gfx, &m_actual, new ConfigMenu, true);
        m_cfg->text(_i("Config"));
        m_cfg->applyTheme(global::theme);

        m_about = new ButtonMenu(global::gfx, &m_actual, new AboutMenu, true);
        m_about->text(_i("About"));
        m_about->applyTheme(global::theme);

        m_quit = new gui::Button(global::gfx);
        m_quit->text(_i("Quit"));
        global::theme->apply(m_quit);

        m_layout = new gui::GridLayout(global::gfx, 2, 4);
        global::theme->apply(m_layout);
        m_layout->addWidget(m_play,  0, 0, 0, 3);
        m_layout->addWidget(m_cfg,   1, 0, 0, 1);
        m_layout->addWidget(m_about, 1, 2, 0, 0);
        m_layout->addWidget(m_quit,  1, 3, 0, 0);
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


