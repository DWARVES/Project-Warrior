
#include "theme.hpp"

/* TODO config file for non-pictures */

namespace gui
{
    Theme::Theme(graphics::Graphics* gfx, const std::string& path)
        : m_gfx(gfx), m_path(path)
    {}

    bool Theme::load()
    {
        bool ret = true;

        /* Global */
        std::string name = m_gfx->actualNamespace();
        m_gfx->enterNamespace("/");
        m_gfx->createNamespace("gui");
        m_gfx->enterNamespace("gui");

        /* FillBar */
        ret = ret && m_gfx->loadTexture("fillbar_empty",  m_path + "/fillbar/empty.png");
        ret = ret && m_gfx->loadTexture("fillbar_full",   m_path + "/fillbar/full.png");
        ret = ret && m_gfx->loadTexture("fillbar_emptyS", m_path + "/fillbar/emptyS.png");
        ret = ret && m_gfx->loadTexture("fillbar_fullS",  m_path + "/fillbar/fullS.png");

        /* CheckBox */
        ret = ret && m_gfx->loadTexture("checkbox_nsel",  m_path + "/checkbox/nsel.png");
        ret = ret && m_gfx->loadTexture("checkbox_sel",   m_path + "/checkbox/sel.png");
        ret = ret && m_gfx->loadTexture("checkbox_nselS", m_path + "/checkbox/nselS.png");
        ret = ret && m_gfx->loadTexture("checkbox_selS",  m_path + "/checkbox/selS.png");

        /* List */
        ret = ret && m_gfx->loadTexture("list_r",  m_path + "/list/r.png");
        ret = ret && m_gfx->loadTexture("list_m",  m_path + "/list/m.png");
        ret = ret && m_gfx->loadTexture("list_l",  m_path + "/list/l.png");
        ret = ret && m_gfx->loadFont   ("list_f",  m_path + "/list/f.png");
        ret = ret && m_gfx->loadTexture("list_rs", m_path + "/list/rs.png");
        ret = ret && m_gfx->loadTexture("list_ms", m_path + "/list/ms.png");
        ret = ret && m_gfx->loadTexture("list_ls", m_path + "/list/ls.png");
        ret = ret && m_gfx->loadFont   ("list_fs", m_path + "/list/fs.png");

        /* Scrollbar */
        ret = ret && m_gfx->loadTexture("scrollbar_up",    m_path + "/scrollbar/up.png");
        ret = ret && m_gfx->loadTexture("scrollbar_down",  m_path + "/scrollbar/down.png");
        ret = ret && m_gfx->loadTexture("scrollbar_norm",  m_path + "/scrollbar/norm.png");
        ret = ret && m_gfx->loadTexture("scrollbar_sel",   m_path + "/scrollbar/sel.png");
        ret = ret && m_gfx->loadTexture("scrollbar_upS",   m_path + "/scrollbar/upS.png");
        ret = ret && m_gfx->loadTexture("scrollbar_downS", m_path + "/scrollbar/downS.png");
        ret = ret && m_gfx->loadTexture("scrollbar_normS", m_path + "/scrollbar/normS.png");
        ret = ret && m_gfx->loadTexture("scrollbar_selS",  m_path + "/scrollbar/selS.png");

        /* Text */
        ret = ret && m_gfx->loadFont("text_font", m_path + "/text/font.png");

        /* Input */
        ret = ret && m_gfx->loadFont("input_font",  m_path + "/input/font.png");

        /* Frame */
        ret = ret && m_gfx->loadTexture("frame_t",  m_path + "/frame/top.png");
        ret = ret && m_gfx->loadTexture("frame_b",  m_path + "/frame/bottom.png");
        ret = ret && m_gfx->loadTexture("frame_l",  m_path + "/frame/left.png");
        ret = ret && m_gfx->loadTexture("frame_r",  m_path + "/frame/right.png");
        ret = ret && m_gfx->loadTexture("frame_tl", m_path + "/frame/tl.png");
        ret = ret && m_gfx->loadTexture("frame_tr", m_path + "/frame/tr.png");
        ret = ret && m_gfx->loadTexture("frame_bl", m_path + "/frame/bl.png");
        ret = ret && m_gfx->loadTexture("frame_br", m_path + "/frame/br.png");
        ret = ret && m_gfx->loadTexture("frame_bg", m_path + "/frame/bg.png");

        /* Button */
        ret = ret && m_gfx->loadTexture("button_r",  m_path + "/button/r.png");
        ret = ret && m_gfx->loadTexture("button_m",  m_path + "/button/m.png");
        ret = ret && m_gfx->loadTexture("button_l",  m_path + "/button/l.png");
        ret = ret && m_gfx->loadFont   ("button_f",  m_path + "/button/f.png");
        ret = ret && m_gfx->loadTexture("button_rf", m_path + "/button/rf.png");
        ret = ret && m_gfx->loadTexture("button_mf", m_path + "/button/mf.png");
        ret = ret && m_gfx->loadTexture("button_lf", m_path + "/button/lf.png");
        ret = ret && m_gfx->loadFont   ("button_ff", m_path + "/button/ff.png");
        ret = ret && m_gfx->loadTexture("button_rs", m_path + "/button/rs.png");
        ret = ret && m_gfx->loadTexture("button_ms", m_path + "/button/ms.png");
        ret = ret && m_gfx->loadTexture("button_ls", m_path + "/button/ls.png");
        ret = ret && m_gfx->loadFont   ("button_fs", m_path + "/button/fs.png");

        /* Radio */
        ret = ret && m_gfx->loadTexture("radio_nsel",  m_path + "/radio/nsel.png");
        ret = ret && m_gfx->loadTexture("radio_sel",   m_path + "/radio/sel.png");
        ret = ret && m_gfx->loadTexture("radio_nselS", m_path + "/radio/nselS.png");
        ret = ret && m_gfx->loadTexture("radio_selS",  m_path + "/radio/selS.png");

        return ret;
    }

