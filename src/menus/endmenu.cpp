
#include "endmenu.hpp"
#include "mainmenu.hpp"
#include "global.hpp"
#include "core/i18n.hpp"

EndMenu::EndMenu(gameplay::Character* pl[4])
    : m_layout(global::gfx), m_end(global::gfx)
{
    for(int i = 0; i < 4; ++i) {
        m_players[i] = pl[i];
        m_scores[i] = new gui::Text(global::gfx);
    }
}

EndMenu::~EndMenu()
{
    for(int i = 0; i < 4; ++i)
        delete m_scores[i];
}

bool EndMenu::prepare()
{
    /* Sorting the players. */
    std::vector<std::pair<int,int>> scrs;
    scrs.resize(4);
    for(size_t i = 0; i < 4; ++i) {
        scrs[i].second = (int)i;
        if(m_players[i])
            scrs[i].first = m_players[i]->getPoints();
        else
            scrs[i].first  = std::numeric_limits<int>::min();
    }

    std::sort(scrs.begin(), scrs.end(), [] (std::pair<int,int> i, std::pair<int,int>j) { return i.first < j.first; });
    for(size_t i = 0; i < 4; ++i) {
        if(m_players[scrs[i].second]) {
            std::ostringstream oss;
            oss << i << _i("eme : Player ") << scrs[i].second << std::endl;
            m_scores[i]->setText(oss.str());
        }
        else
            m_scores[i]->setText(_i("No player."));
    }

    /* Setting up the gui. */
    geometry::Point pos;
    global::gfx->disableVirtualSize();
    global::gfx->invertYAxis(false);
    pos.x = global::gfx->getVirtualWidth() / 3.0f;
    pos.y = global::gfx->getVirtualHeight() * 0.1f;
    global::gui->main(&m_layout, pos, global::gfx->getVirtualWidth() / 3.0f, global::gfx->getVirtualHeight() * 0.8f);

    m_layout.setSize(1, 5);
    global::theme->apply(&m_layout);
    for(unsigned int i = 0; i < 4; ++i) {
        global::theme->apply(m_scores[i]);
        m_layout.addWidget(m_scores[i], 0, i);
    }
    global::theme->apply(&m_end);
    m_end.text(_i("End"));
    m_layout.addWidget(&m_end, 0, 4);

    if(!MainMenu::loadRcs())
        return false;

    return true;
}

bool EndMenu::update()
{
    if(global::evs->keyJustPressed(events::KeyMap::Escape)
            || m_end.clicked())
        return false;

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

