
#include "character.hpp"
#include "global.hpp"
#include "core/pathParser.hpp"
#include "core/logger.hpp"
#include "graphics/graphics.hpp"
#include "lua/graphicsExposure.hpp"
#include "lua/saveExposure.hpp"

#include <sstream>
#include <fstream>
#include <SDL.h>

namespace gameplay
{
    size_t Character::m_count = 0;

    Character::Character(const std::string& path)
        : m_path(path), m_name("broken"), m_desc("Couldn't load."), m_valid(false)
    {
        ++m_count;
        std::ostringstream oss;
        oss << "/characters/" << core::path::head(path) << m_count;
        m_namespace = oss.str();
    }

    Character::~Character()
    {
        global::gfx->enterNamespace("/");
        global::gfx->deleteNamespace(m_namespace);
    }

    bool Character::preload()
    {
        /* Loading the name text file. */
        std::ifstream ifs(m_path + "/name");
        if(!ifs) {
            std::ostringstream oss;
            oss << "Couldn't open \"" << m_path + "/name" << "\" while loading " << m_namespace << " character.";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return false;
        }

        if(!std::getline(ifs, m_name)) {
            std::ostringstream oss;
            oss << "Couldn't read the first line of \"" << m_path + "/name" << "\" while loading " << m_namespace << " character.";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return false;
        }
        if(!std::getline(ifs, m_desc)) {
            std::ostringstream oss;
            oss << "Couldn't read the second line of \"" << m_path + "/name" << "\" while loading " << m_namespace << " character.";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return false;
        }

        /* Loading the preview.png picture. */
        if(!global::gfx->createNamespace(m_namespace)) {
            std::ostringstream oss;
            oss << "Couldn't create \"" << m_namespace << "\" namespace for character.";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return false;
        }

        global::gfx->enterNamespace(m_namespace);
        if(!global::gfx->loadTexture("preview", m_path + "/preview.png")) {
            std::ostringstream oss;
            oss << "Couldn't load \"" << m_path << "/preview.png\" texture for " << m_namespace << " character.";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return false;
        }

        /* Loading the preview.lua script. */
        lua::exposure::Save::expose(&m_preview);
        lua::exposure::Graphics::expose(&m_preview);
        if(!m_preview.load(m_path + "/preview.lua")) {
            std::ostringstream oss;
            oss << "Couldn't load \"" << m_path << "/preview.lua\" script for " << m_namespace << " character.";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return false;
        }

        /* Preparing the namespace for the script. */
        if(!global::gfx->createNamespace("script")) {
            std::ostringstream oss;
            oss << "Couldn't create \"" << m_namespace << "/script\" namespace for character.";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return false;
        }
        global::gfx->enterNamespace("script");

        if(!m_preview.existsFunction("validate"))
            m_valid = true;
        else
            m_preview.callFunction("validate", &m_valid);

        if(m_preview.existsFunction("init"))
            m_preview.callFunction<void, std::string>("init", NULL, m_path);

        if(!m_preview.existsFunction("loadPreview")) {
            std::ostringstream oss;
            oss << "Script preview.lua in \"" << m_path << "\" doesn't have the required \"loadPreview\" function.";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return false;
        }
        else
            m_preview.callFunction<void, int>("loadPreview", NULL, (int)None);

        return true;
    }

    bool Character::validated() const
    {
        return m_valid;
    }

    std::string Character::name() const
    {
        return m_name;
    }

    std::string Character::desc() const
    {
        return m_desc;
    }

    void Character::preview(const geometry::AABB& msize) const
    {
        global::gfx->enterNamespace(m_namespace);
        drawPrev(msize);
    }

    void Character::bigPreview(Color color, const geometry::AABB& msize)
    {
        global::gfx->enterNamespace(m_namespace + "/script");
        m_preview.callFunction<void, int>("loadPreview", NULL, (int)color);
        drawPrev(msize);
    }

    void Character::drawPrev(const geometry::AABB& msize) const
    {
        if(global::gfx->rctype("preview") != graphics::Graphics::TEXT)
            return;

        geometry::AABB used = msize;
        geometry::Point dec(0.0f, 0.0f);

        float ratioSize = used.width / used.height;
        float ratioPict = (float)global::gfx->getTextureWidth("preview") / (float)global::gfx->getTextureHeight("preview");
        if(ratioPict > ratioSize) {
            used.height = used.width / ratioPict;
            dec.y = (msize.height - used.height) / 2.0f;
        }
        else {
            used.width = used.height * ratioPict;
            dec.x = (msize.width - used.width) / 2.0f;
        }

        global::gfx->push();
        global::gfx->move(dec.x, dec.y);
        global::gfx->draw(used, "preview");
        global::gfx->pop();
    }

