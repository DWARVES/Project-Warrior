
#include "charaselmenu.hpp"
#include "stageselmenu.hpp"
#include "global.hpp"
#include "core/i18n.hpp"
#include "core/pathParser.hpp"
#include "core/logger.hpp"
#include "lua/charaExposure.hpp"
#include <sstream>

    CharaSelMenu::List::List()
: gui::List(global::gfx), m_moved(false)
{}

CharaSelMenu::List::~List()
{}

void CharaSelMenu::List::select()
{
    m_moved = true;
}

void CharaSelMenu::List::enter()
{
    m_entered = selectedText();
}

bool CharaSelMenu::List::moved()
{
    bool temp = m_moved;
    m_moved = false;
    return temp;
}

std::string CharaSelMenu::List::entered()
{
    std::string temp = m_entered;
    m_entered.clear();
    return temp;
}

CharaSelMenu::CharaPrev::CharaPrev()
    : Widget(global::gfx), m_chara(NULL)
{}

void CharaSelMenu::CharaPrev::set(gameplay::Character* c)
{
    m_chara = c;
}

void CharaSelMenu::CharaPrev::draw()
{
    if(!m_chara)
        return;
    std::string actual = m_gfx->actualNamespace();
    geometry::AABB rect(width(), height());
    m_chara->bigPreview(gameplay::Character::None, rect);
    m_gfx->enterNamespace(actual);
}

    CharaSelMenu::CharaSelMenu()
: Menu(), m_nb(-1), m_act(0), m_launched(NULL), m_timem(0), m_layout(NULL),
    m_charas(NULL), m_title(NULL), m_desc(NULL), m_prev(NULL),
    m_play(NULL), m_cancel(NULL), m_rules(NULL), m_back(NULL)
{
    for(int i = 0; i < 4; ++i)
        m_sels[i] = NULL;
}
        
void CharaSelMenu::set(int nb, std::string ctrls[4])
{
    m_nb = nb;
    if(m_nb < 0 || m_nb > 4)
        m_nb = -1;

    for(int i = 0; i < 4; ++i)
        m_ctrls[i] = ctrls[i];
}

CharaSelMenu::~CharaSelMenu()
{
    if(m_launched != NULL)
        delete m_launched;
    if(m_layout != NULL)
        delete m_layout;
    if(m_charas != NULL)
        delete m_charas;
    if(m_title != NULL)
        delete m_title;
    if(m_desc != NULL)
        delete m_desc;
    if(m_prev != NULL)
        delete m_prev;
    if(m_play != NULL)
        delete m_play;
    if(m_cancel != NULL)
        delete m_cancel;
    if(m_rules != NULL)
        delete m_rules;
    if(m_back != NULL)
        delete m_back;

    for(size_t i = 0; i < m_avail.size(); ++i) {
        if(m_avail[i])
            delete m_avail[i];
    }
}

bool CharaSelMenu::prepare()
{
    /* Has it be setted. */
    if(m_nb < 0)
        return false;

    /* Loading the characters. */
    if(m_avail.empty())
        loadCharas();

    /* Freeing the already selected. */
    for(int i = 0; i < 4; ++i) {
        if(m_sels[i])
            delete m_sels[i];
        m_sels[i] = NULL;
    }
    m_timem = SDL_GetTicks();
    m_showing = true;

    if(m_launched != NULL)
        delete m_launched;
    m_launched = NULL;

    if(!m_layout) {
        /* Creating the widgets. */
        m_layout = new gui::GridLayout(global::gfx, 3, 9);
        m_charas = new List;
        m_title  = new gui::Text(global::gfx);   m_title->oneline(true);
        m_desc   = new gui::Text(global::gfx);
        m_prev   = new CharaPrev;
        m_play   = new gui::Button(global::gfx); m_play->text(_i("Play"));
        m_cancel = new gui::Button(global::gfx); m_cancel->text(_i("Cancel"));
        m_rules  = new gui::Button(global::gfx); m_rules->text(_i("Edit rules"));
        m_back   = new gui::Button(global::gfx); m_back->text(_i("Back"));
        updateTitle();

        /* Applying the theme. */
        global::theme->apply(m_layout);
        global::theme->apply(m_charas);
        global::theme->apply(m_title);
        global::theme->apply(m_desc);
        global::theme->apply(m_play);
        global::theme->apply(m_cancel);
        global::theme->apply(m_rules);
        global::theme->apply(m_back);

        /* Populating the list. */
        for(size_t i = 0; i < m_avail.size(); ++i)
            m_charas->addItem(i, m_avail[i]->name(), 0.0f, (void*)m_avail[i]);
        m_charas->addItem(m_avail.size(), _i("Random"), 0.0f, NULL);
        updateDesc();

        /* Setting up the layout. */
        m_layout->addWidget(m_title,  0, 0, 2, 0);
        m_layout->addWidget(m_charas, 0, 1, 1, 7);
        m_layout->addWidget(m_prev,   2, 1, 0, 3);
        m_layout->addWidget(m_cancel, 2, 5, 0, 0);
        m_layout->addWidget(m_play,   2, 6, 0, 0);
        m_layout->addWidget(m_rules,  2, 7, 0, 0);
        m_layout->addWidget(m_back,   2, 8, 0, 0);
    }

    /* Setting up the gui. */
    geometry::Point pos;
    global::gfx->disableVirtualSize();
    pos.x = global::gfx->getVirtualWidth() * 0.1f;
    pos.y = global::gfx->getVirtualHeight() * 0.1f;
    global::gui->main(m_layout, pos, global::gfx->getVirtualWidth() * 0.8f, global::gfx->getVirtualHeight() * 0.8f);
    return true;
}

