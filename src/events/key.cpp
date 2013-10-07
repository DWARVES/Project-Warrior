
#include "key.hpp"

namespace events
{
    /* Taken from SDL2 codesource
     * If there is a way to access to the values stored by the SDL, replace this by it
     */
    static const SDL_Keycode qwertyKeymap[SDL_NUM_SCANCODES] = {
        0, 0, 0, 0,
        'a',
        'b',
        'c',
        'd',
        'e',
        'f',
        'g',
        'h',
        'i',
        'j',
        'k',
        'l',
        'm',
        'n',
        'o',
        'p',
        'q',
        'r',
        's',
        't',
        'u',
        'v',
        'w',
        'x',
        'y',
        'z',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        '0',
        SDLK_RETURN,
        SDLK_ESCAPE,
        SDLK_BACKSPACE,
        SDLK_TAB,
        SDLK_SPACE,
        '-',
        '=',
        '[',
        ']',
        '\\',
        '#',
        ';',
        '\'',
        '`',
        ',',
        '.',
        '/',
        SDLK_CAPSLOCK,
        SDLK_F1,
        SDLK_F2,
        SDLK_F3,
        SDLK_F4,
        SDLK_F5,
        SDLK_F6,
        SDLK_F7,
        SDLK_F8,
        SDLK_F9,
        SDLK_F10,
        SDLK_F11,
        SDLK_F12,
        SDLK_PRINTSCREEN,
        SDLK_SCROLLLOCK,
        SDLK_PAUSE,
        SDLK_INSERT,
        SDLK_HOME,
        SDLK_PAGEUP,
        SDLK_DELETE,
        SDLK_END,
        SDLK_PAGEDOWN,
        SDLK_RIGHT,
        SDLK_LEFT,
        SDLK_DOWN,
        SDLK_UP,
        SDLK_NUMLOCKCLEAR,
        SDLK_KP_DIVIDE,
        SDLK_KP_MULTIPLY,
        SDLK_KP_MINUS,
        SDLK_KP_PLUS,
        SDLK_KP_ENTER,
        SDLK_KP_1,
        SDLK_KP_2,
        SDLK_KP_3,
        SDLK_KP_4,
        SDLK_KP_5,
        SDLK_KP_6,
        SDLK_KP_7,
        SDLK_KP_8,
        SDLK_KP_9,
        SDLK_KP_0,
        SDLK_KP_PERIOD,
        0,
        SDLK_APPLICATION,
        SDLK_POWER,
        SDLK_KP_EQUALS,
        SDLK_F13,
        SDLK_F14,
        SDLK_F15,
        SDLK_F16,
        SDLK_F17,
        SDLK_F18,
        SDLK_F19,
        SDLK_F20,
        SDLK_F21,
        SDLK_F22,
        SDLK_F23,
        SDLK_F24,
        SDLK_EXECUTE,
        SDLK_HELP,
        SDLK_MENU,
        SDLK_SELECT,
        SDLK_STOP,
        SDLK_AGAIN,
        SDLK_UNDO,
        SDLK_CUT,
        SDLK_COPY,
        SDLK_PASTE,
        SDLK_FIND,
        SDLK_MUTE,
        SDLK_VOLUMEUP,
        SDLK_VOLUMEDOWN,
        0, 0, 0,
        SDLK_KP_COMMA,
        SDLK_KP_EQUALSAS400,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        SDLK_ALTERASE,
        SDLK_SYSREQ,
        SDLK_CANCEL,
        SDLK_CLEAR,
        SDLK_PRIOR,
        SDLK_RETURN2,
        SDLK_SEPARATOR,
        SDLK_OUT,
        SDLK_OPER,
        SDLK_CLEARAGAIN,
        SDLK_CRSEL,
        SDLK_EXSEL,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        SDLK_KP_00,
        SDLK_KP_000,
        SDLK_THOUSANDSSEPARATOR,
        SDLK_DECIMALSEPARATOR,
        SDLK_CURRENCYUNIT,
        SDLK_CURRENCYSUBUNIT,
        SDLK_KP_LEFTPAREN,
        SDLK_KP_RIGHTPAREN,
        SDLK_KP_LEFTBRACE,
        SDLK_KP_RIGHTBRACE,
        SDLK_KP_TAB,
        SDLK_KP_BACKSPACE,
        SDLK_KP_A,
        SDLK_KP_B,
        SDLK_KP_C,
        SDLK_KP_D,
        SDLK_KP_E,
        SDLK_KP_F,
        SDLK_KP_XOR,
        SDLK_KP_POWER,
        SDLK_KP_PERCENT,
        SDLK_KP_LESS,
        SDLK_KP_GREATER,
        SDLK_KP_AMPERSAND,
        SDLK_KP_DBLAMPERSAND,
        SDLK_KP_VERTICALBAR,
        SDLK_KP_DBLVERTICALBAR,
        SDLK_KP_COLON,
        SDLK_KP_HASH,
        SDLK_KP_SPACE,
        SDLK_KP_AT,
        SDLK_KP_EXCLAM,
        SDLK_KP_MEMSTORE,
        SDLK_KP_MEMRECALL,
        SDLK_KP_MEMCLEAR,
        SDLK_KP_MEMADD,
        SDLK_KP_MEMSUBTRACT,
        SDLK_KP_MEMMULTIPLY,
        SDLK_KP_MEMDIVIDE,
        SDLK_KP_PLUSMINUS,
        SDLK_KP_CLEAR,
        SDLK_KP_CLEARENTRY,
        SDLK_KP_BINARY,
        SDLK_KP_OCTAL,
        SDLK_KP_DECIMAL,
        SDLK_KP_HEXADECIMAL,
        0, 0,
        SDLK_LCTRL,
        SDLK_LSHIFT,
        SDLK_LALT,
        SDLK_LGUI,
        SDLK_RCTRL,
        SDLK_RSHIFT,
        SDLK_RALT,
        SDLK_RGUI,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        SDLK_MODE,
        SDLK_AUDIONEXT,
        SDLK_AUDIOPREV,
        SDLK_AUDIOSTOP,
        SDLK_AUDIOPLAY,
        SDLK_AUDIOMUTE,
        SDLK_MEDIASELECT,
        SDLK_WWW,
        SDLK_MAIL,
        SDLK_CALCULATOR,
        SDLK_COMPUTER,
        SDLK_AC_SEARCH,
        SDLK_AC_HOME,
        SDLK_AC_BACK,
        SDLK_AC_FORWARD,
        SDLK_AC_STOP,
        SDLK_AC_REFRESH,
        SDLK_AC_BOOKMARKS,
        SDLK_BRIGHTNESSDOWN,
        SDLK_BRIGHTNESSUP,
        SDLK_DISPLAYSWITCH,
        SDLK_KBDILLUMTOGGLE,
        SDLK_KBDILLUMDOWN,
        SDLK_KBDILLUMUP,
        SDLK_EJECT,
        SDLK_SLEEP,
    };

