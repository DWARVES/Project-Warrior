
#include "events.hpp"

namespace events
{
    Events::Events()
        : m_mrel(0.0f,0.0f), m_wheel(0.0f,0.0f), m_quit(false)
    {
        initKeys();
        initButtons();
        initStates();
    }

    void Events::initKeys()
    {
        KeyEvent ev;
        ev.state    = false;
        ev.pressT   = 0;
        ev.releaseT = 0;
        ev.pressP   = geometry::Point(0.0f, 0.0f);
        ev.releaseP = geometry::Point(0.0f, 0.0f);

        for(unsigned int k = SDL_SCANCODE_UNKNOWN; k < SDL_NUM_SCANCODES; ++k)
            m_keys[k]   = ev;
    }

    void Events::initButtons()
    {
        ButtonEvent ev;
        ev.state    = false;
        ev.pressT   = 0;
        ev.releaseT = 0;
        ev.pressP   = geometry::Point(0.0f, 0.0f);
        ev.releaseP = geometry::Point(0.0f, 0.0f);

        for(Uint8 b = 0; b < Uint8(Button::Last); ++b)
            m_buttons[b] = ev;
    }

    void Events::initStates()
    {
        WindowEvent ev;
        ev.state  = false;
        ev.earned = false;
        ev.lost   = false;

        for(Uint8 s = 0;  s < Uint8(WindowState::Last); ++s)
            m_wins[s] = ev;
    }

    Events::~Events()
    {}

    void Events::update()
    {
        /* TODO */
    }

    /************************
     *       Keyboard       *
     ************************/
    std::vector<Key> Events::lastKeysPressed() const
    {
        return m_lastPressedK;
    }

    std::vector<Key> Events::lastKeysReleased() const
    {
        return m_lastReleasedK;
    }

    unsigned int Events::lastKeyPress(KeyType k) const
    {
        return m_keys[SDL_GetScancodeFromKey(k)].pressT;
    }

    unsigned int Events::lastKeyRelease(KeyType k) const
    {
        return m_keys[SDL_GetScancodeFromKey(k)].releaseT;
    }

    unsigned int Events::timeKeyPressed(KeyType k) const
    {
        if(!isKeyPressed(k))
            return 0;
        else
            return SDL_GetTicks() - m_keys[SDL_GetScancodeFromKey(k)].pressT;
    }

    geometry::Point Events::posKeyPress(KeyType k) const
    {
        return m_keys[SDL_GetScancodeFromKey(k)].pressP;
    }

    geometry::Point Events::posKeyRelease(KeyType k) const
    {
        return m_keys[SDL_GetScancodeFromKey(k)].releaseP;
    }

    bool Events::isKeyPressed(KeyType k) const
    {
        return m_keys[SDL_GetScancodeFromKey(k)].state;
    }

    /************************
     *      Modifiers       *
     ************************/
    bool Events::alt() const
    {
        return (SDL_GetModState() & KMOD_ALT) == KMOD_ALT;
    }

    bool Events::shift() const
    {
        return (SDL_GetModState() & KMOD_SHIFT) == KMOD_SHIFT;
    }

    bool Events::ctrl() const
    {
        return (SDL_GetModState() & KMOD_CTRL) == KMOD_CTRL;
    }

    bool Events::super() const
    {
        return (SDL_GetModState() & KMOD_GUI) == KMOD_GUI;
    }

    bool Events::num() const
    {
        return (SDL_GetModState() & KMOD_NUM) == KMOD_NUM;
    }

    bool Events::caps() const
    {
        return (SDL_GetModState() & KMOD_CAPS) == KMOD_CAPS;
    }

    /************************
     *       Buttons        *
     ************************/
    std::vector<Button> Events::lastButtonsPressed() const
    {
        return m_lastPressedB;
    }

    std::vector<Button> Events::lastButtonsReleased() const
    {
        return m_lastReleasedB;
    }

    unsigned int Events::lastButtonPress(Button k) const
    {
        return m_buttons[(Uint8)k].pressT;
    }

    unsigned int Events::lastButtonRelease(Button k) const
    {
        return m_buttons[(Uint8)k].releaseT;
    }

    unsigned int Events::timeButtonPressed(Button k) const
    {
        if(!isButtonPressed(k))
            return 0;
        else
            return SDL_GetTicks() - m_buttons[(Uint8)k].pressT;
    }

    geometry::Point Events::posButtonPress(Button k) const
    {
        return m_buttons[(Uint8)k].pressP;
    }

    geometry::Point Events::posButtonRelease(Button k) const
    {
        return m_buttons[(Uint8)k].releaseP;
    }

    bool Events::isButtonPressed(Button k) const
    {
        return m_buttons[(Uint8)k].state;
    }

    /************************
     *       Pointer        *
     ************************/
    geometry::Point Events::mousePos() const
    {
        int x = 0, 
            y = 0;
        SDL_GetMouseState(&x, &y);
        return geometry::Point((float)x, (float)y);
    }

    geometry::Point Events::mouseRel() const
    {
        return m_mrel;
    }

    /************************
     *       Window         *
     ************************/
    bool Events::hasState(WindowState st) const
    {
        return m_wins[(Uint8)st].state;
    }

    bool Events::earnedState(WindowState st) const
    {
        return m_wins[(Uint8)st].earned;
    }

    bool Events::lostState(WindowState st) const
    {
        return m_wins[(Uint8)st].lost;
    }

    bool Events::closed() const
    {
        return m_closed;
    }

    void Events::closeAbort()
    {
        m_closed = false;
    }

    /************************
     *    Miscellaneous     *
     ************************/
    geometry::Point Events::wheel() const
    {
        return m_wheel;
    }

    std::vector<std::string> Events::dropped() const
    {
        return m_dropped;
    }

    bool Events::quit() const
    {
        return m_quit;
    }

    void Events::quitAbort()
    {
        m_quit = false;
    }
}