    bool Character::load(Color c)
    {
        /* Loading the script. */
        lua::exposure::Save::expose(&m_perso);
        lua::exposure::Graphics::expose(&m_perso);
        if(!m_perso.load(m_path + "/perso.lua")) {
            std::ostringstream oss;
            oss << "Couldn't load \"" << m_path << "/perso.lua\" script for " << m_namespace << " character.";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return false;
        }

        /* Preparing the namespace. */
        global::gfx->enterNamespace(m_namespace);
        if(!global::gfx->createNamespace("perso")) {
            std::ostringstream oss;
            oss << "Couldn't create \"" << m_namespace << "/perso\" namespace for character.";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return false;
        }
        global::gfx->enterNamespace("perso");

        /* Checking the presence of necessary methods. */
        bool valid = true;
        valid = valid && checkFunc(m_perso, "perso.lua", "init");
        valid = valid && checkFunc(m_perso, "perso.lua", "walk");
        valid = valid && checkFunc(m_perso, "perso.lua", "run");
        valid = valid && checkFunc(m_perso, "perso.lua", "stop");
        valid = valid && checkFunc(m_perso, "perso.lua", "jump");
        valid = valid && checkFunc(m_perso, "perso.lua", "jumpAir");
        valid = valid && checkFunc(m_perso, "perso.lua", "down");
        valid = valid && checkFunc(m_perso, "perso.lua", "fastDown");
        valid = valid && checkFunc(m_perso, "perso.lua", "land");
        valid = valid && checkFunc(m_perso, "perso.lua", "stand");
        valid = valid && checkFunc(m_perso, "perso.lua", "attack");
        valid = valid && checkFunc(m_perso, "perso.lua", "spell");
        valid = valid && checkFunc(m_perso, "perso.lua", "smash");
        valid = valid && checkFunc(m_perso, "perso.lua", "shield");
        valid = valid && checkFunc(m_perso, "perso.lua", "staticDodge");
        valid = valid && checkFunc(m_perso, "perso.lua", "flyingStaticDodge");
        valid = valid && checkFunc(m_perso, "perso.lua", "dashDodge");
        valid = valid && checkFunc(m_perso, "perso.lua", "flyingDashDodge");
        valid = valid && checkFunc(m_perso, "perso.lua", "appear");
        valid = valid && checkFunc(m_perso, "perso.lua", "won");
        valid = valid && checkFunc(m_perso, "perso.lua", "lost");
        valid = valid && checkFunc(m_perso, "perso.lua", "sent");
        if(!valid)
            return false;

        /* Make the script load the character rcs. */
        bool ret;
        m_perso.callFunction<bool, int>("init", &ret, (int)c);
        if(!ret) {
            std::ostringstream oss;
            oss << "Couldn't load ressources for character \"" << m_namespace << "\".";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return false;
        }

        action(Walk, Fixed);
        return true;
    }

