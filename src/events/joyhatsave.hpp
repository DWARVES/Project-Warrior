
#ifndef DEF_EVENTS_JOYHATSAVE
#define DEF_EVENTS_JOYHATSAVE

#include "events/evsave.hpp"
#include "events/joystick.hpp"

namespace events
{
    /** @brief Represents a joystick hat event. */
    class JoyHatSave : public EvSave
    {
        public:
            JoyHatSave();
            virtual ~JoyHatSave();

            /** @brief Set the event.
             * @param joy The joystick to use. 
             * @param id  The id of the button.
             * @param st The state of the hat.
             */
            void set(Joystick* joy, int mid, JoyHatState st);
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
            /** @brief Loads the JoyHatSave from a string. Can't load the joystick, so only load its id.
             * The user will have to set the loaded joystick with setJoystick.
             */
            virtual bool load(const std::string& sv);
            virtual std::string id() const;
            virtual EvSave* copy() const;

        private:
            int m_id;         /**< @brief The id of the hat. */
            JoyHatState m_st; /**< @brief The state of the hat necessary to valid the event. */
            Joystick* m_joy;  /**< @brief The joystick used. */
            int m_joyID;      /**< @brief The id of the joystick loaded. */
            bool m_valid;     /**< @brief Is the event validated. */
    };
}

#endif

