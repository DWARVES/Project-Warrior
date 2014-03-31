
#ifndef DEF_EVENTS_EVSAVE
#define DEF_EVENTS_EVSAVE

#include <map>
#include <string>

namespace events
{
    class Events;

    /** @brief An abstract class that represent a saved event. */
    class EvSave
    {
        public:
            EvSave();
            virtual ~EvSave();

            /** @brief Returns true the event saved was done. */
            virtual bool valid(const events::Events& ev) = 0;
            /** @brief Indicates if the event is being holded. */
            virtual bool valid() const = 0;
            /** @brief Check if the event is still holded.
             * If the EvSave hasn't been validated before, this has the same effect as valid(ev).
             */
            virtual bool still(const events::Events& ev) = 0;
            /** @brief Save the event to a string. */
            virtual std::string save() const = 0;
            /** @brief Load the event from a string. */
            virtual bool load(const std::string& sv) = 0;
            /** @brief Returns an ID corresponding of the type of event saved. */
            virtual std::string id() const = 0;
            /** @brief Allocates a new saved event with the same params and return it. */
            virtual EvSave* copy() const = 0;

            /* Static members */
            /** @brief Register a new type of event. */
            static void regist(EvSave* sv);
            /** @brief Load a saved event. Returns NULL is the type of the event is not registered or if an error happened. */
            static EvSave* parse(const std::string& sv);
            /** @brief Free all the saved events registered. */
            static void freeAll();

        private:
            /** @brief The registered types <id,type>. */
            static std::map<std::string,EvSave*> m_registered;
    };
}

#endif

