
#include "joyhatsave.hpp"
#include "events.hpp"
#include <sstream>
#include <boost/regex.hpp>

namespace events
{
    JoyHatSave::JoyHatSave()
        : EvSave(), m_id(0), m_st(JoyHatState::Center), m_joy(NULL), m_joyID(-1), m_valid(false)
    {}

    JoyHatSave::~JoyHatSave()
    {}

    void JoyHatSave::set(Joystick* joy, int mid, JoyHatState st)
    {
        m_joy = joy;
        m_joyID = m_joy->id();
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
        oss << "(hat) " << m_id << "->" << (int)m_st << " [" << m_joyID << "]";
        return oss.str();
    }

    bool JoyHatSave::load(const std::string& sv)
    {
        boost::regex ld ("^\\s*\\(hat\\)\\s*(\\d+)->(\\d+)\\s*[(\\d+)]");
        boost::smatch results;
        if(!boost::regex_match(sv, results, ld))
            return false;

        std::istringstream iss(results[2]);
        Uint8 temp;
        iss >> temp;
        m_st = (JoyHatState)temp;
        iss.str(results[1]);
        iss >> m_id;
        iss.str(results[3]);
        iss >> m_joyID;
        m_joy = NULL;
        return false;
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


