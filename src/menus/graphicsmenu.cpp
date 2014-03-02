
#include "graphicsmenu.hpp"
#include "buttonmenu.hpp"
#include "mainmenu.hpp"
#include "global.hpp"
#include "core/i18n.hpp"
#include <sstream>

GraphicsMenu::GraphicsMenu()
    : Menu(), m_res(NULL), m_fs(NULL), m_back(NULL), m_apply(NULL), m_layout(NULL)
{}

GraphicsMenu::~GraphicsMenu()
{
    if(m_res) {
        m_res->freeData([] (void* data) { delete (geometry::AABB*)data; });
        delete m_res;
    }
    if(m_fs)
        delete m_fs;
    if(m_back)
        delete m_back;
    if(m_apply)
        delete m_apply;
    if(m_layout)
        delete m_layout;
}

bool GraphicsMenu::prepare()
{
    /* Create the widgets if necessary. */
    if(!m_layout) {
        m_back = new gui::Button(global::gfx); 
        m_back->text(_i("Back"));
        global::theme->apply(m_back);
        m_apply = new gui::Button(global::gfx);
        m_apply->text(_i("Apply"));
        global::theme->apply(m_apply);

        m_fs = new gui::CheckBox(global::gfx);
        m_fs->label(_i("Fullscreen"));
        global::theme->apply(m_fs);

        m_res = new gui::List(global::gfx);
        global::theme->apply(m_res);
        /* Populate m_res. */
        std::vector<geometry::AABB> res = global::gfx->windowRes(800, 600);
        for(size_t i = 0; i < res.size(); ++i) {
            std::ostringstream label;
            label << res[i].width << "x" << res[i].height << std::endl;
            m_res->addItem(i, label.str(), 0.0f, new geometry::AABB(res[i]));
        }

        m_layout = new gui::GridLayout(global::gfx, 2, 3);
        global::theme->apply(m_layout);
        m_layout->addWidget(m_res,     0, 0, 0, 2);
        m_layout->addWidget(m_fs,      1, 0, 0, 0);
        m_layout->addWidget(m_apply,   1, 1, 0, 0);
        m_layout->addWidget(m_back,    1, 2, 0, 0);
    }

    if(!MainMenu::loadRcs())
        return false;

    /* Default value of m_fs. */
    if(global::cfg->get<bool>("fullscreen"))
        m_fs->set(true);
    else
        m_fs->set(false);

    /* Set up the gui. */
    geometry::Point pos;
    global::gfx->disableVirtualSize();
    pos.x = global::gfx->getVirtualWidth() * 0.1f;
    pos.y = global::gfx->getVirtualHeight() * 0.1f;
    global::gui->main(m_layout, pos, global::gfx->getVirtualWidth() * 0.8f, global::gfx->getVirtualHeight() * 0.8f);

    return true;
}

bool GraphicsMenu::update()
{
    if(global::evs->keyJustPressed(events::KeyMap::Escape)
            || m_back->clicked()) {
        ButtonMenu::click();
        return false;
    }

    /* Applying */
    if(m_apply->clicked()) {
        ButtonMenu::click();

        if(m_fs->get()) {
            global::cfg->set<bool>("fullscreen", true);
            global::gfx->setFullscreen(true);
        }
        else {
            global::cfg->set<bool>("fullscreen", false);
            global::gfx->setFullscreen(false);
            geometry::AABB size = *(geometry::AABB*)m_res->selectedData();
            global::gfx->windowSize((int)size.width, (int)size.height);
        }

        /* Re-loading gui theme. */
        /** @todo Fatal error. */
        if(!global::theme->load())
            core::logger::logm("Couldn't load again the gui theme.", core::logger::ERROR);

        global::gfx->disableVirtualSize();

        return false;
    }

    /* Drawing */
    global::gfx->enterNamespace("/mainmenu");
    geometry::AABB rect(global::gfx->getVirtualWidth(), global::gfx->getVirtualHeight());

    global::gfx->beginDraw();
    global::gfx->draw(rect, "bg");
    global::theme->guiNamespace();
    global::gui->draw();
    global::gfx->endDraw();

    return true;
}

