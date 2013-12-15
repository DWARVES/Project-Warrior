
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
}

#endif

