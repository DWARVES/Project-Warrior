
#include "joyaxissave.hpp"
#include "events.hpp"
#include <sstream>
#include <boost/regex.hpp>

namespace events
{
    JoyAxisSave::JoyAxisSave()
        : EvSave(), m_id(0), m_value(0), m_joy(NULL), m_joyID(-1), m_valid(false)
    {}

    JoyAxisSave::~JoyAxisSave()
    {}

    void JoyAxisSave::set(Joystick* joy, int mid, int value)
    {
        m_joy = joy;
        m_joyID = m_joy->id();
        m_id = mid;
        m_value = value;
        m_valid = false;
    }

    void JoyAxisSave::setJoystick(Joystick* joy)
    {
        m_joy = joy;
        if(m_joy)
            m_joyID = m_joy->id();
        else
            m_joyID = -1;
        m_valid = false;
    }

    void JoyAxisSave::clear()
    {
        m_id = 0;
        m_value = 0;
        m_joy = NULL;
        m_joyID = -1;
        m_valid = false;
    }

    int JoyAxisSave::joyID() const
    {
        return m_joyID;
    }

    bool JoyAxisSave::valid(const events::Events& ev)
    {
        if(!m_joy)
            return false;
        std::vector<int> lmv = ev.lastAxesMoved(m_joy);
        auto it = std::find(lmv.begin(), lmv.end(), m_id);
        if(it == lmv.end())
            return false;

        int value = m_joy->axis(*it);
        if(std::abs(value) < std::abs(m_value) || value*m_value < 0)
            return false;
        m_valid = true;
        return true;
    }

    bool JoyAxisSave::valid() const
    {
        return m_valid;
    }

    bool JoyAxisSave::still(const events::Events& ev)
    {
        if(!m_joy || !m_valid)
            return false;
        std::vector<int> lmv = ev.lastAxesMoved(m_joy);
        auto it = std::find(lmv.begin(), lmv.end(), m_id);
        if(it == lmv.end())
            return true;

        int value = m_joy->axis(*it);
        if(std::abs(value) > std::abs(m_value) && value*m_value > 0)
            return true;
        m_valid = false;
        return false;
    }

    std::string JoyAxisSave::save() const
    {
        std::ostringstream oss;
        oss << "(axis) " << m_id << "->" << m_value << " [" << m_joyID << "]";
        return oss.str();
    }

    bool JoyAxisSave::load(const std::string& sv)
    {
        boost::regex ld ("^\\s*(axis)\\s*(\\d+)->(-?\\d+)\\s*[(\\d+)]");
        boost::smatch results;

        if(!boost::regex_match(sv, results, ld))
            return false;

        clear();
        std::istringstream iss (results[1]);
        iss >> m_id;
        iss.str(results[2]);
        iss >> m_value;
        iss.str(results[3]);
        iss >> m_joyID;
        return true;
    }

    std::string JoyAxisSave::id() const
    {
        return "axis";
    }

    EvSave* JoyAxisSave::copy() const
    {
        JoyAxisSave* jas = new JoyAxisSave;
        jas->set(m_joy, m_id, m_value);
        return jas;
    }

}


