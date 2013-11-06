
#include "list.hpp"
#include <algorithm>

namespace gui
{
    List::List(graphics::Graphics* gfx)
        : Widget(gfx), m_selected(0), m_lselected(0), m_rolling(false), m_upb(0), m_downb(0)
    {}

    List::~List()
    {
        while(!m_items.empty()) {
            deleteItem(0);
        }
    }

    float List::width(float w)
    {
        float r = Widget::width(w);
        updateState();
        return r;
    }

    float List::height(float h)
    {
        float r = Widget::height(h);
        updateState();
        return r;
    }

    float List::width() const
    {
        return Widget::width();
    }

    float List::height() const
    {
        return Widget::height();
    }

    List::ItemID List::addItem(size_t pos, const std::string& text, float offx)
    {
        auto it = m_items.begin();
        for(size_t i = 0; i < pos; ++i, ++it);

        StoredItem st;
        st.it = new internal::Item(m_gfx);
        st.it->text(text);
        st.offx = offx;
        m_items.insert(it, st);
        updateState();
        flushTexts();
        return st.it;
    }

    void List::setItem(List::ItemID id, const std::string& text)
    {
        m_items[posFromID(id)].it->text(text);
    }

    void List::removeItem(List::ItemID id)
    {
        deleteItem( posFromID(id) );
        updateState();
    }

    List::ItemID List::item(size_t pos)
    {
        return m_items[pos].it;
    }

    size_t List::selected() const
    {
        return m_selected;
    }

    std::string List::selectedText() const
    {
        if(m_selected < m_items.size())
            return m_items[m_selected].it->text();
        else
            return "";
    }

    List::ItemID List::selectedID() const
    {
        if(m_selected < m_items.size())
            return m_items[m_selected].it;
        else
            return 0;
    }
            
    void List::setItemSize(const geometry::AABB& r)
    {
        m_itemSize = r;
        updateState();
    }
            
    void List::setPart(Part p, bool sel, const std::string& path)
    {
        m_texts[(sel ? 1 : 0)][(unsigned short)p] = path;
        flushTexts();
    }

    void List::draw()
    {
        m_gfx->push();
        m_gfx->move(width() / 2.0f, 0.0f);

        for(size_t i = m_upb; i < m_downb; ++i) {
            m_gfx->move(0.0f, m_sep);
            m_gfx->push();
            m_gfx->move(m_items[i].offx, 0.0f);
            m_items[i].it->draw();
            m_gfx->pop();
        }
        m_gfx->pop();
    }

    void List::focus(bool f)
    {
        if(f)
            m_selected = 0;
        else
            m_selected = m_items.size();
        updateState();
    }

    bool List::action(Widget::Action a)
    {
        switch(a) {
            case Widget::ScrollUp:
                return prev();
            case Widget::ScrollDown:
                return next();
            case Widget::ScrollLeft:
                if(m_selected < m_items.size())
                    return m_items[m_selected].it->scrollLeft();
                else
                    return false;
            case Widget::ScrollRight:
                if(m_selected < m_items.size())
                    return m_items[m_selected].it->scrollRight();
                else
                    return false;
            case Widget::First:
                if(m_selected == 0)
                    return false;
                m_selected = 0;
                updateState();
                return true;
            case Widget::Last:
                if(m_selected == m_items.size() - 1)
                    return false;
                m_selected = m_items.size() - 1;
                updateState();
                return true;
            case Widget::Select:
            case Widget::Remove:
            default:
                return false;
        }
    }

    bool List::next()
    {
        if(m_selected == m_items.size() - 1)
            return false;
        else if(m_selected == m_items.size())
            m_selected = 0;
        else
            ++m_selected;
        updateState();
        return true;
    }
            
    bool List::prev()
    {
        if(m_selected == 0)
            return false;
        --m_selected;
        updateState();
        return true;
    }

    size_t List::posFromID(List::ItemID id)
    {
        size_t pos = 0;
        while(m_items[pos].it != id && m_items.size()) ++pos;
        return pos;
    }

    void List::updateState()
    {
        if(m_items.empty())
            return;
        size_t nb = (unsigned int)(height() / (m_itemSize.height * 1.2f));
        m_sep = height() / (float)(std::min(nb,m_items.size()) + 1);

        for(StoredItem st : m_items) {
            st.it->width(m_itemSize.width);
            st.it->height(m_itemSize.height);
            st.it->select(false);
        }

        if(m_selected != m_items.size())
            m_items[m_selected].it->select(true);

        if(m_selected != m_lselected) {
            select();
            m_lselected = m_selected;
        }

        if(m_items.size() <= nb) {
            m_rolling = false;
            m_upb = 0;
            m_downb = m_items.size();
        }
        else {
            m_rolling = true;
            if(m_selected == m_items.size())
                m_upb = 0;
            else if(nb/2 >= m_selected)
                m_upb = 0;
            else if(m_selected + nb/2 >= m_items.size())
                m_upb = m_items.size() - nb;
            else
                m_upb = m_selected - nb/2;
            m_downb = m_upb + nb;
        }
    }

    void List::deleteItem(size_t pos)
    {
        if(item(pos) == 0)
            return;
        delete m_items[pos].it;
        auto it = m_items.begin();
        while(it->it != m_items[pos].it) ++it;
        m_items.erase(it);
    }

    void List::flushTexts()
    {
        for(size_t i = 0; i < m_items.size(); ++i) {
            for(size_t p = 0; p < (unsigned short)Last; ++p) {
                m_items[i].it->setPart((internal::Item::Part)p, true,  m_texts[1][p]);
                m_items[i].it->setPart((internal::Item::Part)p, false, m_texts[0][p]);
            }
        }
    }

    void List::select()
    {
        /* Nothing to do */
    }
}



