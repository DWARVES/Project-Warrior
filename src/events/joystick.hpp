
#ifndef DEF_EVENTS_JOYSTICK
#define DEF_EVENTS_JOYSTICK

#include <SDL.h>
#include <string>
#include <exception>

namespace events
{
    /** @brief The type of a joystick id. */
    typedef int JoystickID;

    /** @brief The different positions of a joystick hat. */
    enum class JoyHatState : Uint8 {
        Center    = SDL_HAT_CENTERED,  /**< @brief Nothing pressed. */
        Up        = SDL_HAT_UP,        /**< @brief Oriented upward. */
        Right     = SDL_HAT_RIGHT,     /**< @brief Oriented to the right. */
        Down      = SDL_HAT_DOWN,      /**< @brief Oriented downward. */
        Left      = SDL_HAT_LEFT,      /**< @brief Oriented to the left. */
        RightUp   = SDL_HAT_RIGHTUP,   /**< @brief Oriented to right-up diagonal. */
        RightDown = SDL_HAT_RIGHTDOWN, /**< @brief Oriented to right-down diagonal. */
        LeftUp    = SDL_HAT_LEFTUP,    /**< @brief Oriented to left-up diagonal. */
        LeftDown  = SDL_HAT_LEFTDOWN,  /**< @brief Oriented to left-down diagonal. */
        /* UpRight == RightUp, UpLeft == LeftUp ... only present for user conveniance */
        UpRight   = SDL_HAT_RIGHTUP,   /**< @brief Same as RightUp. */
        UpLeft    = SDL_HAT_LEFTUP,    /**< @brief Same as LeftUp. */
        DownRight = SDL_HAT_RIGHTDOWN, /**< @brief Same as RightDown. */
        DownLeft  = SDL_HAT_LEFTDOWN,  /**< @brief Same as Leftdown. */
    };

    /** @brief Exception launched when an error happen in events::Joystick constructor. */
    class JoystickException : public std::exception
    {
        public:
            JoystickException() = delete;
            JoystickException(JoystickID id) throw();
            virtual ~JoystickException() throw();
            virtual const char* what() const throw();
            /** @brief The id of the joystick which causes the error. */
            JoystickID which() const throw();

        private:
            /** @brief The id of the joystick which causes the error. */
            JoystickID m_id;
    };

    /** @brief Represents a joystick.
     * @todo Support ball system.
     */
    class Joystick
    {
        public:
            /** Launch an JoystickException if ID couldn't be loaded. */
            Joystick(JoystickID ID);
            Joystick() = delete;
            Joystick(const Joystick&) = delete;
            ~Joystick();

            /* Comparison */
            bool operator==(JoystickID cmp) const;
            bool operator!=(JoystickID cmp) const;
            /** @brief Get the id of this joystick. */
            operator JoystickID() const;

            /** @brief Returns the number of axis. */
            int numAxis() const;
            /** @brief Returns the number of buttons. */
            int numButtons() const;
            /** @brief Returns the number of hats. */
            int numHats() const;
            /** @brief Returns the name of the joystick. */
            std::string name() const;
            /** @brief Get the id of this joystick. */
            JoystickID id() const;

            /** @brief Indicate the position of the hat h. */
            JoyHatState hat(int h) const;
            /** @brief The position of the a axis between -32768 and 32767. */
            int axis(int a) const;
            /** @brief Is the b button pressed. */
            bool button(int b) const;

        private:
            /** @brief The internal joystick structure. */
            SDL_Joystick* m_joy;
    };
}

#endif

