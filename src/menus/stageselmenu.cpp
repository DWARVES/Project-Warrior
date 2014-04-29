
#include "stageselmenu.hpp"
#include "global.hpp"
#include "core/i18n.hpp"
#include "core/logger.hpp"
#include "core/pathParser.hpp"
#include <sstream>

    StageSelMenu::StagePreview::StagePreview()
: Widget(global::gfx), m_stage(NULL)
{}

StageSelMenu::StagePreview::~StagePreview()
{}

void StageSelMenu::StagePreview::set(gameplay::Stage* st)
{
    m_stage = st;
}

void StageSelMenu::StagePreview::draw()
{
    if(!m_stage)
        return;
    geometry::AABB rect(width(), height());
    m_stage->draw(rect);
}

    StageSelMenu::List::List()
: gui::List(global::gfx), m_moved(true)
{}

StageSelMenu::List::~List()
{
    freeData([] (void* d) { delete (gameplay::Stage*)d; } );
}

void StageSelMenu::List::select()
{
    m_moved = true;
}

bool StageSelMenu::List::moved()
{
    bool ret = m_moved;
    m_moved = false;
    return ret;
}

StageSelMenu::StageSelMenu(gameplay::Controler* charas[4])
    : m_layout(NULL), m_list(NULL), m_play(NULL), m_back(NULL), m_prev(NULL), m_menu(NULL)
{
    for(int i = 0; i < 4; ++i)
        m_charas[i] = charas[i];
}

StageSelMenu::~StageSelMenu()
{
    if(m_layout != NULL)
        delete m_layout;
    if(m_list != NULL)
        delete m_list;
    if(m_play != NULL)
        delete m_play;
    if(m_back != NULL)
        delete m_back;
    if(m_prev != NULL)
        delete m_prev;
    if(m_menu != NULL)
        delete m_menu;
}

bool StageSelMenu::prepare()
{
    if(m_layout == NULL) {
        /* Allocating the widgets. */
        m_layout = new gui::GridLayout(global::gfx, 3, 6);
        m_list = new List;
        m_play = new gui::Button(global::gfx); m_play->text(_i("Play"));
        m_back = new gui::Button(global::gfx); m_back->text(_i("Back"));
        m_prev = new StagePreview;

        /* Applying the theme. */
        global::theme->apply(m_layout);
        global::theme->apply(m_list);
        global::theme->apply(m_play);
        global::theme->apply(m_back);

        /* Populating the list. */
        populateList();

        /* Setting up the layout. */
        m_layout->addWidget(m_list, 0, 0, 1, 5);
        m_layout->addWidget(m_play, 2, 0, 0, 0);
        m_layout->addWidget(m_back, 2, 1, 0, 0);
        m_layout->addWidget(m_prev, 2, 2, 0, 3);
    }

    /* Setting up the gui. */
    geometry::Point pos;
    global::gfx->disableVirtualSize();
    pos.x = global::gfx->getVirtualWidth() * 0.1f;
    pos.y = global::gfx->getVirtualHeight() * 0.1f;
    global::gui->main(m_layout, pos, global::gfx->getVirtualWidth() * 0.8f, global::gfx->getVirtualHeight() * 0.8f);
    return true;
}

bool StageSelMenu::update()
{
    if(m_menu != NULL) {
        if(!m_menu->update()) {
            global::gfx->disableVirtualSize();
            global::gfx->invertYAxis(false);
            delete m_menu;
            m_menu = NULL;
            return false;
        }
        return true;
    }

    if(global::evs->keyJustPressed(events::KeyMap::Escape)
            || m_back->clicked())
        return false;

    if(m_list->moved()) 
        m_prev->set((gameplay::Stage*)m_list->selectedData());

    /** @todo click on play. */
    if(m_play->clicked()) {
        gameplay::Stage* sel = (gameplay::Stage*)m_list->selectedData();
        if(!sel->load(m_charas)) {
            std::ostringstream oss;
            core::logger::logm("Couldn't launch the game because couldn't load the stage.", core::logger::ERROR);
            return false;
        }
        m_menu = new GameMenu(sel);
        m_menu->prepare();
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

void StageSelMenu::populateList()
{
    std::string path = global::cfg->get<std::string>("rcs") + "/stages/";
    std::vector<std::string> contents = core::path::dirContents(path);

    int pos = 0;
    for(std::string elem : contents) {
        std::string dir = path + elem;
        if(core::path::type(dir) == core::path::Type::Dir) {
            gameplay::Stage* st = new gameplay::Stage(dir);
            if(!st->preload()) {
                std::ostringstream oss;
                oss << "Couldn't load " << dir << " as a stage.";
                core::logger::logm(oss.str(), core::logger::WARNING);
                delete st;
                continue;
            }
            m_list->addItem(pos, st->name(), 0.0f, (void*)st);
            ++pos;
        }
    }
}


