
#ifndef DEF_GLOBAL
#define DEF_GLOBAL

#include "core/config.hpp"
#include "graphics/graphics.hpp"
#include "events/events.hpp"

/** @brief Contains all the global variables. */
namespace global
{
    /** @brief The configuration of the game. */
    extern core::Config* cfg;
    /** @brief The graphic instance used by the game. */
    extern graphics::Graphics* gfx;
    /** @brief The events. */
    extern events::Events* evs;

    /** @brief An exception used to report a fatal error while initializing the game. */
    class init_exception : public std::exception
    {
        public:
            init_exception(const char* msg) noexcept;
            const char* what() const noexcept;

        private:
            const char* m_msg;
    };

    /** @brief Load the locale. */
    void loadLocale();
    /** @brief Parses the command line argument and config file. */
    void loadConfig();
    /** @brief Load the SDL and init the graphics. */
    void initGraphics();
    /** @brief Prepare the event handler. */
    void loadEvents();
    /** @brief Load the controlers, must be called after loadConfig */
    void loadControlers();
    /** @brief Free the global variables. */
    void freeEverything();
}

#endif

