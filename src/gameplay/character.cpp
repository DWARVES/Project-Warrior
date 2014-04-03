
#include "character.hpp"
#include "global.hpp"
#include "core/pathParser.hpp"
#include "core/logger.hpp"
#include "graphics/graphics.hpp"
#include "lua/graphicsExposure.hpp"
#include "lua/saveExposure.hpp"
#include "lua/pathExposure.hpp"
#include "lua/charaExposure.hpp"

#include <sstream>
#include <fstream>
#include <SDL.h>

namespace gameplay
{
    size_t Character::m_count = 0;
    const char* const Character::m_luaCalls[(unsigned int)ActionID::None] = {
        "walk",
        "run",
        "stop",
        "jump",
        "jumpAir",
        "down",
        "fastDown",
        "land",
        "stand",
        "shield",
        "staticDodge",
        "flyingStaticDodge",
        "dashDodge",
        "flyingDashDodge",
        "attack",
        "attackSide",
        "attackUp",
        "attackDown",
        "spell",
        "spellSide",
        "spellDown",
        "spellUp",
        "smashSide",
        "smashUp",
        "smashDown",
        "catch",
        "won",
        "lost",
    };

    Character::Character(const std::string& path)
        : m_path(path), m_name("broken"), m_desc("Couldn't load."), m_valid(false), m_flip(false), m_world(NULL), m_ch(NULL)
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

