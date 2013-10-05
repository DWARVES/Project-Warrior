
#ifndef DEF_EVENTS_KEYMAP
#define DEF_EVENTS_KEYMAP

#include "key.hpp"
#include <SDL.h>

namespace events
{
    /* This enum defines keys usually used
     * It just help the use of the Key class
     */
    enum class KeyMap : KeyType {
        /* Letters are not defined because they are easy to use with a char */
        Escape = SDLK_ESCAPE,
        /* Defined twice as Enter and Return, because both names can be used */
        Enter = SDLK_RETURN,
        Return = SDLK_RETURN,
        Backspace = SDLK_BACKSPACE,
        KP1 = SDLK_KP_1,
        KP2 = SDLK_KP_2,
        KP3 = SDLK_KP_3,
        KP4 = SDLK_KP_4,
        KP5 = SDLK_KP_5,
        KP6 = SDLK_KP_6,
        KP7 = SDLK_KP_7,
        KP8 = SDLK_KP_8,
        KP9 = SDLK_KP_9,
        F1 = SDLK_F1,
        F2 = SDLK_F2,
        F3 = SDLK_F3,
        F4 = SDLK_F4,
        F5 = SDLK_F5,
        F6 = SDLK_F6,
        F7 = SDLK_F7,
        F8 = SDLK_F8,
        F9 = SDLK_F9,
        F10 = SDLK_F10,
        F11 = SDLK_F11,
        F12 = SDLK_F12,
    };
}

#endif

