
#include "joyhatsave.hpp"
#include "events.hpp"
#include <sstream>
#include <boost/regex.hpp>

namespace events
{
    JoyHatSave::JoyHatSave()
        : JoyEventSave(), m_id(0), m_st(JoyHatState::Center), m_joy(NULL), m_joyID(-1), m_valid(false)
    {}

    JoyHatSave::~JoyHatSave()
    {}

    void JoyHatSave::set(Joystick* joy, int mid, JoyHatState st)
    {
        m_joy = joy;
        if(m_joy)
            m_joyID = m_joy->id();
        else
            m_joyID = -1;
        m_id = mid;
        m_st = st;
        m_valid = false;
    }

    void JoyHatSave::setJoystick(Joystick* joy)
    {
        m_joy = joy;
        if(m_joy)
            m_joyID = m_joy->id();
        else
            m_joyID = -1;
        m_valid = false;
    }

    void JoyHatSave::clear()
    {
        m_id = 0;
        m_st = JoyHatState::Center;
        m_joy = NULL;
        m_joyID = -1;
        m_valid = false;
    }

    int JoyHatSave::joyID() const
    {
        return m_joyID;
    }

    bool JoyHatSave::valid(const events::Events& ev)
    {
        if(!m_joy)
            return false;

        std::vector<int> lmv = ev.lastHatsMoved(m_joy);
        auto it = std::find(lmv.begin(), lmv.end(), m_id);
        if(it == lmv.end())
            return false;

        if(m_joy->hat(*it) != m_st)
            return false;
        m_valid = true;
        return true;
    }

    bool JoyHatSave::valid() const
    {
        return m_valid;
    }

    bool JoyHatSave::still(const events::Events& ev)
    {
        if(!m_joy || !m_valid)
            return false;

        std::vector<int> lmv = ev.lastHatsMoved(m_joy);
        auto it = std::find(lmv.begin(), lmv.end(), m_id);
        if(it == lmv.end())
            return true;

        if(m_joy->hat(*it) == m_st)
            return true;
        m_valid = false;
        return false;
    }

    std::string JoyHatSave::save() const
    {
        std::ostringstream oss;
        oss << "(hat) " << m_id << "->";
        switch(m_st) {
            case JoyHatState::Center: oss << "center"; break;
            case JoyHatState::Left:   oss << "left";   break;
            case JoyHatState::Right:  oss << "right";  break;
            case JoyHatState::Up:     oss << "up";     break;
            case JoyHatState::Down:   oss << "down";   break;
            case JoyHatState::RightUp:
            case JoyHatState::LeftUp:
                                      oss << "up";
                                      break;
            case JoyHatState::RightDown:
            case JoyHatState::LeftDown:
                                      oss << "down";
                                      break;
            default:                  oss << "center"; break;
        }
        oss << " [" << m_joyID << "]";
        return oss.str();
    }

    bool JoyHatSave::load(const std::string& sv)
    {
        boost::regex ld ("^\\s*\\(hat\\)\\s*(\\d+)->(center|left|right|up|down)\\s*(\\[(\\d+)\\])?");
        boost::smatch results;
        if(!boost::regex_match(sv, results, ld))
            return false;

        std::istringstream iss(results[1]);
        iss.seekg(0, std::ios::beg);
        iss >> m_id;

        if(results[2] == "center")
            m_st = JoyHatState::Center;
        else if(results[2] == "left")
            m_st = JoyHatState::Left;
        else if(results[2] == "right")
            m_st = JoyHatState::Right;
        else if(results[2] == "up")
            m_st = JoyHatState::Up;
        else if(results[2] == "down")
            m_st = JoyHatState::Down;

        if(results.size() > 3) {
            iss.str(results[3]);
            iss.seekg(0, std::ios::beg);
            iss >> m_joyID;
        }
        else
            m_joyID = -1;
        m_joy = NULL;
        return true;
    }

    std::string JoyHatSave::id() const
    {
        return "hat";
    }

    EvSave* JoyHatSave::copy() const
    {
        JoyHatSave* jhs = new JoyHatSave;
        jhs->set(m_joy, m_id, m_st);
        return jhs;
    }

}


