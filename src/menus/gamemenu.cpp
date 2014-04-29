
#include "gamemenu.hpp"
#include "global.hpp"

GameMenu::GameMenu(gameplay::Stage* st)
    : m_stage(st)
{}

GameMenu::~GameMenu()
{
}

bool GameMenu::prepare()
{
    /* Nothing to do. */
    return true;
}

bool GameMenu::update()
{
    if(global::evs->keyJustPressed(events::KeyMap::Escape)
            || global::evs->keyJustPressed(events::Key('q')))
        return false;

    m_stage->update(*global::evs);
    global::gfx->beginDraw();
    m_stage->draw();
    global::gfx->endDraw();
    return true;
}

