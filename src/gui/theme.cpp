
#include "theme.hpp"
#include <fstream>

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
        ret = ret && m_gfx->loadTexture("list_rf", m_path + "/list/rf.png");
        ret = ret && m_gfx->loadTexture("list_mf", m_path + "/list/mf.png");
        ret = ret && m_gfx->loadTexture("list_lf", m_path + "/list/lf.png");
        ret = ret && m_gfx->loadFont   ("list_ff", m_path + "/list/ff.png");

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

        /* Loading the data */
        m_data.clear();
        std::ifstream ifs(m_path + "/data.fakefs");
        if(!ifs)
            ret = false;
        else {
            if(!m_data.load(ifs, [] (const std::string& str) {
                        std::istringstream iss(str);
                        float v;
                        iss >> v;
                        return v;
                        }))
                ret = false;
            else if(!checkData())
                ret = false;
        }

        if(!ret) {
            m_gfx->enterNamespace("/");
            m_gfx->deleteNamespace("gui");
            m_data.clear();
        }
        return ret;
    }
            
    /* Check if m_data have all the necessary entries */
    bool Theme::checkData()
    {
        if(!m_data.existsNamespace("/fillbar"))
            return false;
        m_data.enterNamespace("/fillbar");
        if(!m_data.existsEntity("ratio"))
            return false;

        if(!m_data.existsNamespace("/checkbox"))
            return false;
        m_data.enterNamespace("/checkbox");
        if(!m_data.existsEntity("width") || !m_data.existsEntity("height"))
            m_cbMs = false;
        else
            m_cbMs = true;
        if(!m_data.existsEntity("square"))
            return false;

        if(!m_data.existsNamespace("/gridlayout"))
            return false;
        m_data.enterNamespace("/gridlayout");
        if(!m_data.existsEntity("gaps"))
            return false;

        if(!m_data.existsNamespace("/list"))
            return false;
        m_data.enterNamespace("/list");
        if(!m_data.existsEntity("itemw") || !m_data.existsEntity("itemh"))
            return false;

        if(!m_data.existsNamespace("/scrollbar"))
            return false;
        m_data.enterNamespace("/scrollbar");
        if(!m_data.existsEntity("ratio"))
            return false;

        if(!m_data.existsNamespace("/text"))
            return false;
        m_data.enterNamespace("/text");
        if(!m_data.existsEntity("font_size"))
            return false;

        if(!m_data.existsNamespace("/input"))
            return false;
        m_data.enterNamespace("/input");
        if(!m_data.existsEntity("font_size"))
            return false;

        if(!m_data.existsNamespace("/frame"))
            return false;
        m_data.enterNamespace("/frame");
        if(!m_data.existsEntity("use_bg")
                || !m_data.existsEntity("bg_fill")
                || !m_data.existsEntity("borders"))
            return false;

        if(!m_data.existsNamespace("/button"))
            return false;
        m_data.enterNamespace("/button");
        if(!m_data.existsEntity("width") || !m_data.existsEntity("height"))
            return false;

        if(!m_data.existsNamespace("/radio"))
            return false;
        m_data.enterNamespace("/radio");
        if(!m_data.existsEntity("width") || !m_data.existsEntity("height"))
            m_rdMs = false;
        else
            m_rdMs = true;
        if(!m_data.existsEntity("square"))
            return false;
        return true;
    }

    Theme::~Theme()
    {
    }

    void Theme::apply(FillBar* fb)
    {
        fb->setTexture(false, "fillbar_empty",  "fillbar_full");
        fb->setTexture(true,  "fillbar_emptyS", "fillbar_fullS");
        m_data.enterNamespace("/fillbar");
        fb->ratio(m_data.getEntityValue("ratio"));
    }

    void Theme::apply(CheckBox* cb)
    {
        cb->setTexture(false, "checkbox_nsel",  "checkbox_sel");
        cb->setTexture(true,  "checkbox_nselS", "checkbox_selS");
        m_data.enterNamespace("/checkbox");
        if(m_cbMs) {
            cb->maxSize(geometry::AABB(m_data.getEntityValue("width"),
                        m_data.getEntityValue("height")));
        }
        cb->square(m_data.getEntityValue("square") > 0.0f);
    }

    void Theme::apply(GridLayout* gl)
    {
        m_data.enterNamespace("/gridlayout");
        gl->gaps(m_data.getEntityValue("gaps"));
    }

    void Theme::apply(List* l)
    {
        l->setPart(List::Right,  List::Norm,     "list_r");
        l->setPart(List::Middle, List::Norm,     "list_m");
        l->setPart(List::Left,   List::Norm,     "list_l");
        l->setPart(List::Font,   List::Norm,     "list_f");
        l->setPart(List::Right,  List::Selected, "list_rs");
        l->setPart(List::Middle, List::Selected, "list_ms");
        l->setPart(List::Left,   List::Selected, "list_ls");
        l->setPart(List::Font,   List::Selected, "list_fs");
        l->setPart(List::Right,  List::Focused,  "list_rf");
        l->setPart(List::Middle, List::Focused,  "list_mf");
        l->setPart(List::Left,   List::Focused,  "list_lf");
        l->setPart(List::Font,   List::Focused,  "list_ff");
        m_data.enterNamespace("/list");
        l->setItemSize(geometry::AABB(m_data.getEntityValue("itemw"),
                    m_data.getEntityValue("itemh")));
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
        m_data.enterNamespace("/scrollbar");
        sb->ratio(m_data.getEntityValue("ratio"));
    }

    void Theme::apply(Text* t)
    {
        m_data.enterNamespace("/text");
        t->setFont("text_font", m_data.getEntityValue("font_size"));
    }

    void Theme::apply(Input* in)
    {
        m_data.enterNamespace("/input");
        in->setFont("input_font", m_data.getEntityValue("font_size"));
    }

    void Theme::apply(Frame* fr)
    {
        fr->set(Frame::Top,         "frame_t");
        fr->set(Frame::Bottom,      "frame_b");
        fr->set(Frame::Left,        "frame_l");
        fr->set(Frame::Right,       "frame_r");
        fr->set(Frame::TopRight,    "frame_tr");
        fr->set(Frame::TopLeft,     "frame_tl");
        fr->set(Frame::BottomRight, "frame_br");
        fr->set(Frame::BottomLeft,  "frame_bl");
        m_data.enterNamespace("/frame");
        fr->border(m_data.getEntityValue("borders"));
        if(m_data.getEntityValue("use_bg") > 0.0f)
            fr->setBg("frame_bg", (m_data.getEntityValue("bg_fill") <= 0.0f));
    }

    void Theme::apply(Button* b)
    {
        b->setTexture(Button::Right,  Button::Norm,    "button_r");
        b->setTexture(Button::Middle, Button::Norm,    "button_m");
        b->setTexture(Button::Left,   Button::Norm,    "button_l");
        b->setTexture(Button::Font,   Button::Norm,    "button_f");
        b->setTexture(Button::Right,  Button::Focused, "button_rs");
        b->setTexture(Button::Middle, Button::Focused, "button_ms");
        b->setTexture(Button::Left,   Button::Focused, "button_ls");
        b->setTexture(Button::Font,   Button::Focused, "button_fs");
        b->setTexture(Button::Right,  Button::Clicked, "button_rf");
        b->setTexture(Button::Middle, Button::Clicked, "button_mf");
        b->setTexture(Button::Left,   Button::Clicked, "button_lf");
        b->setTexture(Button::Font,   Button::Clicked, "button_ff");
        m_data.enterNamespace("/button");
        b->maxSize(geometry::AABB(m_data.getEntityValue("width"),
                    m_data.getEntityValue("height")));
    }
            
    void Theme::apply(Radio* r)
    {
        r->setTexture(false, "radio_nsel",  "radio_sel");
        r->setTexture(true,  "radio_nselS", "radio_selS");
        m_data.enterNamespace("/radio");
        if(m_rdMs) {
            r->maxSize(geometry::AABB(m_data.getEntityValue("width"),
                        m_data.getEntityValue("height")));
        }
        r->square(m_data.getEntityValue("square") > 0.0f);
    }
            
    void Theme::guiNamespace()
    {
        m_gfx->enterNamespace("/");
        m_gfx->enterNamespace("gui");
    }

}

