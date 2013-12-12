
#ifndef DEF_EVENTS_KEYSAVE
#define DEF_EVENTS_KEYSAVE

#include "events/key.hpp"
#include "events/evsave.hpp"

namespace events
{
    /** @brief Represents a keyboard event. */
    class KeySave : public EvSave
    {
        public:
            KeySave();
            KeySave(Key k);
            virtual ~KeySave();

            /** @brief Defines to which key this event correspond. */
            void setKey(Key k);
            Key getKey() const;

            virtual bool valid(const Events& ev);
            virtual bool valid() const;
            virtual bool still(const Events& ev);
            virtual std::string save() const;
            virtual bool load(const std::string& sv);
            virtual std::string id() const;
            virtual EvSave* copy() const;

        private:
            Key m_key;    /**< @brief The key corresponding to the saved event. */
            bool m_valid; /**< @brief Is the key setted validated. */
    };
}

#endif

