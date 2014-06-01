
#include "gamemenu.hpp"
#include "global.hpp"
#include "core/i18n.hpp"

/** @brief The duration of a game in ms. */
const Uint32 gameDuration = 180000;

GameMenu::GameMenu(gameplay::Stage* st)
    : m_stage(st), m_duration(gameDuration), m_first(true)
{
    for(int i = 0; i < 4; ++i)
        m_pl[i] = m_stage->getCharacter(i);
}

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
        path << global::cfg->get<std::string>("rcs") << "/gamemenu/dmgfont.wf";
        if(!global::gfx->loadFont("dmgfont", path.str()))
            return false;

        path.str("");
        path << global::cfg->get<std::string>("rcs") << "/gamemenu/stfont.wf";
        if(!global::gfx->loadFont("stfont", path.str()))
            return false;

        path.str("");
        path << global::cfg->get<std::string>("rcs") << "/gamemenu/stun.png";
        if(!global::gfx->loadTexture("stun", path.str()))
            return false;

        path.str("");
        path << global::cfg->get<std::string>("rcs") << "/gamemenu/mana.png";
        if(!global::gfx->loadTexture("mana", path.str()))
            return false;
    }

    return true;
}

bool GameMenu::update()
{
    std::string status;
    float prg = m_stage->appearProgress();
    if(prg < 1.0f) {
        if(prg < 0.9f)
            status = _i("Ready ...");
        else
            status = _i("Go !");
        m_begin = SDL_GetTicks();
    }
    else if(m_first) {
        m_first = false;
        m_begin = SDL_GetTicks();
        status = "0:00";
    }
    else {
        Uint32 time = SDL_GetTicks() - m_begin;
        time = m_duration - time;
        time /= 1000;
        std::ostringstream oss;
        oss << time/60 << ":" << time%60 << std::endl;
        status = oss.str();
    }

    if(global::evs->keyJustPressed(events::KeyMap::Escape)) {
        m_first = true;
        return false;
    }
    if(SDL_GetTicks() - m_begin > m_duration) {
        m_first = true;
        return false;
    }

    m_stage->update(*global::evs);
    global::gfx->beginDraw();
    m_stage->draw();
    global::gfx->enterNamespace("/gamemenu/");

    /* Drawing the status text on top. */
    /** @todo Center status text. */
    float x = global::gfx->getVirtualWidth() / 2.0f;
    float y = global::gfx->getVirtualHeight() / 5.0f * 4.0f;
    float h = global::gfx->getVirtualHeight() / 15.0f;
    y -= (h / 2.0f);
    global::gfx->push();
    global::gfx->move(x,y);
    global::gfx->draw(status, "stfont", h);
    global::gfx->pop();

    /* Drawing the damages. */
    int i = 0;
    gameplay::Character* act = m_stage->getCharacter(i);
    while(act) {
        /* Drawing the damages. */
        int dms = act->getDamages();
        x = global::gfx->getVirtualWidth() / 6.0f * float(i+1);
        y = global::gfx->getVirtualHeight() / 15.0f;
        h = global::gfx->getVirtualHeight() / 15.0f;
        std::ostringstream text;
        text << dms << "%";
        global::gfx->push();
        global::gfx->move(x, y);
        global::gfx->draw(text.str(), "dmgfont", h);
        global::gfx->pop();

        /* Drawing the mana. */
        float w = global::gfx->getVirtualWidth() / 8.0f;
        x -= w / 2.0f;
        h = global::gfx->getVirtualHeight() / 45.0f;
        y = global::gfx->getVirtualHeight() / 15.0f;
        h *= act->manaProgress();
        y -= h;
        global::gfx->push();
        global::gfx->move(x,y);
        global::gfx->draw(geometry::AABB(w,h), "mana", 1.0f, act->manaProgress());
        global::gfx->pop();

        /* Drawing the CCs. */
        w = global::gfx->getVirtualWidth() / 45.0f;
        x = global::gfx->getVirtualWidth() / 6.0f * float(i+1);
        y = global::gfx->getVirtualHeight() / 15.0f;
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