    Theme::~Theme()
    {
    }

    void Theme::apply(FillBar* fb)
    {
        fb->setTexture(false, "fillbar_empty",  "fillbar_full");
        fb->setTexture(true,  "fillbar_emptyS", "fillbar_fullS");
    }

    void Theme::apply(CheckBox* cb)
    {
        cb->setTexture(false, "checkbox_nsel",  "checkbox_sel");
        cb->setTexture(true,  "checkbox_nselS", "checkbox_selS");
        /* TODO maxSize */
    }

    void Theme::apply(GridLayout* gl)
    {
        /* TODO gaps */
        if(gl) {} /* Avoid warnings */
    }

    void Theme::apply(List* l)
    {
        l->setPart(List::Right,  false, "list_r");
        l->setPart(List::Middle, false, "list_m");
        l->setPart(List::Left,   false, "list_l");
        l->setPart(List::Font,   false, "list_f");
        l->setPart(List::Right,  true,  "list_rs");
        l->setPart(List::Middle, true,  "list_ms");
        l->setPart(List::Left,   true,  "list_ls");
        l->setPart(List::Font,   true,  "list_fs");
        /* TODO Item size */
    }

    void Theme::apply(ScrollBar* sb)
    {
        sb->setTexture(ScrollBar::Up,       false, "scrollbar_up");
        sb->setTexture(ScrollBar::Down,     false, "scrollbar_down");
        sb->setTexture(ScrollBar::Norm,     false, "scrollbar_norm");
        sb->setTexture(ScrollBar::Selected, false, "scrollbar_sel");
        sb->setTexture(ScrollBar::Up,       true,  "scrollbar_upS");
        sb->setTexture(ScrollBar::Down,     true,  "scrollbar_downS");
        sb->setTexture(ScrollBar::Norm,     true,  "scrollbar_normS");
        sb->setTexture(ScrollBar::Selected, true,  "scrollbar_selS");
    }

    void Theme::apply(Text* t)
    {
        /* TODO font size */
        t->setFont("text_font", 30.0f);
    }

    void Theme::apply(Input* in)
    {
        /* TODO font size */
        in->setFont("input_font", 30.0f);
    }

    void Theme::apply(Frame* fr, bool bg, bool fill)
    {
        fr->set(Frame::Top,         "frame_t");
        fr->set(Frame::Bottom,      "frame_b");
        fr->set(Frame::Left,        "frame_l");
        fr->set(Frame::Right,       "frame_r");
        fr->set(Frame::TopRight,    "frame_tr");
        fr->set(Frame::TopLeft,     "frame_tl");
        fr->set(Frame::BottomRight, "frame_br");
        fr->set(Frame::BottomLeft,  "frame_bl");
        if(bg)
            fr->setBg("frame_bg", fill);
    }

    void Theme::apply(Button* b)
    {
        /* TODO max size */
        b->setTexture(Button::Right,  false, "button_r");
        b->setTexture(Button::Middle, false, "button_m");
        b->setTexture(Button::Left,   false, "button_l");
        b->setTexture(Button::Font,   false, "button_f");
        b->setTexture(Button::Right,  true,  "button_rs");
        b->setTexture(Button::Middle, true,  "button_ms");
        b->setTexture(Button::Left,   true,  "button_ls");
        b->setTexture(Button::Font,   true,  "button_fs");
        b->setSel    (Button::Right,         "button_rf");
        b->setSel    (Button::Middle,        "button_mf");
        b->setSel    (Button::Left,          "button_lf");
        b->setSel    (Button::Font,          "button_ff");
    }
            
    void Theme::apply(Radio* r)
    {
        r->setTexture(false, "radio_nsel",  "radio_sel");
        r->setTexture(true,  "radio_nselS", "radio_selS");
    }
            
    void Theme::guiNamespace()
    {
        m_gfx->enterNamespace("/");
        m_gfx->enterNamespace("gui");
    }

}

