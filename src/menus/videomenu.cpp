
#include "videomenu.hpp"
#include "global.hpp"
#include "core/pathParser.hpp"
#include <sstream>
#include <SDL.h>

VideoMenu::VideoMenu(const std::string& path, const std::string& music)
    : Menu()
{
    /* Loading the video. */
    if(!global::gfx->enterNamespace("/videomenu")) {
        if(!global::gfx->createNamespace("/videomenu"))
            return;
        global::gfx->enterNamespace("/videomenu");
    }

    std::ostringstream name;
    name << core::path::head(path) << "_" << SDL_GetTicks();
    if(!global::gfx->loadMovie(name.str(), path))
        return;
    m_name = name.str();

    /* Loading the music. */
    if(music.empty())
        return;
    if(!global::audio->enterNamespace("/videomenu")) {
        if(!global::audio->createNamespace("/videomenu"))
            return;
        global::audio->enterNamespace("/videomenu");
    }

    name.clear();
    name << core::path::head(music) << "_" << SDL_GetTicks();
    if(!global::audio->loadRawMusic(name.str(), music))
        return;
    m_audio = name.str();
}

VideoMenu::~VideoMenu()
{
    if(!m_name.empty()) {
        global::gfx->enterNamespace("/videomenu");
        global::gfx->free(m_name);
    }
}

bool VideoMenu::prepare()
{
    if(m_name.empty())
        return false;
    /* Preparing the video. */
    global::gfx->enterNamespace("/videomenu");
    global::gfx->rewindMovie(m_name);
    global::gfx->disableVirtualSize();
    /* Preparing the music. */
    if(!m_audio.empty()) {
        global::audio->enterNamespace("/videomenu");
        global::audio->play(m_audio);
    }
    return true;
}

bool VideoMenu::update()
{
    global::gfx->enterNamespace("/videomenu");

    if(!global::evs->lastKeysPressed().empty()
            || !global::evs->lastJoyButtonsPressed().empty())
        return false;

    geometry::AABB rect(global::gfx->getVirtualWidth(), global::gfx->getVirtualHeight());
    graphics::Color bg(0, 0, 0);

    global::gfx->beginDraw();
    global::gfx->draw(rect, bg);
    bool ret = global::gfx->play(m_name, rect, true);
    global::gfx->endDraw();

    return ret;
}


