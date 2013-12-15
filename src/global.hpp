
#ifndef DEF_GLOBAL
#define DEF_GLOBAL

#include "core/config.hpp"
#include "graphics/graphics.hpp"

namespace global
{
    /** @brief The configuration of the game. */
    extern core::Config* cfg;
    /** @brief The graphic instance used by the game. */
    extern graphics::Graphics* gfx;
}

#endif

