
#ifndef DEF_GLOBAL
#define DEF_GLOBAL

#include "core/config.hpp"
#include "graphics/graphics.hpp"
#include "audio/audio.hpp"
#include "events/events.hpp"
#include "gui/gui.hpp"
#include "gui/theme.hpp"

/** @brief Contains all the global variables. */
namespace global
{
    /** @brief The configuration of the game. */
    extern core::Config* cfg;
    /** @brief The graphic instance used by the game. */
    extern graphics::Graphics* gfx;
    /** @brief The events. */
    extern events::Events* evs;
    /** @brief The sound. */
    extern audio::Audio* audio;
    /** @brief The gui. */
    extern gui::Gui* gui;
    /** @brief The gui theme. */
    extern gui::Theme* theme;

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
    void loadConfig(int argc, char *argv[]);
    /** @brief Load the SDL and init the graphics. */
    void initGraphics();
    /** @brief Load the gui and the gui theme. */
    void loadGui();
    /** @brief Init the audio. */
    void loadAudio();
    /** @brief Prepare the event handler. */
    void loadEvents();
    /** @brief Free the global variables. */
    void freeEverything();
}

#endif

