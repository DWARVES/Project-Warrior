
#include "configmenu.hpp"
#include "graphicsmenu.hpp"
#include "musicmenu.hpp"
#include "buttonmenu.hpp"
#include "controlermenu.hpp"
#include "addcontrolermenu.hpp"
#include "mainmenu.hpp"
#include "global.hpp"
#include "core/i18n.hpp"
#include "gameplay/controler.hpp"

MenuList::MenuList(Menu** feed)
    : List(global::gfx), m_feed(feed)
{}

MenuList::~MenuList()
{
}

void MenuList::enter()
{
    Menu* menu = (Menu*)selectedData();
    if(!menu || !menu->prepare())
        return;
    *m_feed = menu;

    ButtonMenu::click();
}

ConfigMenu::ConfigMenu()
    : Menu(), m_layout(NULL), m_back(NULL), m_list(NULL), m_actual(NULL)
{}

ConfigMenu::~ConfigMenu()
{
    if(m_layout)
        delete m_layout;
    if(m_back)
        delete m_back;
    if(m_list) {
        m_list->freeData([] (void* data) { delete (Menu*)data; });
        delete m_list;
    }
}

bool ConfigMenu::prepare()
{
    /* Load the widgets if necessary. */
    if(!m_layout) {
        m_list = new MenuList(&m_actual);
        global::theme->apply(m_list);
        m_list->addItem(0, _i("Music."),    0.0f, new MusicMenu);
        m_list->addItem(0, _i("Graphics."), 0.0f, new GraphicsMenu);

        /* Add controlers. */
        std::vector<std::string> conts = gameplay::Controler::listAll();
        int i = 2;
        for(std::string id : conts) {
            m_list->addItem(i, id, 0.0f, new ControlerMenu(id));
            ++i;
        }
        m_list->addItem(i, _i("New controler"), 0.0f, new AddControlerMenu);

        m_back = new gui::Button(global::gfx);
        m_back->text(_i("Back to main menu."));
        global::theme->apply(m_back);

        m_layout = new gui::GridLayout(global::gfx, 1, 6);
        global::theme->apply(m_layout);
        m_layout->addWidget(m_list, 0, 0, 0, 4);
        m_layout->addWidget(m_back, 0, 5, 0, 0);
    }

    if(!MainMenu::loadRcs())
        return false;

    /* Set up the main. */
    geometry::Point pos;
    global::gfx->disableVirtualSize();
    pos.x = global::gfx->getVirtualWidth() * 0.1f;
    pos.y = global::gfx->getVirtualHeight() * 0.1f;
    global::gui->main(m_layout, pos, global::gfx->getVirtualWidth() * 0.8f, global::gfx->getVirtualHeight() * 0.8f);
    return true;
}

bool ConfigMenu::update()
{
    if(m_actual) {
        if(m_actual->update())
            return true;
        prepare();
        m_actual = NULL;
        return true;
    }

    if(global::evs->keyJustPressed(events::KeyMap::Escape)
            || m_back->clicked()) {
        ButtonMenu::click();
        return false;
    }

    /* Drawing */
    geometry::AABB rect(global::gfx->getVirtualWidth(), global::gfx->getVirtualHeight());
    global::gfx->enterNamespace("/mainmenu");

    global::gfx->beginDraw();
    global::gfx->draw(rect, "bg");
    global::theme->guiNamespace();
    global::gui->draw();
    global::gfx->endDraw();

    return true;
}


