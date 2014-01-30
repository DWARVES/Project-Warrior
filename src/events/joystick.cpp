
#include "joystick.hpp"
#include "core/logger.hpp"
#include <sstream>

namespace events
{
    JoystickException::JoystickException(JoystickID id) throw()
        : m_id(id)
    {}

    JoystickException::~JoystickException() throw()
    {}

    const char* JoystickException::what() const throw()
    {
        return "Joystick couldn't be loaded.";
    }

    JoystickID JoystickException::which() const throw()
    {
        return m_id;
    }

    Joystick::Joystick(JoystickID ID)
        : m_joy(NULL)
    {
        m_joy = SDL_JoystickOpen(ID);
        if(m_joy == NULL) {
            std::ostringstream oss;
            oss << "Couldn't open joystick #" << ID << " : \"" << SDL_GetError() << "\".";
            core::logger::logm(oss.str(), core::logger::WARNING);
            throw JoystickException(ID);
        }
        else {
            std::ostringstream oss;
            oss << "Joystick #" << ID << " \"" << name() << "\" opened.";
            core::logger::logm(oss.str(), core::logger::MSG);
        }
    }

    Joystick::~Joystick()
    {
        if(m_joy != NULL)
            SDL_JoystickClose(NULL);
    }

    bool Joystick::operator==(JoystickID cmp) const
    {
        return id() == cmp;
    }

    bool Joystick::operator!=(JoystickID cmp) const
    {
        return id() != cmp;
    }

    Joystick::operator JoystickID() const
    {
        return id();
    }

    int Joystick::numAxis() const
    {
        return SDL_JoystickNumAxes(m_joy);
    }

    int Joystick::numButtons() const
    {
        return SDL_JoystickNumButtons(m_joy);
    }

    int Joystick::numHats() const
    {
        return SDL_JoystickNumHats(m_joy);
    }

    std::string Joystick::name() const
    {
        const char* n = SDL_JoystickName(m_joy);
        if(n == NULL)
            return std::string("");
        else
            return std::string(n);
    }

    JoystickID Joystick::id() const
    {
        return SDL_JoystickInstanceID(m_joy);
    }

    JoyHatState Joystick::hat(int h) const
    {
        return (JoyHatState)SDL_JoystickGetHat(m_joy, h);
    }

    int Joystick::axis(int a) const
    {
        return SDL_JoystickGetAxis(m_joy, a);
    }

    bool Joystick::button(int b) const
    {
        return SDL_JoystickGetButton(m_joy, b);
    }

}



