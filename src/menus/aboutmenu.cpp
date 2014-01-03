
#include "aboutmenu.hpp"
#include "buttonmenu.hpp"
#include "mainmenu.hpp"
#include "global.hpp"
#include "core/i18n.hpp"


AboutMenu::AboutMenu()
    : Menu(), m_back(NULL), m_about(NULL), m_frame(NULL), m_lay(NULL)
{}

AboutMenu::~AboutMenu()
{
    if(m_back)
        delete m_back;
    if(m_about)
        delete m_about;
    if(m_frame)
        delete m_frame;
    if(m_lay)
        delete m_lay;
}

bool AboutMenu::prepare()
{
    /* Load the textures if necessary. */
    global::theme->guiNamespace();
    if(global::gfx->rctype("aboutmenubg") != graphics::Graphics::TEXT) {
        std::ostringstream path;
        path << global::cfg->get<std::string>("rcs") << "/aboutmenu/";
        if(!global::gfx->loadTexture("aboutmenubg", path.str() + "bg.png"))
            return false;
        if(!MainMenu::loadRcs())
            return false;
    }

    /* Create the widgets if necessary. */
    if(!m_lay) {
        m_back = new gui::Button(global::gfx);
        m_back->text(_i("Back to main menu"));
        global::theme->apply(m_back);

        m_about = new gui::Text(global::gfx);
        m_about->setText(_i("\t\tProject Warrior\n"
                    "\tThis game is a fighting game occuring in a brand new original universe,"
                    "with gameplay inspired by Super Smash Bros. games from Nintendo.\n"
                    "\n\tThe game is released under GNU GPLv3 licence. You should have had an copy"
                    " of the licence with this program. If not, please contact the developpers"
                    " at <luc.linux@mailoo.org>.\n"
                    "\n\tThis game has been created by two french programmers : Luc Chabassier alias Lucas8"
                    " and Pablo Donato alias Champitoad."
                    ));
        global::theme->apply(m_about);

        m_frame = new gui::Frame(global::gfx);
        global::theme->apply(m_frame);
        m_frame->setBg("aboutmenubg", m_frame->strictBg());
        m_frame->set(m_about);

        m_lay = new gui::GridLayout(global::gfx, 1, 10);
        global::theme->apply(m_lay);
        m_lay->addWidget(m_frame, 0, 0, 0, 8);
        m_lay->addWidget(m_back,  0, 9, 0, 0);
    }

    /* Set up the gui. */
    geometry::Point pos;
    global::gfx->disableVirtualSize();
    pos.x = global::gfx->getVirtualWidth() * 0.1f;
    pos.y = global::gfx->getVirtualHeight() * 0.1f;
    global::gui->main(m_lay, pos, global::gfx->getVirtualWidth() * 0.8f, global::gfx->getVirtualHeight() * 0.8f);

    return true;
}

bool AboutMenu::update()
{
    global::gui->update(*global::evs);
    if(m_back->clicked()) {
        ButtonMenu::click();
        return false;
    }

    /* Drawing, use the bg of the mainmenu */
    global::gfx->enterNamespace("/mainmenu");
    geometry::AABB rect(global::gfx->getVirtualWidth(), global::gfx->getVirtualHeight());

    global::gfx->beginDraw();
    global::gfx->draw(rect, "bg");
    global::theme->guiNamespace();
    global::gui->draw();
    global::gfx->endDraw();

    return true;
}

