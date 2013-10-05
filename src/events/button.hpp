
#ifndef DEF_EVENTS_BUTTONS
#define DEF_EVENTS_BUTTONS

#include <SDL_mouse.h>

namespace events
{
    enum class Button : Uint8 {
        Left = SDL_BUTTON_LEFT,
        Right = SDL_BUTTON_RIGHT,
        Middle = SDL_BUTTON_MIDDLE,
        X1 = SDL_BUTTON_X1,
        X2 = SDL_BUTTON_X2,
        Last
    };
}

#endif

