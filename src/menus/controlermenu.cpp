
#include "controlermenu.hpp"
#include "core/logger.hpp"
#include "core/i18n.hpp"
#include "global.hpp"
#include <sstream>

    ControlerMenu::ControlerMenu(const std::string& id)
: m_id(id), m_joy(NULL), m_ctrl(id), m_plugged(false),
    m_ctrls(NULL), m_back(NULL), m_text(NULL), m_layout(NULL)
{
    if(m_ctrl.isOpen()) {
        if(m_id != "keyboard")
            m_joy = global::evs->joyFromName(id);
        m_plugged = true;
    }
}

ControlerMenu::~ControlerMenu()
{
    if(m_ctrls)
        delete m_ctrls;
    if(m_back)
        delete m_back;
    if(m_text)
        delete m_text;
    if(m_layout)
        delete m_layout;
}

bool ControlerMenu::prepare()
{
    if(!m_layout) {
        m_layout = new gui::GridLayout(global::gfx);
        global::theme->apply(m_layout);

        /* If the controler is plugged. */
        if(m_plugged) {
            m_back = new gui::Button(global::gfx);
            m_back->text("Back");
            global::theme->apply(m_back);
            m_text = new gui::Text(global::gfx);
            global::theme->apply(m_text);

            m_ctrls = new List(m_text, &m_ctrl, m_joy);
            global::theme->apply(m_ctrls);
            m_ctrls->addItem( 0, _i("Left"),      0.0f, _i("The left direction."));
            m_ctrls->addItem( 1, _i("Right"),     0.0f, _i("The right direction."));
            m_ctrls->addItem( 2, _i("Up"),        0.0f, _i("Pointing upward."));
            m_ctrls->addItem( 3, _i("Down"),      0.0f, _i("Pointing Downward."));
            m_ctrls->addItem( 4, _i("Run Left"),  0.0f, _i("Running to the left."));
            m_ctrls->addItem( 5, _i("Run Right"), 0.0f, _i("Running to the right."));
            m_ctrls->addItem( 6, _i("Attack"),    0.0f, _i("The basic attack control."));
            m_ctrls->addItem( 7, _i("Special"),   0.0f, _i("The special/magic attack control."));
            m_ctrls->addItem( 8, _i("Dodge"),     0.0f, _i("Dodging allows you to evades attacks."));
            m_ctrls->addItem( 9, _i("Catch"),     0.0f, _i("Catching an opponent allows you to hit him and throw him elsewhere."));
            m_ctrls->addItem(10, _i("Jump"),      0.0f, _i("The jump control."));
            m_text->setText((const char*)m_ctrls->getData(m_ctrls->item(0)));

            m_layout->setSize(4, 3);
            m_layout->addWidget(m_ctrls, 0, 0, 2, 2);
            m_layout->addWidget(m_back,  3, 0, 0, 0);
            m_layout->addWidget(m_text,  3, 1, 0, 1);
            updatePrinted();
        }
        /* If the controler isn't plugged. */
        else {
            std::ostringstream oss;
            oss << _i("The controler \"") << m_id << _i("\" isn't plugged, can't be configured.");
            m_text = new gui::Text(global::gfx);
            m_text->setText(oss.str());
            global::theme->apply(m_text);
            m_back = new gui::Button(global::gfx);
            m_back->text(_i("Back to config menu."));
            global::theme->apply(m_back);

            m_layout->setSize(1, 2);
            m_layout->addWidget(m_text, 0,0);
            m_layout->addWidget(m_back, 0,1);
        }
    }

    /* Set up the gui. */
    geometry::Point pos;
    global::gfx->disableVirtualSize();
    pos.x = global::gfx->getVirtualWidth() * 0.1f;
    pos.y = global::gfx->getVirtualHeight() * 0.2f;
    global::gui->main(m_layout, pos, global::gfx->getVirtualWidth() * 0.8f, global::gfx->getVirtualHeight() * 0.7f);

    return true;
}

bool ControlerMenu::update()
{
    global::gui->update(*global::evs);
    if(global::evs->keyJustPressed(events::KeyMap::Escape)
            || m_back->clicked())
        return false;

    /** @todo Config. */

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

void ControlerMenu::updatePrinted() const
{
    for(size_t i = 0; i < 11; ++i) {
        std::ostringstream oss;
        oss << gameplay::Controler::controlName((gameplay::Controler::Controls)i) << " : " << m_ctrl.get((gameplay::Controler::Controls)i);
        m_ctrls->setItem(m_ctrls->item(i), oss.str());
    }
}

ControlerMenu::List::List(gui::Text* text, gameplay::Controler* ctrl, events::Joystick* joy)
    : gui::List(global::gfx), m_text(text), m_ctrl(ctrl), m_joy(joy)
{}

ControlerMenu::List::~List()
{}

void ControlerMenu::List::select()
{
    const char* data = (const char*)selectedData();
    m_text->setText(data);
}

void ControlerMenu::List::enter()
{
}

events::EvSave* ControlerMenu::List::getEvent()
{
    return NULL;
}