    void Character::action(Control control, Direction dir)
    {
        m_begin = SDL_GetTicks();

        /** @todo Make transitions (depends on physics). */
        switch(control) {
            case Walk:
                m_next.id = ActionID::None;
                switch(dir) {
                    case Left:  m_actual.id = ActionID::Walk; 
                                m_actual.flip = false; 
                                break;
                    case Right: m_actual.id = ActionID::Walk;
                                m_actual.flip = true;
                                break;
                    case Up:    m_actual.id = ActionID::Jump;      break; /* Handle jump air if not on ground. */
                    case Down:  m_actual.id = ActionID::FastDown;  break;
                    case Fixed: m_actual.id = ActionID::Stand;     break;
                    default:    m_actual.id = ActionID::None;      break;
                }
                m_next.flip = m_actual.flip;
                break;

            case Run:
                m_next.id = ActionID::None;
                switch(dir) {
                    case Left:  m_actual.id = ActionID::Run;
                                m_actual.flip = false; 
                                break;
                    case Right: m_actual.id = ActionID::Run;
                                m_actual.flip = true; 
                                break;
                    case Up:    m_actual.id = ActionID::Jump;     break; /* Handle jump air if not on ground. */
                    case Down:  m_actual.id = ActionID::FastDown; break;
                    case Fixed: m_actual.id = ActionID::Stop;
                                m_next.id = ActionID::Stand;
                                break;
                    default:    m_actual.id = ActionID::None;     break;
                }
                m_next.flip = m_actual.flip;
                break;

            case Attack:
                m_next.id = ActionID::Stand;
                switch(dir) {
                    case Left:  m_actual.id = ActionID::AttackSide;
                                m_actual.flip = false; 
                                break;
                    case Right: m_actual.id = ActionID::AttackSide;
                                m_actual.flip = true; 
                                break;
                    case Up:    m_actual.id = ActionID::AttackUp;    break;
                    case Down:  m_actual.id = ActionID::AttackDown;  break;
                    case Fixed: m_actual.id = ActionID::Attack;      break;
                    default:    m_actual.id = ActionID::None;        break;
                }
                m_next.flip = m_actual.flip;
                break;

            case Spell:
                m_next.id = ActionID::Stand;
                switch(dir) {
                    case Left:  m_actual.id = ActionID::SpellSide;
                                m_actual.flip = false; 
                                break;
                    case Right: m_actual.id = ActionID::SpellSide;
                                m_actual.flip = true; 
                                break;
                    case Up:    m_actual.id = ActionID::SpellUp;    break;
                    case Down:  m_actual.id = ActionID::SpellDown;  break;
                    case Fixed: m_actual.id = ActionID::Spell;      break;
                    default:    m_actual.id = ActionID::None;       break;
                }
                break;

            case Smash:
                m_next.id = ActionID::Stand;
                switch(dir) {
                    case Left:  m_actual.id = ActionID::SmashSide;
                                m_actual.flip = false; 
                                break;
                    case Right: m_actual.id = ActionID::SmashSide;
                                m_actual.flip = true; 
                                break;
                    case Up:    m_actual.id = ActionID::SmashUp;    break;
                    case Down:  m_actual.id = ActionID::SmashDown;  break;
                    case Fixed: m_actual.id = ActionID::Stand;      break;
                    default:    m_actual.id = ActionID::None;       break;
                }
                m_next.flip = m_actual.flip;
                break;

            case Dodge:
                /* Handle flying dodge. */
                m_actual.id = ActionID::StaticDodge;
                m_next.id   = ActionID::Stand;
                m_next.flip = m_actual.flip;
                break;

            case DDodge:
                /* Handle flying dodge. */
                m_next.id = ActionID::Stand;
                switch(dir) {
                    case Left:  m_actual.id = ActionID::DashDodge;
                                m_actual.flip = false; 
                                break;
                    case Right: m_actual.id = ActionID::DashDodge;
                                m_actual.flip = true; 
                                break;
                    case Up: case Down: case Fixed:
                    default:    m_actual.id = ActionID::Stand;          break;
                }
                m_next.flip = m_actual.flip;
                break;

            case Catch:
                m_next.id = ActionID::Stand;
                switch(dir) {
                    case Left:  m_actual.id = ActionID::Catch;
                                m_actual.flip = false;
                                break;
                    case Right: m_actual.id = ActionID::Catch;
                                m_actual.flip = true;
                                break;
                    case Up: case Down: case Fixed:
                    default:    m_actual.id = ActionID::Stand;      break;
                }
                m_next.flip = m_actual.flip;
                break;

            default:
                m_next.id   = ActionID::None;
                m_actual.id = ActionID::Stand;
                break;
        }
    }

    void Character::draw()
    {
        /* Getting physic position. */
        geometry::Point pos(0.0f, 0.0f);

        /* Updating animation. */
        actuateByLua();

        /* Drawing. */
        global::gfx->enterNamespace(m_namespace);
        global::gfx->enterNamespace("perso");
        global::gfx->blitTexture("drawed", pos);
    }
            
    void Character::actuateByLua()
    {
        unsigned long ms = SDL_GetTicks() - m_begin;
        /* TODO */
    }

    bool Character::checkFunc(lua::Script& script, const std::string& nm, const std::string& func)
    {
        if(script.existsFunction(func))
            return true;
        std::ostringstream oss;
        oss << "Script " << nm << " in \"" << m_path << "\" doesn't hace the required \"" << func << "\" function.";
        core::logger::logm(oss.str(), core::logger::WARNING);
        return false;
    }

}

