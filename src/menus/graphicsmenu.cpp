
#include "graphicsmenu.hpp"
#include "global.hpp"
#include "core/i18n.hpp"
#include <sstream>

GraphicsMenu::GraphicsMenu()
    : Menu(), m_res(NULL), m_fs(NULL), m_fslabel(NULL), m_back(NULL), m_apply(NULL), m_layout(NULL)
{}

GraphicsMenu::~GraphicsMenu()
{
    if(m_res)
        delete m_res;
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
        global::theme->apply(m_fs);
        m_fslabel = new gui::Text(global::gfx);
        m_fslabel->setText(_i("Fullscreen"));
        global::theme->apply(m_fslabel);

        m_res = new gui::List(global::gfx);
        global::theme->apply(m_res);

        m_layout = new gui::GridLayout(global::gfx, 4, 3);
        global::theme->apply(m_layout);
        m_layout->addWidget(m_res,     0, 0, 1, 2);
        m_layout->addWidget(m_fs,      2, 0, 0, 0);
        m_layout->addWidget(m_fslabel, 3, 0, 0, 0);
        m_layout->addWidget(m_apply,   2, 1, 1, 0);
        m_layout->addWidget(m_back,    2, 2, 1, 0);
    }

    /* Populate m_res. */
    std::vector<geometry::AABB> res = global::gfx->windowRes(800, 600);
    for(size_t i = 0; i < res.size(); ++i) {
        std::ostringstream label;
        label << res[i].width << "x" << res[i].height << std::endl;
        m_res->addItem(i, label.str());
    }

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
    global::gui->update(*global::evs);
    if(global::evs->keyJustPressed(events::KeyMap::Escape)
            || m_back->clicked()) {
        if(global::audio->enterNamespace("/menubutton")
                && global::audio->rctype("click") == audio::Audio::SOUND)
            global::audio->play("click");
        return false;
    }

    /* Applying */
    if(m_apply->clicked()) {
        if(global::audio->enterNamespace("/menubutton")
                && global::audio->rctype("click") == audio::Audio::SOUND)
            global::audio->play("click");

        /** @todo Reoppening the window with the same gl context. */
        if(m_fs->get()) {
            global::cfg->set<bool>("fullscreen", true);
        }
        else {
            global::cfg->set<bool>("fullscreen", false);
            /** @todo Get the selected size. */
        }

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