        if(m_ch && m_world)
            m_world->deleteNamespace(m_namespace);
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
        lua::exposure::Path::expose(&m_preview);
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
        drawPrev("preview", msize);
    }

    void Character::bigPreview(Color color, const geometry::AABB& msize)
    {
        global::gfx->enterNamespace(m_namespace + "/script");
        m_preview.callFunction<void, int>("loadPreview", NULL, (int)color);
        drawPrev("preview", msize);
    }

    void Character::drawPrev(const std::string& nm, const geometry::AABB& msize, bool flip, bool center) const
    {
        if(global::gfx->rctype(nm) != graphics::Graphics::TEXT)
            return;

        geometry::AABB used = msize;
        geometry::Point dec(0.0f, 0.0f);

        float ratioSize = used.width / used.height;
        float ratioPict = (float)global::gfx->getTextureWidth(nm) / (float)global::gfx->getTextureHeight(nm);
        if(ratioPict > ratioSize) {
            used.height = used.width / ratioPict;
            dec.y = (msize.height - used.height) / 2.0f;
        }
        else {
            used.width = used.height * ratioPict;
            if(msize.width < 100000.0f) /* Special value of "infinite" width. */
                dec.x = (msize.width - used.width) / 2.0f;
            else
                dec.x = 0.0f;
        }

        global::gfx->push();
        global::gfx->move(dec.x, dec.y);
        if(center)
            global::gfx->move(-used.width / 2.0f, -used.height / 2.0f);
        if(flip) {
            global::gfx->move(used.width, 0.0f);
            global::gfx->scale(-1.0f, 1.0f);
        }
        global::gfx->draw(used, nm);
        global::gfx->pop();
    }

    bool Character::load(Color c, int nb)
    {
        /* Loading the script. */
        lua::exposure::Save::expose(&m_perso);
        lua::exposure::Graphics::expose(&m_perso);
        lua::exposure::Path::expose(&m_perso);
        lua::exposure::exposeChara(&m_perso, nb);
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
        valid = valid && checkFunc(m_perso, "perso.lua", "attackSide");
        valid = valid && checkFunc(m_perso, "perso.lua", "attackUp");
        valid = valid && checkFunc(m_perso, "perso.lua", "attackDown");
        valid = valid && checkFunc(m_perso, "perso.lua", "spell");
        valid = valid && checkFunc(m_perso, "perso.lua", "spellSide");
        valid = valid && checkFunc(m_perso, "perso.lua", "spellUp");
        valid = valid && checkFunc(m_perso, "perso.lua", "spellDown");
        valid = valid && checkFunc(m_perso, "perso.lua", "smashSide");
        valid = valid && checkFunc(m_perso, "perso.lua", "smashUp");
        valid = valid && checkFunc(m_perso, "perso.lua", "smashDown");
        valid = valid && checkFunc(m_perso, "perso.lua", "shield");
        valid = valid && checkFunc(m_perso, "perso.lua", "staticDodge");
        valid = valid && checkFunc(m_perso, "perso.lua", "flyingStaticDodge");
        valid = valid && checkFunc(m_perso, "perso.lua", "dashDodge");
        valid = valid && checkFunc(m_perso, "perso.lua", "flyingDashDodge");
        valid = valid && checkFunc(m_perso, "perso.lua", "catch");
        valid = valid && checkFunc(m_perso, "perso.lua", "appear");
        valid = valid && checkFunc(m_perso, "perso.lua", "won");
        valid = valid && checkFunc(m_perso, "perso.lua", "lost");
        valid = valid && checkFunc(m_perso, "perso.lua", "sent");
        if(!valid)
            return false;

        /* Make the script load the character rcs. */
        bool ret;
        m_perso.callFunction<bool, std::string, int>("init", &ret, m_path, (int)c);
        if(!ret) {
            std::ostringstream oss;
            oss << "Couldn't load ressources for character \"" << m_namespace << "\".";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return false;
        }

        m_actual.flip = m_next.flip = false;
        action(Walk, Fixed);
        return true;
    }

    void Character::action(Control control, Direction dir)
    {
        Action save = m_actual;
        m_stir = 0.0f;

        if(onGround())
            m_doubleJump = false;

        switch(control) {
            case Walk:
                m_next.id   = ActionID::None;
                m_actual.id = ActionID::None;
                switch(dir) {
                    case Left:  
                        if(onGround() && save.id != ActionID::Jump && save.id != ActionID::JumpAir)
                            m_actual.id = ActionID::Walk; 
                        else if(m_ch->getYLinearVelocity() < 0.0f) { /* going down */
                            m_stir = 50.0f;
                            m_actual.id = ActionID::Down;
                        }
                        else { /* going up */
                            m_stir = 50.0f;
                            m_actual.id = save.id;
                        }
                        m_actual.flip = m_flip; 
                        break;

                    case Right: 
                        if(onGround() && save.id != ActionID::Jump && save.id != ActionID::JumpAir)
                                m_actual.id = ActionID::Walk;
                        else if(m_ch->getYLinearVelocity() < 0.0f) { /* going down */
                            m_stir = 50.0f;
                            m_actual.id = ActionID::Down;
                        }
                        else { /* going up */
                            m_stir = 50.0f;
                            m_actual.id = save.id;
                        }
                        m_actual.flip = !m_flip;
                        break;

                    case Down:
                        if(!onGround())
                            m_actual.id = ActionID::FastDown;
                        else if(save.id == ActionID::Down || save.id == ActionID::FastDown || save.id == ActionID::Land) {
                            m_actual.id = ActionID::Land;
                            m_next.id   = ActionID::Stand;
                        }
                        else
                            m_actual.id = ActionID::Stand;
                        break;

                    case Fixed:
                        if(onGround()) {
                            if(save.id == ActionID::Down || save.id == ActionID::FastDown || save.id == ActionID::Land) {
                                m_actual.id = ActionID::Land;
                                m_next.id   = ActionID::Stand;
                            }
                            else if(save.id == ActionID::Run || save.id == ActionID::Stop) {
                                m_actual.id = ActionID::Stop;
                                m_next.id   = ActionID::Stand;
                            }
                            else if(save.id == ActionID::Jump || save.id == ActionID::JumpAir)
                                m_actual.id = save.id;
                            else
                                m_actual.id = ActionID::Stand;
                        }
                        else if(m_ch->getYLinearVelocity() < 0.0f)
                            m_actual.id = ActionID::Down;
                        break;

                    case Up:
                    default:
                        m_actual.id = ActionID::None;
                        break;
                }
                m_next.flip = m_actual.flip;
                break;

            case Run:
                m_next.id = ActionID::None;
                m_actual.id = ActionID::None;
                switch(dir) {
                    case Left:
                        if(onGround() && save.id != ActionID::Jump && save.id != ActionID::JumpAir)
                                m_actual.id = ActionID::Run;
                        else if(m_ch->getYLinearVelocity() < 0.0f) { /* going down */
                            m_stir = 50.0f;
                            m_actual.id = ActionID::Down;
                        }
                        else {
                            m_stir = 50.0f;
                            m_actual.id = save.id;
                        }
                        m_actual.flip = m_flip; 
                        break;

                    case Right:
                        if(onGround() && save.id != ActionID::Jump && save.id != ActionID::JumpAir)
                                m_actual.id = ActionID::Run;
                        else if(m_ch->getYLinearVelocity() < 0.0f) { /* going down */
                            m_stir = 50.0f;
                            m_actual.id = ActionID::Down;
                        }
                        else {
                            m_stir = 50.0f;
                            m_actual.id = save.id;
                        }
                        m_actual.flip = !m_flip; 
                        break;

                    case Up:
                        if(onGround() && save.id != ActionID::Jump && save.id != ActionID::JumpAir)
                            m_actual.id = ActionID::Jump;
                        else if(!m_doubleJump) {
                            m_actual.id = ActionID::JumpAir;
                            m_doubleJump = true;
                        }
                        else if(save.id == ActionID::Jump || save.id == ActionID::JumpAir)
                            m_actual.id = save.id;
                        break;

                    case Down:
                        action(Walk, Down);
                        return;

                    case Fixed:
                        action(Walk, Fixed);
                        return;

                    default:
                        m_actual.id = ActionID::None;
                        break;
                }
                m_next.flip = m_actual.flip;
                break;

            case Attack:
                m_next.id = ActionID::Stand;
                switch(dir) {
                    case Left:  m_actual.id = ActionID::AttackSide;
                                m_actual.flip = m_flip; 
                                break;
                    case Right: m_actual.id = ActionID::AttackSide;
                                m_actual.flip = !m_flip; 
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
                                m_actual.flip = m_flip; 
                                break;
                    case Right: m_actual.id = ActionID::SpellSide;
                                m_actual.flip = !m_flip; 
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
                                m_actual.flip = m_flip; 
                                break;
                    case Right: m_actual.id = ActionID::SmashSide;
                                m_actual.flip = !m_flip; 
                                break;
                    case Up:    m_actual.id = ActionID::SmashUp;    break;
                    case Down:  m_actual.id = ActionID::SmashDown;  break;
                    case Fixed: m_actual.id = ActionID::Stand;      break;
                    default:    m_actual.id = ActionID::None;       break;
                }
                m_next.flip = m_actual.flip;
                break;

            case Dodge:
                switch(dir) {
                    case Left:
                        if(onGround())
                            m_actual.id = ActionID::DashDodge;
                        else
                            m_actual.id = ActionID::FlyingDashDodge;
                        m_actual.flip = m_flip; 
                        break;

                    case Right:
                        if(onGround())
                            m_actual.id = ActionID::DashDodge;
                        else
                            m_actual.id = ActionID::FlyingDashDodge;
                        m_actual.flip = !m_flip; 
                        break;

                    case Fixed:
                        if(onGround())
                            m_actual.id = ActionID::StaticDodge;
                        else
                            m_actual.id = ActionID::FlyingStaticDodge;
                        break;

                    case Up: case Down:
                    default:
                        if(onGround())
                            m_actual.id = ActionID::Stand;
                        else
                            m_actual.id = ActionID::None;
                        break;
                }
                m_next.id = ActionID::Stand;
                m_next.flip = m_actual.flip;
                break;

            case Catch:
                m_next.id = ActionID::Stand;
                switch(dir) {
                    case Left:  m_actual.id = ActionID::Catch;
                                m_actual.flip = m_flip;
                                break;
                    case Right: m_actual.id = ActionID::Catch;
                                m_actual.flip = !m_flip;
                                break;
                    case Up: case Down: case Fixed:
                    default:    m_actual.id = ActionID::Stand;      break;
                }
                m_next.flip = m_actual.flip;
                break;

            default:
                break;
        }

        actuateByPhysic(m_actual, save);
        if(save.id != m_actual.id)
            m_begin = SDL_GetTicks();
    }

    void Character::actuateByPhysic(Action actual, Action previous)
    {
        if(!m_ch)
            return;

        switch(actual.id) {
            case ActionID::Walk:
                if(actual.flip != m_flip)
                    m_ch->setXLinearVelocity(5.0f);
                else
                    m_ch->setXLinearVelocity(-5.0f);
                break;
            case ActionID::Run:
                if(actual.flip != m_flip)
                    m_ch->setXLinearVelocity(10.0f);
                else
                    m_ch->setXLinearVelocity(-10.0f);
                break;
            case ActionID::Jump:
                if(previous.id != ActionID::Jump)
                    m_ch->jump(m_ch->getXLinearVelocity());
                break;
            case ActionID::JumpAir:
                if(previous.id != ActionID::JumpAir)
                    m_ch->jump(m_ch->getXLinearVelocity());
                break;
            case ActionID::FastDown:
                m_ch->applyForce(0.0f, -20.0f * m_ch->getMass());
                break;
            case ActionID::Stand:
            case ActionID::Land:
            case ActionID::Down:
            case ActionID::Won:
            case ActionID::Lost:
            case ActionID::None:
            default:
                break;
        }

        if(std::abs(m_stir) > 0.00001f) {
            float stir = m_stir;
            if(actual.flip == m_flip)
                stir *= -1.0f;
            m_ch->applyForce(stir, 0.0f);
        }
    }

    void Character::draw()
    {
        /* Getting physic position. */
        geometry::Point pos(0.0f, 0.0f);
        if(m_ch)
            pos = m_ch->getPosition();

        /* Updating animation. */
        actuateByLua();

        /* Drawing. */
        global::gfx->enterNamespace(m_namespace);
        global::gfx->enterNamespace("perso");
        global::gfx->push();
        global::gfx->move(pos.x, pos.y);
        if(m_useMsize)
            drawPrev("drawed", m_msize, m_actual.flip, true);
        else
            global::gfx->blitTexture("drawed", geometry::Point(0.0f,0.0f), m_actual.flip); /* TODO center */
    }

    void Character::actuateByLua()
    {
        if(m_actual.id == ActionID::None)
            return;

        unsigned long ms = SDL_GetTicks() - m_begin;
        bool ret = true;
        m_perso.callFunction<bool, unsigned long>(m_luaCalls[(unsigned int)m_actual.id], &ret, ms);

        if(!ret) {
            actuateByPhysic(m_next, m_actual);
            m_actual = m_next;
            m_begin = SDL_GetTicks();
        }
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

    void Character::appear(float percent, const geometry::AABB& msize)
    {
        global::gfx->enterNamespace(m_namespace);
        global::gfx->enterNamespace("perso");
        m_perso.callFunction<void, float>("appear", NULL, percent);
        drawPrev("drawed", msize);
    }

    void Character::lost(const geometry::AABB& msize)
    {
        m_msize       = msize;
        m_useMsize    = true;
        m_begin       = SDL_GetTicks();
        m_actual.flip = false;
        m_actual.id   = ActionID::Lost;
        m_next.flip   = false;
        m_next.id     = ActionID::Stand;
    }

    void Character::won(const geometry::AABB& msize)
    {
        m_msize       = msize;
        m_useMsize    = true;
        m_begin       = SDL_GetTicks();
        m_actual.flip = false;
        m_actual.id   = ActionID::Won;
        m_next.flip   = false;
        m_next.id     = ActionID::Stand;
    }

    Character* Character::clone() const
    {
        Character* cl = new Character(m_path);
        cl->preload();
        return cl;
    }

    bool Character::world(physics::World* w)
    {
        /* Preparing the world. */
        m_world = w;
        m_world->createNamespace(m_namespace);
        m_world->enterNamespace(m_namespace);

        /* Creating the character. */
        m_ch = m_world->createCharacter(m_name, m_phpos, m_phsize, m_phweight);
        if(!m_ch) {
            std::ostringstream oss;
            oss << "Couldn't create physic entity for character " << m_name << ".";
            core::logger::logm(oss.str(), core::logger::ERROR);
            return false;
        }

        return true;
    }

    void Character::appearancePos(const geometry::Point& pos)
    {
        m_phpos = pos;
    }

    physics::World* Character::world() const
    {
        return m_world;
    }

    bool Character::onGround() const
    {
        if(!m_ch)
            return true;
        else
            return m_ch->onGround();
    }

    bool Character::requireMana(unsigned int mn)
    {
        if(mn > m_mana)
            return false;
        m_mana -= mn;
        return true;
    }

    void Character::addMana(unsigned int mn)
    {
        m_mana = std::max(m_mana + mn, m_manamax);
    }

    unsigned int Character::mana() const
    {
        return m_mana;
    }

    unsigned int Character::manaMax() const
    {
        return m_manamax;
    }

    void Character::phSize(const geometry::AABB& size)
    {
        m_phsize = size;
    }

    void Character::phWeight(float w)
    {
        m_phweight = w;
    }

    void Character::setManaMax(unsigned int m)
    {
        m_manamax = m;
    }

    void Character::enableMSize(bool en, const geometry::AABB& ms)
    {
        m_useMsize = en;
        m_msize = ms;
    }

    void Character::setFlip(bool f)
    {
        m_flip = f;
    }

    void Character::physicMSize(float fact, bool hgh)
    {
        geometry::AABB size = m_phsize;
        size.width  *= fact;
        size.height *= fact;
        if(hgh)
            size.width = 100000.f;
        enableMSize(true, size);
    }

    geometry::Point Character::getPos() const
    {
        return m_ch->getPosition();
    }
            
    void Character::warp(const geometry::Point& p)
    {
        m_ch->setPosition(p);
    }
}

