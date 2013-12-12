
#ifndef DEF_EVENTS_JOYAXISSAVE
#define DEF_EVENTS_JOYAXISSAVE

#include "events/evsave.hpp"
#include "events/joystick.hpp"

namespace events
{
    /** @brief Represent a joystick axis event. */
    class JoyAxisSave : public EvSave
    {
        public:
            JoyAxisSave();
            virtual ~JoyAxisSave();

            /** @brief Set the event.
             * @param joy The joystick to use. 
             * @param id  The id of the axis.
             * @param value The value the axis must reach (can be positive or negative).
             */
            void set(Joystick* joy, int mid, int value);
            /** @brief Change the joystick witheout changing the configuration. */
            void setJoystick(Joystick* joy);
            /** @brief Clears the saved event. */
            void clear();
            /** @brief Returns the joystick id loaded. */
            int joyID() const;

            /* EvSave memebers */
            virtual bool valid(const events::Events& ev);
            virtual bool valid() const;
            virtual bool still(const events::Events& ev);
            virtual std::string save() const;
            /** @brief Loads the JoyAxisSave from a string. Can't load the joystick, so only load its id.
             * The user will have to set the loaded joystick with setJoystick.
             */
            virtual bool load(const std::string& sv);
            virtual std::string id() const;
            virtual EvSave* copy() const;

        private:
            int m_id;        /**< @brief The id of the axis. */
            int m_value;     /**< @brief The value the axis must reach. */
            Joystick* m_joy; /**< @brief The joystick used. */
            int m_joyID;     /**< @brief The id of the joystick loaded. */
            bool m_valid;    /**< @brief Is the event validated. */
    };
}

#endif

