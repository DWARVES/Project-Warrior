
#ifndef DEF_EVENT_JOYEVENTSAVE
#define DEF_EVENT_JOYEVENTSAVE

#include "evsave.hpp"
#include "joystick.hpp"

namespace events
{
    /** @brief Virtual pure interface for all joystick saves. */
    class JoyEventSave : public EvSave
    {
        public:
            JoyEventSave();
            virtual ~JoyEventSave();

            /** @brief Set the joystick to use for the event saved. */
            virtual void setJoystick(Joystick*) = 0;
    };
}

#endif

