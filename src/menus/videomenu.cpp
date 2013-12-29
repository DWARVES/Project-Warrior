
#include "videomenu.hpp"
#include "global.hpp"
#include "core/pathParser.hpp"
#include <sstream>
#include <SDL.h>

VideoMenu::VideoMenu(const std::string& path)
    : Menu()
{
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
    global::gfx->enterNamespace("/videomenu");
    global::gfx->rewindMovie(m_name);
    global::gfx->disableVirtualSize();
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


