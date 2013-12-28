
#include "musicmenu.hpp"
#include "buttonmenu.hpp"
#include "global.hpp"
#include "core/i18n.hpp"

MusicMenu::MusicMenu()
    : Menu(), m_sound(NULL), m_music(NULL), m_sndVol(NULL), m_musVol(NULL), m_back(NULL), m_layout(NULL)
{}

MusicMenu::~MusicMenu()
{
    if(m_sound)
        delete m_sound;
    if(m_music)
        delete m_music;
    if(m_sndVol)
        delete m_sndVol;
    if(m_musVol)
        delete m_musVol;
    if(m_back)
        delete m_back;
    if(m_layout)
        delete m_layout;
}

bool MusicMenu::prepare()
{
    /* Create the widgets if necessary. */
    if(!m_layout) {
        m_sound = new gui::CheckBox(global::gfx);
        m_sound->label(_i("Sounds"));
        global::theme->apply(m_sound);
        m_music = new gui::CheckBox(global::gfx);
        m_music->label(_i("Music"));
        global::theme->apply(m_music);

        m_sndVol = new gui::FillBar(global::gfx);
        m_sndVol->size(255);
        m_sndVol->dir(false);
        global::theme->apply(m_sndVol);
        m_sndVol->step(5);
        m_musVol = new gui::FillBar(global::gfx);
        m_musVol->size(255);
        m_musVol->dir(false);
        global::theme->apply(m_musVol);
        m_musVol->step(5);

        m_back = new gui::Button(global::gfx);
        m_back->text(_i("Back"));
        global::theme->apply(m_back);

        m_layout = new gui::GridLayout(global::gfx, 1, 5);
        global::theme->apply(m_layout);
        m_layout->addWidget(m_music,  0, 0);
        m_layout->addWidget(m_musVol, 0, 1);
        m_layout->addWidget(m_sound,  0, 2);
        m_layout->addWidget(m_sndVol, 0, 3);
        m_layout->addWidget(m_back,   0, 4);
    }

    /* Get config. */
    unsigned char volume = global::audio->musicVolume();
    if(volume == 0)
        m_music->set(false);
    else
        m_music->set(true);
    m_musVol->set(volume);
    volume = global::audio->soundsVolume();
    if(volume == 0)
        m_sound->set(false);
    else
        m_sound->set(true);
    m_sndVol->set(volume);

    /* Set up the gui. */
    geometry::Point pos;
    global::gfx->disableVirtualSize();
    pos.x = global::gfx->getVirtualWidth() * 0.1f;
    pos.y = global::gfx->getVirtualHeight() * 0.1f;
    global::gui->main(m_layout, pos, global::gfx->getVirtualWidth() * 0.8f, global::gfx->getVirtualHeight() * 0.8f);

    return true;
}

bool MusicMenu::update()
{
    global::gui->update(*global::evs);
    if(global::evs->keyJustPressed(events::KeyMap::Escape)
            || m_back->clicked()) {
        ButtonMenu::click();

        /* Saving */
        if(!m_music->get())
            global::cfg->set<int>("music", 0);
        else
            global::cfg->set<int>("music", (int)m_musVol->get());
        if(!m_sound->get())
            global::cfg->set<int>("sounds", 0);
        else
            global::cfg->set<int>("sounds", (int)m_sndVol->get());
        return false;
    }

    /* Applying */
    if(!m_music->get())
        global::audio->musicVolume(0);
    else
        global::audio->musicVolume((unsigned char)m_musVol->get());
    if(!m_sound->get())
        global::audio->soundsVolume(0);
    else
        global::audio->soundsVolume((unsigned char)m_sndVol->get());

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

