
#include "addcontrolermenu.hpp"
#include "controlermenu.hpp"
#include "global.hpp"
#include "core/i18n.hpp"

AddControlerMenu::AddControlerMenu()
    : Menu(), m_layout(NULL), m_back(NULL), m_list(NULL), m_actual(NULL)
{}

AddControlerMenu::~AddControlerMenu()
{
    if(m_layout != NULL)
        delete m_layout;
    if(m_back != NULL)
        delete m_back;
    if(m_list != NULL)
        delete m_list;
    if(m_actual != NULL)
        delete m_actual;
}

bool AddControlerMenu::prepare()
{
    if(!m_layout) {
        /* Allocating the widgets. */
        m_layout = new gui::GridLayout(global::gfx, 1, 5);
        global::theme->apply(m_layout);
        m_back = new gui::Button(global::gfx);
        global::theme->apply(m_back);
        m_back->text(_i("Back"));
        m_list = new List(&m_actual);
        global::theme->apply(m_list);

        /* Listing new joysticks. */
        std::vector<std::string> joys = global::evs->joyNames();
        std::vector<std::string> alrd = gameplay::Controler::listAll();
        std::vector<std::string> news;
        news.reserve(joys.size());
        for(std::string joy : joys) {
            if(std::find(alrd.begin(), alrd.end(), joy) == alrd.end())
                news.push_back(joy);
        }

        /* Setting up the list. */
        for(size_t i = 0; i < news.size(); ++i)
            m_list->addItem(i, news[i]);

        /* Setting up the layout. */
        m_layout->addWidget(m_list, 0, 0, 0, 3);
        m_layout->addWidget(m_back, 0, 4);
    }

    /* Setting up the gui. */
    geometry::Point pos;
    global::gfx->disableVirtualSize();
    pos.x = global::gfx->getVirtualWidth() * 0.1f;
    pos.y = global::gfx->getVirtualHeight() * 0.1f;
    global::gui->main(m_layout, pos, global::gfx->getVirtualWidth() * 0.8f, global::gfx->getVirtualHeight() * 0.8f);
    return true;
}

bool AddControlerMenu::update()
{
    if(global::evs->keyJustPressed(events::KeyMap::Escape)
            || m_back->clicked())
        return false;

    if(m_actual)
        return m_actual->update();

    /* Drawing. */
    global::gfx->enterNamespace("/mainmenu");
    geometry::AABB rect(global::gfx->getVirtualWidth(), global::gfx->getVirtualHeight());

    global::gfx->beginDraw();
    global::gfx->draw(rect, "bg");
    global::theme->guiNamespace();
    global::gui->draw();
    global::gfx->endDraw();

    return true;
}

AddControlerMenu::List::List(Menu** act)
    : gui::List(global::gfx), m_act(act)
{}

AddControlerMenu::List::~List()
{}

void AddControlerMenu::List::enter()
{
    std::string newID = selectedText();
    if(gameplay::Controler::create(newID)) {
        *m_act = new ControlerMenu(newID);
        (*m_act)->prepare();
    }
}


