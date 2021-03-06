
#include "controler.hpp"
#include "global.hpp"
#include "core/logger.hpp"
#include "events/joyeventsave.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

namespace gameplay
{
    /** @brief The duration of the turning in ms. */
    const Uint32 turnTime = 300;
    /** @brief Time after a move in which a smash can be done in ms. */
    const Uint32 smashTime = 50;
    /** @brief The names of all the controls. */
    const char* const controlsNames[(unsigned int)Controler::Last] = {
        "left",
        "right",
        "up",
        "down",
        "runleft",
        "runright",
        "attack",
        "special",
        "dodge",
        "catch",
        "jump",
    };

    core::FakeFS<std::string> Controler::m_evs;
    std::string Controler::m_path;

    Controler::Controler(const std::string& id)
        : m_loaded(false), m_ch(NULL), m_actual(Last)
    {
        m_namespace = "/" + id;
        for(unsigned int i = 0; i < (unsigned int)Last; ++i)
            m_ctrls[i] = NULL;

        /* Checking the existence of controls config. */
        m_evs.enterNamespace(m_namespace);
        for(unsigned int i = 0; i < (unsigned int)Last; ++i) {
            if(!m_evs.existsEntity(controlsNames[i])) {
                std::ostringstream oss;
                oss << "Entity \"" << controlsNames[i] << "\" is missing in controler \"" << id << "\" configuration.";
                core::logger::logm(oss.str(), core::logger::ERROR);
                return;
            }
        }

        /* Loading the joystick if necessary. */
        events::Joystick* j = NULL;
        if(id != "Keyboard") {
            j = global::evs->joyFromName(id);
            if(!j) {
                std::ostringstream oss;
                oss << "Joystick \"" << id << "\" not plugged : can't be used.";
                core::logger::logm(oss.str(), core::logger::ERROR);
                return;
            }
        }

        /* Loading the controls config. */
        for(unsigned int i = 0; i < (unsigned int)Last; ++i) {
            m_ctrls[i] = events::EvSave::parse(m_evs.getEntityValue(controlsNames[i]));
            if(!m_ctrls[i]) {
                std::ostringstream oss;
                oss << "Couldn't load configuration for event \"" << controlsNames[i] << "\" in controler \"" << id << "\".";
                core::logger::logm(oss.str(), core::logger::ERROR);
                return;
            }

            if(j)
                dynamic_cast<events::JoyEventSave*>(m_ctrls[i])->setJoystick(j);
        }
        m_loaded = true;
    }

    Controler::~Controler()
    {
        if(!m_loaded)
            return;
        for(unsigned int i = 0; i < (unsigned int)Last; ++i) {
            if(m_ctrls[i])
                delete m_ctrls[i];
        }
    }

    bool Controler::isOpen() const
    {
        return m_loaded;
    }

    void Controler::attach(Character* ch)
    {
        m_ch = ch;
    }

    Character* Controler::attached() const
    {
        return m_ch;
    }

    void Controler::update()
    {
        if(!m_ch || !m_loaded)
            return;

        for(unsigned int i = (unsigned int)Attack; i < (unsigned int)Last; ++i)
            m_ctrls[i]->still(*global::evs);

        Character::Control ctrl = Character::Walk;
        Character::Direction dir = Character::Fixed;
        if(m_ctrls[(unsigned int)Dodge]->valid(*global::evs))
            ctrl = Character::Dodge;
        else if(m_ctrls[(unsigned int)Attack]->valid(*global::evs))
            ctrl = Character::Attack;
        else if(m_ctrls[(unsigned int)Special]->valid(*global::evs))
            ctrl = Character::Spell;
        else if(m_ctrls[(unsigned int)Catch]->valid(*global::evs))
            ctrl = Character::Catch;
        else if(m_ctrls[(unsigned int)Jump]->valid(*global::evs)) {
            ctrl = Character::Run;
            dir  = Character::Up;
        }
        else if(m_ctrls[(unsigned int)RunLeft]->still(*global::evs)) {
            ctrl = Character::Run;
            dir  = Character::Left;
        }
        else if(m_ctrls[(unsigned int)RunRight]->still(*global::evs)) {
            ctrl = Character::Run;
            dir  = Character::Right;
        }

        if(dir == Character::Fixed) {
            for(unsigned int i = (unsigned int)Left; i <= (unsigned int)Down; ++i) {
                if(m_ctrls[i]->still(*global::evs)) {
                    dir = (Character::Direction)i;
                    break;
                }
            }
        }

        if(dir != m_prevdir)
            m_turned = SDL_GetTicks();
        if(dir == Character::Right && m_prevdir == Character::Left)
            dir = Character::TurnRight;
        else if(dir == Character::Left && m_prevdir == Character::Right)
            dir = Character::TurnLeft;
        else if(m_prevdir == Character::TurnLeft || m_prevdir == Character::TurnRight) {
            if(SDL_GetTicks() - m_turned > turnTime)
                dir = m_prevdir;
        }
        if(dir != Character::Fixed)
            m_prevdir = dir;

        if(ctrl == Character::Attack && dir != Character::Fixed && SDL_GetTicks() - m_turned < smashTime)
            ctrl = Character::Smash;

        m_ch->action(ctrl, dir);
    }

