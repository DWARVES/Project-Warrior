
#ifndef DEF_EVENTS_WINDOWSTATE
#define DEF_EVENTS_WINDOWSTATE

#include <SDL.h>

namespace events
{
    enum class WindowState : Uint8 {
        Visible,    /* Shown or hidden */
        Minimized,  /* Minimized or restored */
        Maximized,  /* Maximized or restored */
        MouseFocus,
        InputFocus,
        /* The following states can only be earned, hasState and lostState will always returns false */
        Moved,
        Resized,
        Exposed,    /* The window needs to be redrawn */
        Last,       /* Only for internal use */
    };
}

#endif

