
#ifndef DEF_EVENTS_JOYBUTTONSAVE
#define DEF_EVENTS_JOYBUTTONSAVE

#include "events/joyeventsave.hpp"
#include "events/joystick.hpp"

namespace events
{
    /** @brief Represents a joystick button event. */
    class JoyButtonSave : public JoyEventSave
    {
        public:
            JoyButtonSave();
            virtual ~JoyButtonSave();

            /** @brief Set the event.
             * @param joy The joystick to use. 
             * @param id  The id of the button.
             */
            void set(Joystick* joy, int mid);
            /** @brief Change the joystick witheout changing the configuration. */
            virtual void setJoystick(Joystick* joy);
            /** @brief Clears the saved event. */
            void clear();
            /** @brief Returns the joystick id loaded. */
            int joyID() const;

            /* EvSave memebers */
            virtual bool valid(const events::Events& ev);
            virtual bool valid() const;
            virtual bool still(const events::Events& ev);
            virtual std::string save() const;
            /** @brief Loads the JoyButtonSave from a string. Can't load the joystick, so only load its id.
             * The user will have to set the loaded joystick with setJoystick.
             */
            virtual bool load(const std::string& sv);
            virtual std::string id() const;
            virtual EvSave* copy() const;

        private:
            int m_id;        /**< @brief The id of the button. */
            Joystick* m_joy; /**< @brief The joystick used. */
            int m_joyID;     /**< @brief The id of the joystick loaded. */
            bool m_valid;    /**< @brief Is the event validated. */
    };
}

#endif

