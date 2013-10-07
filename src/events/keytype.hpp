
#ifndef DEF_EVENTS_KEYTYPE
#define DEF_EVENTS_KEYTYPE

#include <SDL.h>

namespace events
{
    /* KeyType must be an integer, the user does not need to know it's real size
     * SDL_Keycode is already a typedef used by SDL to Sint32
     */
    typedef SDL_Keycode KeyType;
}

#endif

