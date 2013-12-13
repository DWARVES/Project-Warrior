
#ifndef DEF_EVENTS_KEYMAP
#define DEF_EVENTS_KEYMAP

#include "key.hpp"
#include "keytype.hpp"
#include <SDL.h>

namespace events
{
    /** @brief This enum defines code keys usually used.
     *
     * It just help the use of the events::Key class.
     */
    enum class KeyMap : KeyType {
        Escape = SDLK_ESCAPE,       /**< @brief The escape key. */
        Enter = SDLK_RETURN,        /**< @brief The return key, same as Return. */
        Return = SDLK_RETURN,       /**< @brief The return key, same as Enter. */
        Backspace = SDLK_BACKSPACE, /**< @brief The backspace key (<-). */
        Left = SDLK_LEFT,           /**< @brief The left directional key. */
        Up = SDLK_UP,               /**< @brief The up directional key. */
        Right = SDLK_RIGHT,         /**< @brief The right directional key. */
        Down = SDLK_DOWN,           /**< @brief The down directional key. */
        Tab = SDLK_TAB,             /**< @brief The tab key. */
        KP1 = SDLK_KP_1,            /**< @brief The keypad key 1. */
        KP2 = SDLK_KP_2,            /**< @brief The keypad key 2. */
        KP3 = SDLK_KP_3,            /**< @brief The keypad key 3. */
        KP4 = SDLK_KP_4,            /**< @brief The keypad key 4. */
        KP5 = SDLK_KP_5,            /**< @brief The keypad key 5. */
        KP6 = SDLK_KP_6,            /**< @brief The keypad key 6. */
        KP7 = SDLK_KP_7,            /**< @brief The keypad key 7. */
        KP8 = SDLK_KP_8,            /**< @brief The keypad key 8. */
        KP9 = SDLK_KP_9,            /**< @brief The keypad key 9. */
        F1 = SDLK_F1,               /**< @brief The F1 key. */
        F2 = SDLK_F2,               /**< @brief The F2 key. */
        F3 = SDLK_F3,               /**< @brief The F3 key. */
        F4 = SDLK_F4,               /**< @brief The F4 key. */
        F5 = SDLK_F5,               /**< @brief The F5 key. */
        F6 = SDLK_F6,               /**< @brief The F6 key. */
        F7 = SDLK_F7,               /**< @brief The F7 key. */
        F8 = SDLK_F8,               /**< @brief The F8 key. */
        F9 = SDLK_F9,               /**< @brief The F9 key. */
        F10 = SDLK_F10,             /**< @brief The F10 key. */
        F11 = SDLK_F11,             /**< @brief The F11 key. */
        F12 = SDLK_F12,             /**< @brief The F12 key. */
        Begin = SDLK_HOME,          /**< @brief The "begin" key, usually represented by a diagonal arrow. */
        End = SDLK_END,             /**< @brief The end key. */
        PageUp = SDLK_PAGEUP,       /**< @brief The page up key. */
        PageDown = SDLK_PAGEDOWN,   /**< @brief The page down key. */
    };
}

#endif

