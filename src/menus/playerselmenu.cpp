
#include "playerselmenu.hpp"
#include "global.hpp"
#include "core/i18n.hpp"
#include "gameplay/controler.hpp"
#include <sstream>

    PlayerSelMenu::PlayerSelMenu()
: m_act(0), m_layout(NULL), m_list(NULL), m_text(NULL),
    m_cancel(NULL), m_play(NULL), m_back(NULL), m_ingame(false)
{}

PlayerSelMenu::~PlayerSelMenu()
{
    if(m_layout != NULL)
        delete m_layout;
    if(m_list != NULL)
        delete m_list;
    if(m_text != NULL)
        delete m_text;
    if(m_cancel != NULL)
        delete m_cancel;
    if(m_play != NULL)
        delete m_play;
    if(m_back != NULL)
        delete m_back;
}

bool PlayerSelMenu::prepare()
{
    m_act = 0;
    for(int i = 0; i < 4; ++i)
        m_players[i].clear();
    m_ingame = false;

    if(!m_layout) {
        /* Create GUI elems. */
        m_layout = new gui::GridLayout(global::gfx, 3, 6);
        m_list   = new List;
        m_text   = new gui::Text(global::gfx);
        m_cancel = new gui::Button(global::gfx);
        m_cancel->text(_i("Cancel"));
        m_play   = new gui::Button(global::gfx);
        m_play->text(_i("Play"));
        m_back   = new gui::Button(global::gfx);
        m_back->text(_i("Back"));

        /* Apply theme. */
        global::theme->apply(m_layout);
        global::theme->apply(m_list);
        global::theme->apply(m_text);
        global::theme->apply(m_cancel);
        global::theme->apply(m_play);
        global::theme->apply(m_back);

        /* Populate the list. */
        std::vector<std::string> ctrls = gameplay::Controler::getIDs();
        for(unsigned int i = 0; i < ctrls.size(); ++i)
            m_list->addItem(i, ctrls[i]);

        /* Set up the layout. */
        m_layout->addWidget(m_list,   0, 0, 1, 5);
        m_layout->addWidget(m_text,   2, 0, 0, 2);
        m_layout->addWidget(m_cancel, 2, 3, 0, 0);
        m_layout->addWidget(m_play,   2, 4, 0, 0);
        m_layout->addWidget(m_back,   2, 5, 0, 0);
    }
    updateText();

    /* Setting up the gui. */
    geometry::Point pos;
    global::gfx->disableVirtualSize();
    pos.x = global::gfx->getVirtualWidth() * 0.1f;
    pos.y = global::gfx->getVirtualHeight() * 0.1f;
    global::gui->main(m_layout, pos, global::gfx->getVirtualWidth() * 0.8f, global::gfx->getVirtualHeight() * 0.8f);
    return true;
}

bool PlayerSelMenu::update()
{
    if(m_ingame)
        return m_game.update();

    if(global::evs->keyJustPressed(events::KeyMap::Escape)
            || m_back->clicked())
        return false;

    if(m_cancel->clicked() && m_act > 0) {
        if(m_act < 4)
            m_players[m_act].clear();
        --m_act;
        updateText();

        global::audio->enterNamespace("/menubutton");
        global::audio->play("click");
    }

    if(m_play->clicked() && m_act > 0) {
        m_game.set(m_act, m_players);
        if(!m_game.prepare())
            return false;
        else
            m_ingame = true;
        global::audio->enterNamespace("/menubutton");
        global::audio->play("click");
    }

    std::string sel = m_list->entered();
    if(!sel.empty() && m_act < 4) {
        m_players[m_act] = sel;
        if(m_act < 4)
            ++m_act;
        updateText();

        global::audio->enterNamespace("/menubutton");
        global::audio->play("click");
    }

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

void PlayerSelMenu::updateText()
{
    std::ostringstream oss;
    for(int i = 0; i < 4; ++i) {
        if(i == m_act)
            oss << _i("Player ") << i << _i(" : Please select a controler.\n");
        else if(m_players[i].empty())
            oss << _i("Player ") << i << _i(" : None.\n");
        else
            oss << _i("Player ") << i << _i(" : ") << m_players[i] << ".\n";
    }
    m_text->setText(oss.str());
}

PlayerSelMenu::List::List()
    : gui::List(global::gfx)
{}

PlayerSelMenu::List::~List()
{}

void PlayerSelMenu::List::select()
{
    if(selectedText() != m_enter)
        m_enter.clear();
}

void PlayerSelMenu::List::enter()
{
    m_enter = selectedText();
}

std::string PlayerSelMenu::List::entered()
{
    std::string ret = m_enter;
    m_enter.clear();
    return ret;
}

