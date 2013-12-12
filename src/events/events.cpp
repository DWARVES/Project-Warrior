
#include "events.hpp"
#include "keysave.hpp"
#include "joybuttonsave.hpp"
#include "joyhatsave.hpp"
#include "core/logger.hpp"
#include <sstream>
#include <algorithm>

namespace events
{
    Events::Events()
        : m_input(false), m_mrel(0.0f,0.0f), m_closed(false), m_wheel(0.0f,0.0f), m_quit(false)
    {
        initKeys();
        initButtons();
        initStates();
        initEvSaves();
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
            m_keys[k] = ev;
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
            
    void Events::initEvSaves()
    {
        KeySave* ks = new KeySave;
        EvSave::regist(ks);
        JoyButtonSave* jb = new JoyButtonSave;
        EvSave::regist(jb);
        JoyHatSave* jh = new JoyHatSave;
        EvSave::regist(jh);
        m_maxSaved = 0;
    }

    Events::~Events()
    {
        closeJoysticks();
        EvSave::freeAll();
        removeSaveds();
    }

    void Events::update()
    {
        /* Initialising */
        m_lastPressedK.clear();
        m_lastReleasedK.clear();
        m_lastInput.clear();
        m_lastPressedB.clear();
        m_lastReleasedB.clear();
        m_mrel = geometry::Point(0.0f, 0.0f);
        for(Uint8 s = 0;  s < Uint8(WindowState::Last); ++s) {
            m_wins[s].earned = false;
            m_wins[s].lost = false;
        }
        clearJoysticks();
        m_lastSavedValidated.clear();
        m_lastSavedReleased.clear();
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
                case SDL_TEXTINPUT:
                    process(&ev.text);
                    break;
                case SDL_JOYAXISMOTION:
                    process(&ev.jaxis);
                    break;
                case SDL_JOYHATMOTION:
                    process(&ev.jhat);
                    break;
                case SDL_JOYBUTTONDOWN:
                case SDL_JOYBUTTONUP:
                    process(&ev.jbutton);
                    break;
                case SDL_JOYDEVICEADDED:
                case SDL_JOYDEVICEREMOVED:
                    process(&ev.jdevice);
                    break;
                default:
                    break;
            }
        }
        processSavedEvents();
    }

    void Events::process(SDL_KeyboardEvent* ev)
    {
        /* Repeat events mustn't be processed */
        if(ev->repeat)
            return;

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

    void Events::process(SDL_TextInputEvent* ev)
    {
        m_lastInput += ev->text;
        m_fullInput += ev->text;
    }

    void Events::clearJoysticks()
    {
        for(auto& j : m_joys) {
            j.second.lpressed.clear();
            j.second.lreleased.clear();
            j.second.laxis.clear();
            j.second.lhats.clear();
        }
        m_lastJoyAdded.clear();
        m_lastJoyRemoved.clear();
    }

    void Events::process(SDL_JoyAxisEvent* ev)
    {
        Joystick* j = getJoyFromID(ev->which);
        if(j == NULL) /* Means joystick non loaded */
            return;
        m_joys[j].laxis.push_back(ev->axis);
    }

    void Events::process(SDL_JoyHatEvent* ev)
    {
        Joystick* j = getJoyFromID(ev->which);
        if(j == NULL) /* Means joystick non loaded */
            return;
        m_joys[j].lhats.push_back(ev->hat);
    }

    void Events::process(SDL_JoyButtonEvent* ev)
    {
        Joystick* j = getJoyFromID(ev->which);
        if(j == NULL) /* Means joystick non loaded */
            return;

        if(ev->type == SDL_JOYBUTTONDOWN) {
            m_joys[j].buttons[ev->button].pressT = SDL_GetTicks();
            m_joys[j].buttons[ev->button].pressP = mousePos();
            m_joys[j].lpressed.push_back(ev->button);
        }
        else {
            m_joys[j].buttons[ev->button].releaseT = SDL_GetTicks();
            m_joys[j].buttons[ev->button].releaseP = mousePos();
            m_joys[j].lreleased.push_back(ev->button);
        }
    }
            
    void Events::process(SDL_JoyDeviceEvent* ev)
    {
        if(ev->type == SDL_JOYDEVICEADDED)
            m_lastJoyAdded.push_back(ev->which);
        else { /* Joystick removed */
            Joystick* j = getJoyFromID(ev->which);
            if(j == NULL) /* Shouldn't happen */
                return;
            closeJoystick(j);
            m_lastJoyRemoved.push_back(j);
        }
    }
            
    void Events::processSavedEvents()
    {
        for(auto& p : m_saved) {
            EvSave* ev = p.second.ev;
            if(!ev->valid()) {
                if(ev->valid(*this)) {
                    p.second.lvalid = SDL_GetTicks();
                    m_lastSavedValidated.push_back(p.first);
                }
            }
            else {
                if(!ev->still(*this)) {
                    p.second.lrelease = SDL_GetTicks();
                    m_lastSavedReleased.push_back(p.first);
                }
            }
        }
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

    bool Events::keyJustPressed(Key k) const
    {
        std::vector<Key> lp = lastKeysPressed();
        if(std::find(lp.begin(), lp.end(), k) != lp.end())
            return true;
        else
            return false;
    }

    bool Events::keyJustReleased(Key k) const
    {
        std::vector<Key> lr = lastKeysReleased();
        if(std::find(lr.begin(), lr.end(), k) != lr.end())
            return true;
        else
            return false;
    }

    unsigned int Events::lastKeyPress(Key k) const
    {
        return m_keys[SDL_GetScancodeFromKey(k)].pressT;
    }

    unsigned int Events::lastKeyRelease(Key k) const
    {
        return m_keys[SDL_GetScancodeFromKey(k)].releaseT;
    }

    unsigned int Events::timeKeyPressed(Key k) const
    {
        if(!isKeyPressed(k))
            return 0;
        else
            return SDL_GetTicks() - m_keys[SDL_GetScancodeFromKey(k)].pressT;
    }

    geometry::Point Events::posKeyPress(Key k) const
    {
        return m_keys[SDL_GetScancodeFromKey(k)].pressP;
    }

    geometry::Point Events::posKeyRelease(Key k) const
    {
        return m_keys[SDL_GetScancodeFromKey(k)].releaseP;
    }

    bool Events::isKeyPressed(Key k) const
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
     *     Text input       *
     ************************/
    std::string Events::lastInput() const
    {
        return m_lastInput;
    }

    std::string Events::fullInput() const
    {
        return m_fullInput;
    }

    void Events::clearInput()
    {
        m_fullInput.clear();
    }

    void Events::enableInput(bool e)
    {
        if(m_input == e) /* Nothing change */
            return;
        if(e)
            SDL_StartTextInput();
        else
            SDL_StopTextInput();
        m_input = e;
    }

    bool Events::isInputEnabled() const
    {
        return m_input;
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

    bool Events::buttonJustPressed(Button k) const
    {
        std::vector<Button> lp = lastButtonsPressed();
        if(std::find(lp.begin(), lp.end(), k) != lp.end())
            return true;
        else
            return false;
    }

    bool Events::buttonJustReleased(Button k) const
    {
        std::vector<Button> lr = lastButtonsReleased();
        if(std::find(lr.begin(), lr.end(), k) != lr.end())
            return true;
        else
            return false;
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
     *      Joystick        *
     ************************/
    int Events::numJoysticks() const
    {
        return SDL_NumJoysticks();
    }

    bool Events::openJoysticks()
    {
        bool ret = true;
        for(int i = 0; i < numJoysticks(); ++i) {
            if(std::find(m_joyLoaded.begin(), m_joyLoaded.end(), i) != m_joyLoaded.end())
                continue;
            if(openJoystick(i) == NULL)
                ret = false;
        }
        return ret;
    }

    void Events::closeJoysticks()
    {
        m_joyLoaded.clear();
        for(auto& it : m_joys)
            delete it.first;
        m_joys.clear();
    }

    Joystick* Events::openJoystick(JoystickID id)
    {
        /* Was it already loaded ? */
        bool found = false;
        for(size_t i = 0; i < m_joyLoaded.size(); ++i) {
            if(m_joyLoaded[i] == id) {
                found = true;
                break;
            }
        }
        if(found) {
            std::ostringstream oss;
            oss << "Tryed to open joystick #" << id << ", which was already loaded.";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return NULL;
        }

        /* Loading the joystick */
        Joystick* j = NULL;
        try {
            j = new Joystick(id);
        }
        catch(const JoystickException& e) {
            /* Logging already done in Joystick() */
            return NULL;
        }

        /* Adding the joystick to the structure */
        JoystickEvent ev = initEvent(j);
        m_joys[j] = ev;
        return j;
    }

    Events::JoystickEvent Events::initEvent(Joystick* j)
    {
        JoystickEvent ev;
        ev.buttons.resize(j->numButtons());
        for(size_t i = 0; i < ev.buttons.size(); ++i) {
            ev.buttons[i].pressT   = 0;
            ev.buttons[i].releaseT = 0;
            ev.buttons[i].pressP   = geometry::Point(0.0f,0.0f);
            ev.buttons[i].releaseP = geometry::Point(0.0f,0.0f);
        }

        return ev;
    }

    void Events::closeJoystick(Joystick* j)
    {
        if(!isJoystickLoaded(j)) {
            core::logger::logm("Tryed to close an unopened joystick.", core::logger::WARNING);
            return;
        }

        auto it = std::find(m_joyLoaded.begin(), m_joyLoaded.end(), j->id());
        if(it == m_joyLoaded.end())
            return;
        m_joyLoaded.erase(it);
        m_joys.erase(j);
        delete j;
    }

    bool Events::isJoystickLoaded(Joystick* j) const
    {
        return m_joys.find(j) != m_joys.end();
    }

    std::vector<int> Events::lastJoyButtonsPressed(Joystick* j) const
    {
        std::vector<int> defRet;
        if(!isJoystickLoaded(j)) {
            core::logger::logm("Tryed to access an unopened joystick.", core::logger::WARNING);
            return defRet;
        }
        else
            return m_joys.at(j).lpressed;
    }

    std::vector<int> Events::lastJoyButtonsReleased(Joystick* j) const
    {
        std::vector<int> defRet;
        if(!isJoystickLoaded(j)) {
            core::logger::logm("Tryed to access an unopened joystick.", core::logger::WARNING);
            return defRet;
        }
        else
            return m_joys.at(j).lreleased;
    }

    unsigned int Events::lastJoyButtonPress(Joystick* j, int b) const
    {
        if(!isJoystickLoaded(j)) {
            core::logger::logm("Tryed to access an unopened joystick.", core::logger::WARNING);
            return 0;
        }
        else if(b >= j->numButtons()) {
            std::ostringstream oss;
            oss << "Tryed to access to unexisting button '" << b << "' in joystick #" << j->id() << ".";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return 0;
        }
        else
            return m_joys.at(j).buttons[b].pressT;
    }

    unsigned int Events::lastJoyButtonRelease(Joystick* j, int b) const
    {
        if(!isJoystickLoaded(j)) {
            core::logger::logm("Tryed to access an unopened joystick.", core::logger::WARNING);
            return 0;
        }
        else if(b >= j->numButtons()) {
            std::ostringstream oss;
            oss << "Tryed to access to unexisting button '" << b << "' in joystick #" << j->id() << ".";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return 0;
        }
        else
            return m_joys.at(j).buttons[b].releaseT;
    }

    bool Events::joyButtonJustPressed(Joystick* j, int b) const
    {
        /* No need to check j, lp will be empty if j is not a valid Joystick */
        std::vector<int> lp = lastJoyButtonsPressed(j);
        if(std::find(lp.begin(), lp.end(), b) != lp.end())
            return true;
        else
            return false;
    }

    bool Events::joyButtonJustReleased(Joystick* j, int b) const
    {
        /* No need to check j, lp will be empty if j is not a valid Joystick */
        std::vector<int> lr = lastJoyButtonsReleased(j);
        if(std::find(lr.begin(), lr.end(), b) != lr.end())
            return true;
        else
            return false;
    }

    unsigned int Events::timeJoyButtonPressed(Joystick* j, int b) const
    {
        if(!isJoystickLoaded(j)) {
            core::logger::logm("Tryed to access an unopened joystick.", core::logger::WARNING);
            return 0;
        }
        else if(b >= j->numButtons()) {
            std::ostringstream oss;
            oss << "Tryed to access to unexisting button '" << b << "' in joystick #" << j->id() << ".";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return 0;
        }
        else if(!j->button(b)) /* If the button is not pressed */
            return 0;
        else
            return SDL_GetTicks() - m_joys.at(j).buttons[b].pressT;
    }

    geometry::Point Events::posJoyButtonPress(Joystick* j, int b) const
    {
        if(!isJoystickLoaded(j)) {
            core::logger::logm("Tryed to access an unopened joystick.", core::logger::WARNING);
            return geometry::Point(0.0f,0.0f);
        }
        else if(b >= j->numButtons()) {
            std::ostringstream oss;
            oss << "Tryed to access to unexisting button '" << b << "' in joystick #" << j->id() << ".";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return geometry::Point(0.0f,0.0f);
        }
        else
            return m_joys.at(j).buttons[b].pressP;
    }

    geometry::Point Events::posJoyButtonRelease(Joystick* j, int b) const
    {
        if(!isJoystickLoaded(j)) {
            core::logger::logm("Tryed to access an unopened joystick.", core::logger::WARNING);
            return geometry::Point(0.0f,0.0f);
        }
        else if(b >= j->numButtons()) {
            std::ostringstream oss;
            oss << "Tryed to access to unexisting button '" << b << "' in joystick #" << j->id() << ".";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return geometry::Point(0.0f,0.0f);
        }
        else
            return m_joys.at(j).buttons[b].releaseP;
    }

    std::vector<int> Events::lastAxesMoved(Joystick* j) const
    {
        std::vector<int> defRet;
        if(!isJoystickLoaded(j)) {
            core::logger::logm("Tryed to access an unopened joystick.", core::logger::WARNING);
            return defRet;
        }
        else
            return m_joys.at(j).laxis;
    }

    std::vector<int> Events::lastHatsMoved(Joystick* j) const
    {
        std::vector<int> defRet;
        if(!isJoystickLoaded(j)) {
            core::logger::logm("Tryed to access an unopened joystick.", core::logger::WARNING);
            return defRet;
        }
        else
            return m_joys.at(j).lhats;
    }

    bool Events::joysticksChanged() const
    {
        return !m_lastJoyAdded.empty() || !m_lastJoyRemoved.empty();
    }

    std::vector<JoystickID> Events::lastJoysticksAdded() const
    {
        return m_lastJoyAdded;
    }
    
    std::vector<Joystick*> Events::lastJoysticksRemoved() const
    {
        return m_lastJoyRemoved;
    }

    std::vector<std::pair<int,Joystick*>> Events::lastJoyButtonsPressed() const
    {
        std::vector<std::pair<int,Joystick*>> ret;
        for(auto it : m_joys) {
            ret.reserve(it.second.lpressed.size());
            for(auto but : it.second.lpressed)
                ret.push_back(std::pair<int,Joystick*>(but, it.first));
        }
        return ret;
    }

    std::vector<std::pair<int,Joystick*>> Events::lastJoyButtonsReleased() const
    {
        std::vector<std::pair<int,Joystick*>> ret;
        for(auto it : m_joys) {
            ret.reserve(it.second.lreleased.size());
            for(auto but : it.second.lreleased)
                ret.push_back(std::pair<int,Joystick*>(but, it.first));
        }
        return ret;
    }

    std::vector<std::pair<int,Joystick*>> Events::lastAxesMoved() const
    {
        std::vector<std::pair<int,Joystick*>> ret;
        for(auto it : m_joys) {
            ret.reserve(it.second.laxis.size());
            for(auto but : it.second.laxis)
                ret.push_back(std::pair<int,Joystick*>(but, it.first));
        }
        return ret;
    }

    std::vector<std::pair<int,Joystick*>> Events::lastHatsMoved() const
    {
        std::vector<std::pair<int,Joystick*>> ret;
        for(auto it : m_joys) {
            ret.reserve(it.second.lhats.size());
            for(auto but : it.second.lhats)
                ret.push_back(std::pair<int,Joystick*>(but, it.first));
        }
        return ret;
    }
            
    Joystick* Events::getJoyFromID(JoystickID id)
    {
        for(auto& it : m_joys) {
            if(it.first->id() == id)
                return it.first;
        }
        return NULL;
    }

    /************************
     *     Saved events     *
     ************************/
    size_t Events::addSaved(EvSave* sv, bool tosave)
    {
        SavedEvent ev;
        ev.ev = sv;
        ev.lvalid = 0;
        ev.lrelease = 0;
        ev.tosave = tosave;

        m_saved[m_maxSaved] = ev;
        ++m_maxSaved;
        return (m_maxSaved-1);
    }

    EvSave* Events::getSaved(size_t id) const
    {
        auto it = m_saved.find(id);
        if(it == m_saved.end())
            return NULL;
        else
            return it->second.ev;
    }

    void Events::removeSaved(size_t id)
    {
        if(m_saved.find(id) == m_saved.end())
            return;
        if(m_saved[id].tosave)
            delete m_saved[id].ev;
        m_saved.erase(id);
    }

    void Events::removeSaveds()
    {
        for(auto p : m_saved) {
            if(p.second.tosave)
                delete p.second.ev;
        }
        m_saved.clear();
    }

    bool Events::isSavedValid(size_t id) const
    {
        return m_saved.at(id).ev->valid();
    }

    bool Events::isSavedJustValid(size_t id) const
    {
        return std::find(m_lastSavedValidated.begin(), m_lastSavedValidated.end(), id) != m_lastSavedValidated.end();
    }

    std::vector<size_t> Events::lastSavedValidated() const
    {
        return m_lastSavedValidated;
    }

    std::vector<size_t> Events::lastSavedReleased() const
    {
        return m_lastSavedReleased;
    }

    unsigned int Events::lastSavedValid(size_t id) const
    {
        return m_saved.at(id).lvalid;
    }

    unsigned int Events::lastSavedRelease(size_t id) const
    {
        return m_saved.at(id).lrelease;
    }

    unsigned int Events::timeSavedValidated(size_t id) const
    {
        if(!isSavedJustValid(id))
            return 0;
        else
            return SDL_GetTicks() - m_saved.at(id).lvalid;
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


