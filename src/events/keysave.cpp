
#include "keysave.hpp"
#include "events.hpp"
#include <sstream>
#include <boost/regex.hpp>

namespace events
{
    KeySave::KeySave()
        : EvSave(), m_valid(false)
    {}

    KeySave::KeySave(Key k)
        : EvSave(), m_key(k), m_valid(false)
    {}

    KeySave::~KeySave()
    {}

    void KeySave::setKey(Key k)
    {
        m_key = k;
        m_valid = false;
    }

    Key KeySave::getKey() const
    {
        return m_key;
    }

    bool KeySave::valid(const Events& ev)
    {
        m_valid = ev.isKeyPressed(m_key);
        return m_valid;
    }

    bool KeySave::valid() const
    {
        return m_valid;
    }

    bool KeySave::still(const Events& ev)
    {
        if(!m_valid)
            return false;
        m_valid = ev.isKeyPressed(m_key);
        return m_valid;
    }

    std::string KeySave::save() const
    {
        std::ostringstream oss;
        oss << "(key) " << m_key.getSym();
        return oss.str();
    }

    bool KeySave::load(const std::string& sv)
    {
        boost::regex reg ("^\\s*(key)\\s*(\\d+?)");
        boost::smatch results;
        if(!boost::regex_match(sv, results, reg))
            return false;

        std::istringstream iss(results[1]);
        KeyType t;
        iss >> t;
        setKey(t);
        return true;
    }

    std::string KeySave::id() const
    {
        return "key";
    }

    EvSave* KeySave::copy() const
    {
        KeySave* ret = new KeySave(m_key);
        return ret;
    }

}


