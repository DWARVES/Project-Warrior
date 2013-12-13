
#ifndef DEF_EVENTS_WINDOWSTATE
#define DEF_EVENTS_WINDOWSTATE

#include <SDL.h>

namespace events
{
    /** @brief This are differents states states a window can have. */
    enum class WindowState : Uint8 {
        Visible,    /**< @brief Shown or hidden. */
        Minimized,  /**< @brief Minimized or restored. */
        MouseFocus, /**< @brief Is the mouse on the window. */
        InputFocus, /**< @brief Are the events sended to the window. */
        Maximized,  /**< @brief Has the window been maximized. This states can only be earned : events::Events::hasState and events::Events::lostState will always return false. */
        Moved,      /**< @brief The window has been moved, can only be earned. */
        Resized,    /**< @brief The window has been resized, can only be earned. */
        Exposed,    /**< @brief The window needs to be redrawn, can only be earned. */
        Last,       /**< @brief Only for internal use, number of states. */
    };
}

#endif

