
#include "joybuttonsave.hpp"
#include "events.hpp"
#include <sstream>
#include <boost/regex.hpp>

namespace events
{
    JoyButtonSave::JoyButtonSave()
        : JoyEventSave(), m_id(0), m_joy(NULL), m_joyID(-1), m_valid(false)
    {}

    JoyButtonSave::~JoyButtonSave()
    {}

    void JoyButtonSave::set(Joystick* joy, int mid)
    {
        m_id = mid;
        m_joy = joy;
        m_valid = false;

        if(m_joy)
            m_joyID = m_joy->id();
        else
            m_joyID = -1;
    }

    void JoyButtonSave::setJoystick(Joystick* joy)
    {
        m_joy = joy;
        if(m_joy)
            m_joyID = m_joy->id();
        else
            m_joyID = -1;
    }

    void JoyButtonSave::clear()
    {
        m_id = 0;
        m_joy = NULL;
        m_joyID = -1;
        m_valid = false;
    }

    int JoyButtonSave::joyID() const
    {
        return m_joyID;
    }

    bool JoyButtonSave::valid(const events::Events& ev)
    {
        if(!m_joy)
            return false;

        std::vector<int> lmv = ev.lastJoyButtonsPressed(m_joy);
        auto it = std::find(lmv.begin(), lmv.end(), m_id);
        if(it == lmv.end())
            return false;
        
        m_valid = true;
        return true;
    }

    bool JoyButtonSave::valid() const
    {
        return m_valid;
    }

    bool JoyButtonSave::still(const events::Events& ev)
    {
        if(!m_joy || !m_valid)
            return false;

        std::vector<int> lmv = ev.lastJoyButtonsReleased(m_joy);
        auto it = std::find(lmv.begin(), lmv.end(), m_id);
        if(it == lmv.end())
            return true;
        
        m_valid = false;
        return false;
    }

    std::string JoyButtonSave::save() const
    {
        std::ostringstream oss;
        oss << "(joybutton) " << m_id << " [" << m_joy->id() << "]";
        return oss.str();
    }

    bool JoyButtonSave::load(const std::string& sv)
    {
        boost::regex ld ("^\\s*\\(joybutton\\)\\s*(\\d+)\\s*(\\[(\\d+)\\])?");
        boost::smatch results;
        if(!boost::regex_match(sv, results, ld))
            return false;

        std::istringstream iss(results[2]);
        iss >> m_id;
        if(results.size() > 3) {
            iss.str(results[3]);
            iss >> m_joyID;
        }
        else
            m_joyID = -1;
        m_joy = NULL;
        return true;
    }

    std::string JoyButtonSave::id() const
    {
        return "joybutton";
    }

    EvSave* JoyButtonSave::copy() const
    {
        JoyButtonSave* cp = new JoyButtonSave;
        cp->set(m_joy, m_id);
        return cp;
    }

}


