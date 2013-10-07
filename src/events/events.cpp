
#include "events.hpp"
#include "core/logger.hpp"
#include <sstream>

namespace events
{
    Events::Events()
        : m_mrel(0.0f,0.0f), m_closed(false), m_wheel(0.0f,0.0f), m_quit(false)
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
        /* Initialising */
        m_lastPressedK.clear();
        m_lastReleasedK.clear();
        m_lastPressedB.clear();
        m_lastReleasedB.clear();
        m_mrel = geometry::Point(0.0f, 0.0f);
        for(Uint8 s = 0;  s < Uint8(WindowState::Last); ++s) {
            m_wins[s].earned = false;
            m_wins[s].lost = false;
        }
        m_wheel = geometry::Point(0.0f, 0.0f);
        m_dropped.clear();

        /* Processing */
        SDL_Event ev;
        while(SDL_PollEvent(&ev)) {
            switch(ev.type) {
                case SDL_QUIT:
                    m_quit = true;
                    break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    process(&ev.key);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    process(&ev.button);
                    break;
                case SDL_MOUSEWHEEL:
                    process(&ev.wheel);
                    break;
                case SDL_MOUSEMOTION:
                    process(&ev.motion);
                    break;
                case SDL_WINDOWEVENT:
                    process(&ev.window);
                    break;
                case SDL_DROPFILE:
                    process(&ev.drop);
                    break;
                default:
                    break;
            }
        }
    }

    void Events::process(SDL_KeyboardEvent* ev)
    {
        SDL_Scancode id = ev->keysym.scancode;
        if(ev->type == SDL_KEYDOWN) {
            m_lastPressedK.push_back(ev->keysym.sym);
            m_keys[id].pressT = SDL_GetTicks();
            m_keys[id].pressP = mousePos();
            m_keys[id].state  = true;
        }
        else {
            m_lastReleasedK.push_back(ev->keysym.sym);
            m_keys[id].releaseT = SDL_GetTicks();
            m_keys[id].releaseP = mousePos();
            m_keys[id].state    = false;
        }
    }

    void Events::process(SDL_MouseMotionEvent* ev)
    {
        m_mrel.x += (float)ev->x;
        m_mrel.y += (float)ev->y;
    }

    void Events::process(SDL_MouseWheelEvent* ev)
    {
        m_wheel.x += (float)ev->x;
        m_wheel.y += (float)ev->y;
    }

    void Events::process(SDL_MouseButtonEvent* ev)
    {
        if(ev->button >= (Uint8)Button::Last)
            return;
        if(ev->type == SDL_MOUSEBUTTONDOWN) {
            m_lastPressedB.push_back((Button)ev->button);
            m_buttons[ev->button].state  = true;
            m_buttons[ev->button].pressT = SDL_GetTicks();
            m_buttons[ev->button].pressP = mousePos();
        }
        else {
            m_lastReleasedB.push_back((Button)ev->button);
            m_buttons[ev->button].state    = true;
            m_buttons[ev->button].releaseT = SDL_GetTicks();
            m_buttons[ev->button].releaseP = mousePos();
        }
    }

    void Events::process(SDL_DropEvent* ev)
    {
        std::string path(ev->file);
        SDL_free(ev->file);
        m_dropped.push_back(path);
    }

    void Events::process(SDL_WindowEvent* ev)
    {
        WindowState id;
        bool nvalue;
        bool change = true; /* Must thhe state be changed ? */

        switch(ev->event) {
            case SDL_WINDOWEVENT_SHOWN:
                id = WindowState::Visible;
                nvalue = true;
                break;
            case SDL_WINDOWEVENT_HIDDEN:
                id = WindowState::Visible;
                nvalue = false;
                break;
            case SDL_WINDOWEVENT_EXPOSED:
                id = WindowState::Exposed;
                nvalue = true;
                change = false;
                break;
            case SDL_WINDOWEVENT_MOVED:
                id = WindowState::Moved;
                nvalue = true;
                change = false;
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                id = WindowState::Minimized;
                nvalue = true;
                break;
            case SDL_WINDOWEVENT_RESTORED:
                id = WindowState::Minimized;
                nvalue = false;
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                id = WindowState::Maximized;
                nvalue = true;
                change = false;
                break;
            case SDL_WINDOWEVENT_ENTER:
                id = WindowState::MouseFocus;
                nvalue = true;
                break;
            case SDL_WINDOWEVENT_LEAVE:
                id = WindowState::MouseFocus;
                nvalue = false;
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                id = WindowState::InputFocus;
                nvalue = true;
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                id = WindowState::InputFocus;
                nvalue = false;
                break;
            case SDL_WINDOWEVENT_CLOSE:
                m_closed = true;
                return;
            default:
                break;
        }

        if(!nvalue && m_wins[(Uint8)id].state)
            m_wins[(Uint8)id].lost = true;
        else if(nvalue && !m_wins[(Uint8)id].state)
            m_wins[(Uint8)id].earned = true;
        if(change)
            m_wins[(Uint8)id].state = nvalue;
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

    bool Events::mouseRelMode(bool enable)
    {
        SDL_bool st = (enable ? SDL_TRUE : SDL_FALSE);
        if(SDL_SetRelativeMouseMode(st) < 0) {
            std::ostringstream oss;
            oss << "Couldn't enable relative mouse mode : \"" << SDL_GetError() << "\".";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return false;
        }
        else
            return true;
    }
            
    bool Events::mouseRelMode() const
    {
        return (SDL_GetRelativeMouseMode() == SDL_TRUE) ? true : false;
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


