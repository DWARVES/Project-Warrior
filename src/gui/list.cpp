
#include "list.hpp"
#include <algorithm>

namespace gui
{
    List::List(graphics::Graphics* gfx)
        : Widget(gfx), m_selected(0), m_rolling(false), m_upb(0), m_downb(0)
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
        return m_items[m_selected].it->text();
    }

    List::ItemID List::selectedID() const
    {
        return m_items[m_selected].it;
    }
            
    void List::setItemSize(const geometry::AABB& r)
    {
        m_itemSize = r;
        updateState();
    }

    void List::draw()
    {
        /* TODO */
    }

    void List::focus(bool f)
    {
        /* TODO */
    }

    void List::click(const geometry::Point& p)
    {
        /* TODO */
    }

    void List::keyPress(events::Key k)
    {
        /* TODO */
    }

    bool List::next()
    {
        if(m_selected == m_items.size() - 1)
            return true;
        ++m_selected;
        updateState();
        return false;
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
        size_t nb = (unsigned int)(height() / (m_itemSize.height + 1));
        m_sep = (float)nb / (float)(m_items.size() + 1);

        for(StoredItem st : m_items) {
            st.it->width(m_itemSize.width);
            st.it->height(m_itemSize.height);
            st.it->select(false);
        }
        m_items[m_selected].it->select(true);

        if(nb < m_items.size()) {
            m_rolling = false;
            m_upb = 0;
            m_downb = m_items.size() - 1;
        }
        else {
            m_rolling = true;
            m_upb = std::max(m_selected - nb/2, (size_t)0);
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

}