    /* Key methods definition */
    Key::Key()
        : m_key(SDLK_UNKNOWN)
    {}

    Key::Key(char c)
        : Key()
    {
        setChar(c);
    }
            
    Key::Key(KeyType k)
        : Key()
    {
        setSym(k);
    }
            
    Key::Key(KeyMap m)
        : Key()
    {
        setSym(m);
    }

    char Key::getChar() const
    {
        if(m_key > SDLK_TAB && m_key < SDLK_z) /* Ascii printable caracters */
            return static_cast<char>(m_key); /* No futher convertion needed */
        else
            return '\0';
    }

    std::string Key::name() const
    {
        return SDL_GetKeyName(m_key);
    }

    KeyType Key::getSym() const
    {
        return m_key;
    }
            
    Key::operator KeyType() const
    {
        return m_key;
    }

    KeyType Key::getPos() const
    {
        SDL_Scancode scan = SDL_GetScancodeFromKey(m_key);
        if(scan == SDL_SCANCODE_UNKNOWN)
            return SDLK_UNKNOWN;
        else
            return qwertyKeymap[scan];
    }

    std::string Key::posName() const
    {
        KeyType k = getPos();
        return SDL_GetKeyName(k);
    }

    void Key::setChar(char c)
    {
        if(c >= 'A' && c <= 'Z') /* Majuscules must be mapped to minus */
            m_key = static_cast<SDL_Keycode>(c - 'A' + 'a');
        else if(c > SDLK_TAB && c < SDLK_z) /* Ascii printable caracters */
            m_key = static_cast<SDL_Keycode>(c);
        else
            m_key = SDLK_UNKNOWN;
    }

    void Key::setSym(KeyType sym)
    {
        m_key = sym;
    }
            
    void Key::setSym(KeyMap map)
    {
        m_key = (KeyType)map;
    }

    void Key::setPos(KeyType pos)
    {
        SDL_Scancode scan = SDL_SCANCODE_UNKNOWN;
        for(unsigned int scanIdx = SDL_SCANCODE_UNKNOWN;
                scanIdx < SDL_NUM_SCANCODES;
                ++scanIdx) {
            if(qwertyKeymap[scanIdx] == pos) {
                scan = static_cast<SDL_Scancode>(scanIdx);
                break;
            }
        }

        m_key = SDL_GetKeyFromScancode(scan);
    }

}


