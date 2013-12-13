
#ifndef DEF_EVENTS_BUTTONS
#define DEF_EVENTS_BUTTONS

#include <SDL_mouse.h>

namespace events
{
    /** @brief The different buttons of a mouse. */
    enum class Button : Uint8 {
        Left = SDL_BUTTON_LEFT,     /**< @brief The left button. */
        Right = SDL_BUTTON_RIGHT,   /**< @brief The right button. */
        Middle = SDL_BUTTON_MIDDLE, /**< @brief The middle button. */
        X1 = SDL_BUTTON_X1,         /**< @brief An extra button. */
        X2 = SDL_BUTTON_X2,         /**< @brief Another extra button. */
        Last                        /**< @brief The number of buttons, only for internal use. */
    };
}

#endif