bool CharaSelMenu::update()
{
    if(m_launched != NULL)
        return m_launched->update();

    if(global::evs->keyJustPressed(events::KeyMap::Escape)
            || m_back->clicked())
        return false;

    std::string entered = m_charas->entered();
    if(!entered.empty() && m_act < m_nb) {
        gameplay::Character* c = (gameplay::Character*)m_charas->selectedData();
        while(c == NULL) { /* Means random character. */
            size_t pos = rand() % m_avail.size();
            c = (gameplay::Character*)m_charas->getData(m_charas->item(pos));
        }

        m_sels[m_act] = c->clone();
        ++m_act;
        updateTitle();

        global::audio->enterNamespace("/menubutton");
        global::audio->play("click");
    }

    if(m_charas->moved()) {
        m_timem = SDL_GetTicks();
        updateDesc();
    }

    Uint32 lapsed = SDL_GetTicks() - m_timem;
    if(lapsed > 2000 && m_showing) {
        m_showing = false;
        m_layout->removeWidget(2, 1);
        m_layout->addWidget(m_desc, 2, 1, 0, 3);
    }
    else if(lapsed < 2000 && !m_showing) {
        m_showing = true;
        m_layout->removeWidget(2, 1);
        m_layout->addWidget(m_prev, 2, 1, 0, 3);
    }

    if(m_cancel->clicked() && m_act > 0) {
        --m_act;
        delete m_sels[m_act];
        m_sels[m_act] = NULL;
        updateTitle();

        global::audio->enterNamespace("/menubutton");
        global::audio->play("click");
    }

    if(m_play->clicked() && m_act == m_nb) {
        gameplay::Controler* ctrls[4];
        for(int i = 0; i < 4; ++i) {
            if(i >= m_act) {
                ctrls[i] = NULL;
                continue;
            }
            ctrls[i] = new gameplay::Controler(m_ctrls[i]);
            ctrls[i]->attach(m_sels[i]);
            lua::exposure::characters[i] = m_sels[i];
            if(!m_sels[i]->load(gameplay::Character::Color::None, i)) {
                core::logger::logm("Couldn't launch game because couldn't load a character.", core::logger::ERROR);
                return false;
            }
        }

        m_launched = new StageSelMenu(ctrls);
        m_launched->prepare();
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

void CharaSelMenu::updateTitle()
{
    std::ostringstream oss;
    oss << _i("Selecting player ") << m_act << _i(" character.");
    m_title->setText(oss.str());
}

void CharaSelMenu::updateDesc()
{
    gameplay::Character* sel = (gameplay::Character*)m_charas->selectedData();
    if(sel == NULL) {
        m_desc->setText(_i("Pic up a random character."));
        m_prev->set(NULL);
        return;
    }
    m_desc->setText(sel->desc());
    m_prev->set(sel);
}

void CharaSelMenu::loadCharas()
{
    std::string path = global::cfg->get<std::string>("rcs") + "/chara/";
    std::vector<std::string> elems = core::path::dirContents(path);
    m_avail.clear();
    m_avail.reserve(elems.size());

    for(std::string elem : elems) {
        std::string dir = path + elem;
        if(core::path::type(dir) == core::path::Type::Dir) {
            gameplay::Character* chara = new gameplay::Character(dir);
            if(chara->preload())
                m_avail.push_back(chara);
            else {
                std::ostringstream oss;
                oss << "Couldn't preload character \"" << dir << "\".\n";
                core::logger::logm(oss.str(), core::logger::WARNING);
                delete chara;
            }
        }
    }
}


