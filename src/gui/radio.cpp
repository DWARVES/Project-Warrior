
#include "radio.hpp"
#include <algorithm>

namespace gui
{
    /* statics members */
    std::map<std::string, std::vector<Radio*>> Radio::m_groups;

    /* Methods */
    Radio::Radio(graphics::Graphics* gfx, const std::string& gr)
        : Widget(gfx), m_box(gfx), m_group(gr)
    {
        if(createGroupIfNotExistant()) {
            /* If the first in the group, it's selected by default */
            m_box.set(true);
        }
        m_groups[m_group].push_back(this);
    }

    Radio::~Radio()
    {
        removeFromGroup();
    }

    float Radio::width(float w)
    {
        Widget::width(w);
        return m_box.width(w);
    }

    float Radio::height(float h)
    {
        Widget::height(h);
        return m_box.height(h);
    }

    float Radio::width() const
    {
        return Widget::width();
    }

    float Radio::height() const
    {
        return Widget::height();
    }

    bool Radio::existsGroup(const std::string& name)
    {
        return m_groups.find(name) != m_groups.end();
    }

    std::vector<Radio*> Radio::groupContent(const std::string& name)
    {
        auto it = m_groups.find(name);
        if(it != m_groups.end())
            return it->second;
        else {
            std::vector<Radio*> empty;
            return empty;
        }
    }

    Radio* Radio::getSelected(const std::string& name)
    {
        std::vector<Radio*> rds = groupContent(name);
        if(rds.empty())
            return NULL;

        /* Find the first (only) one selected */
        auto it = std::find_if(rds.begin(), rds.end(), [](Radio* rd) { return rd->get(); });
        if(it != rds.end())
            return *it;
        else /* Shouldn't happen */
            return NULL;
    }

    void Radio::setGroup(const std::string& name)
    {
        removeFromGroup();
        m_group = name;
        if(createGroupIfNotExistant())
            m_box.set(true);
        else
            m_box.set(false);
    }

    std::string Radio::group() const
    {
        return m_group;
    }

    bool Radio::select()
    {
        deselectGroup();
        return m_box.set(true);
    }

    bool Radio::get() const
    {
        return m_box.get();
    }

    void Radio::label(const std::string& lab)
    {
        m_box.label(lab);
    }

    std::string Radio::label() const
    {
        return m_box.label();
    }

    void Radio::setTexture(bool f, const std::string& empty, const std::string& selected)
    {
        m_box.setTexture(f, empty, selected);
    }
            
    void Radio::setFont(bool f, const std::string& font)
    {
        m_box.setFont(f, font);
    }

    void Radio::maxSize(float m)
    {
        m_box.maxSize(m);
    }

    void Radio::disableMaxSize()
    {
        m_box.disableMaxSize();
    }

    void Radio::draw()
    {
        m_box.draw();
    }

    void Radio::focus(bool f)
    {
        m_box.focus(f);
    }

    bool Radio::action(Widget::Action a)
    {
        if(a != Widget::Select)
            return false;

        if(get())
            return false;
        else {
            select();
            return true;
        }
    }

    void Radio::deselectGroup()
    {
        std::vector<Radio*> rds = groupContent(m_group);
        for(Radio* rd : rds)
            rd->m_box.set(false);
    }

    bool Radio::createGroupIfNotExistant()
    {
        if(existsGroup(m_group))
            return false;
        else {
            std::vector<Radio*> empty;
            empty.reserve(10);
            m_groups[m_group] = empty;
            return true;
        }
    }

    void Radio::removeFromGroup()
    {
        auto it = m_groups.find(m_group);
        auto to_erase = std::find(it->second.begin(), it->second.end(), this);
        if(to_erase != it->second.end()) {
            it->second.erase(to_erase);
            if(it->second.empty())
                m_groups.erase(it);
        }
    }

}


