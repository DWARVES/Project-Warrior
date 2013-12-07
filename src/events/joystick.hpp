
#ifndef DEF_EVENTS_JOYSTICK
#define DEF_EVENTS_JOYSTICK

#include <SDL.h>
#include <string>
#include <exception>

namespace events
{
    typedef int JoystickID;

    enum class JoyHatState : Uint8 {
        Center    = SDL_HAT_CENTERED,
        Up        = SDL_HAT_UP,
        Right     = SDL_HAT_RIGHT,
        Down      = SDL_HAT_DOWN,
        Left      = SDL_HAT_LEFT,
        RightUp   = SDL_HAT_RIGHTUP,
        RightDown = SDL_HAT_RIGHTDOWN,
        LeftUp    = SDL_HAT_LEFTUP,
        LeftDown  = SDL_HAT_LEFTDOWN,
        /* UpRight == RightUp, UpLeft == LeftUp ... only present for user conveniance */
        UpRight   = SDL_HAT_RIGHTUP,
        UpLeft    = SDL_HAT_LEFTUP,
        DownRight = SDL_HAT_RIGHTDOWN,
        DownLeft  = SDL_HAT_LEFTDOWN,
    };

    class JoystickException : public std::exception
    {
        public:
            JoystickException() = delete;
            JoystickException(JoystickID id) throw();
            virtual ~JoystickException() throw();
            virtual const char* what() const throw();
            JoystickID which() const throw();

        private:
            JoystickID m_id;
    };

    /** @todo Support ball system. */
    class Joystick
    {
        public:
            /* Launch an JoystickException if ID couldn't be loaded */
            Joystick(JoystickID ID);
            Joystick() = delete;
            Joystick(const Joystick&) = delete;
            ~Joystick();

            /* Comparison */
            bool operator==(JoystickID cmp) const;
            bool operator!=(JoystickID cmp) const;
            operator JoystickID() const;

            /* Querying informations about the device */
            int numAxis() const;
            int numButtons() const;
            int numHats() const;
            std::string name() const;
            JoystickID id() const;

            /* Getting state of the device */
            JoyHatState hat(int h) const;
            /* The value is between -32768 and 32767 */
            int axis(int a) const;
            bool button(int b) const;

        private:
            SDL_Joystick* m_joy;
    };
}

#endif

