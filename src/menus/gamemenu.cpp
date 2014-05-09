
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
    if(!global::gfx->existsNamespace("/gamemenu")) {
        if(!global::gfx->createNamespace("/gamemenu"))
            return false;
        global::gfx->enterNamespace("/gamemenu");

        std::ostringstream path;
        path << global::cfg->get<std::string>("rcs") << "/gamemenu/font.wf";
        if(!global::gfx->loadFont("dmgfont", path.str()))
            return false;

        path.str("");
        path << global::cfg->get<std::string>("rcs") << "/gamemenu/stun.png";
        if(!global::gfx->loadTexture("stun", path.str()))
            return false;
    }

    /* The gui isn't used during the game. */
    global::gui->focus(false);
    return true;
}

bool GameMenu::update()
{
    if(global::evs->keyJustPressed(events::KeyMap::Escape)
            || global::evs->keyJustPressed(events::Key('q'))) {
        global::gui->focus(true);
        return false;
    }

    m_stage->update(*global::evs);
    global::gfx->beginDraw();
    m_stage->draw();

    /* Drawing the damages. */
    int i = 0;
    gameplay::Character* act = m_stage->getCharacter(i);
    global::gfx->enterNamespace("/gamemenu/");
    while(act) {
        /* Drawing the damages. */
        int dms = act->getDamages();
        float x = global::gfx->getVirtualWidth() / 6.0f * float(i+1);
        float y = global::gfx->getVirtualHeight() / 15.0f;
        float h = global::gfx->getVirtualHeight() / 15.0f;
        std::ostringstream text;
        text << dms << "%";
        global::gfx->push();
        global::gfx->move(x, y);
        global::gfx->draw(text.str(), "dmgfont", h);
        global::gfx->pop();

        /* Drawing the CCs. */
        float w = global::gfx->getVirtualWidth() / 45.0f;
        x -= w / 2.0f;
        y += w / 2.0f;
        float fact = 1.0f - act->stunProgress();
        if(fact > 1e-4f) {
            h = fact * w;
            global::gfx->push();
            global::gfx->move(x, y);
            global::gfx->move(0.0f, (w-h)/2.0f);
            global::gfx->move(-w/2.0f, -h/2.0f);
            global::gfx->draw(geometry::AABB(w,h), "stun", 1.0f, fact);
            global::gfx->pop();
        }

        ++i;
        act = m_stage->getCharacter(i);
    }

    global::gfx->endDraw();
    return true;
}