    bool Controler::load(const std::string& path)
    {
        m_path.clear();

        /* Opening the file. */
        std::ifstream ifs(path);
        if(!ifs) {
            std::ostringstream oss;
            oss << "Couldn't open " << path << " file when loading the controlers.";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return false;
        }

        /* Parsing the file. */
        if(!m_evs.load(ifs ,[] (const std::string& str) { return str; })) {
            std::ostringstream oss;
            oss << "Couldn't parse " << path << " file when loading the controlers.";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return false;
        }

        m_path = path;
        return true;
    }

    bool Controler::close()
    {
        if(m_path.empty())
            return false;

        /* Opening the file. */
        std::ofstream ofs(m_path);
        if(!ofs) {
            std::ostringstream oss;
            oss << "Couldn't open " << m_path << " file when saving the controlers.";
            core::logger::logm(oss.str(), core::logger::ERROR);
            return false;
        }

        /* Saving to the file. */
        if(!m_evs.save(ofs, [] (const std::string& str) { return str; })) {
            std::ostringstream oss;
            oss << "Couldn't write to " << m_path << " file when saving the controlers.";
            core::logger::logm(oss.str(), core::logger::ERROR);
            return false;
        }

        /* Clearing the loaded controlers. */
        m_evs.clear();
        m_path.clear();
        return true;
    }

    std::vector<std::string> Controler::getIDs()
    {
        std::vector<std::string> ret;
        std::vector<std::string> set = listAll();
        std::vector<std::string> plug = global::evs->joyNames();

        for(auto it = set.begin(); it != set.end(); ++it) {
            if(*it == "Keyboard") {
                ret.push_back("Keyboard");
                continue;
            }

            auto result = std::find(plug.begin(), plug.end(), *it);
            if(result == plug.end())
                continue;
            ret.push_back(*it);

            plug.erase(result);
            if(plug.empty())
                return ret;
        }
        return ret;
    }

    std::vector<std::string> Controler::listAll()
    {
        m_evs.enterNamespace("/");
        return m_evs.listNamespaces();
    }

    void Controler::set(Controls ctrl, events::EvSave* nev)
    {
        if(!m_loaded || ctrl == Last)
            return;

        /* Setting the new control. */
        if(m_ctrls[(unsigned int)ctrl])
            delete m_ctrls[(unsigned int)ctrl];
        m_ctrls[(unsigned int)ctrl] = nev;

        /* Saving the change. */
        m_evs.enterNamespace(m_namespace);
        m_evs.setEntityValue(controlsNames[(unsigned int)ctrl], nev->save());
    }

    std::string Controler::get(Controls ctrl) const
    {
        if(!m_loaded || ctrl == Last)
            return "";
        return m_ctrls[(unsigned int)ctrl]->save();
    }
            
    std::string Controler::controlName(Controls ctrl)
    {
        if(ctrl != Last)
            return controlsNames[(unsigned int)ctrl];
        else
            return "Invalid";
    }
            
    bool Controler::create(const std::string& name)
    {
        m_evs.enterNamespace("/");
        if(m_evs.existsNamespace(name))
            return false;

        /* Create the controler. */
        if(!m_evs.createNamespace(name))
            return false;
        m_evs.enterNamespace(name);

        /* Set default values. */
        m_evs.createEntity("up",       "(axis) 1->-16384");
        m_evs.createEntity("down",     "(axis) 1->16384");
        m_evs.createEntity("left",     "(axis) 0->-16384");
        m_evs.createEntity("right",    "(axis) 0->16384");
        m_evs.createEntity("runleft",  "(axis) 0->-32768");
        m_evs.createEntity("runright", "(axis) 0->32768");
        m_evs.createEntity("attack",   "(joybutton) 0");
        m_evs.createEntity("special",  "(joybutton) 1");
        m_evs.createEntity("jump",     "(joybutton) 2");
        m_evs.createEntity("catch",    "(joybutton) 3");
        m_evs.createEntity("dodge",    "(joybutton) 4");

        m_evs.enterNamespace("/");
        return true;
    }

}


