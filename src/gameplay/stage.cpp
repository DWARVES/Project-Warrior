
#include "stage.hpp"
#include "global.hpp"
#include "core/pathParser.hpp"
#include "core/logger.hpp"
#include "lua/graphicsExposure.hpp"
#include "lua/saveExposure.hpp"
#include "lua/pathExposure.hpp"
#include "lua/charaExposure.hpp"
#include <sstream>
#include <fstream>

namespace gameplay
{
    /** @brief Number of pixels per physic unit. */
    const int pixelsPerPhysic = 100;
    /** @brief Time of appearance of characters in milliseconds. */
    const Uint32 appearTime = 5000;
    int Stage::m_count = 0;

    Stage::Stage(const std::string& path)
        : m_path(path), m_namespace(getNamespace()), m_valid(false), m_nbPlayers(0), m_started(false)
    {}

    Stage::~Stage()
    {
        global::gfx->enterNamespace("/");
        global::gfx->deleteNamespace(m_namespace);

        m_world.enterNamespace("/");
        if(m_world.existsNamespace(m_namespace))
            m_world.deleteNamespace(m_namespace);
    }

    bool Stage::preload()
    {
        if(core::path::type(m_path) != core::path::Type::Dir)
            return false;

        /* Loading the name. */
        std::ifstream ifs(m_path + "/name");
        if(!ifs) {
            std::ostringstream oss;
            oss << "Couldn't open file " << m_path + "/name" << " when loading the stage " << m_path << ".";
            core::logger::logm(oss.str(), core::logger::ERROR);
            return false;
        }
        std::getline(ifs, m_name);
        ifs.close();

        /* Loading the preview picture. */
        std::string path = m_path + "/preview.png";
        global::gfx->enterNamespace(m_namespace);
        if(!global::gfx->loadTexture("preview", path)) {
            std::ostringstream oss;
            oss << "Couldn't load texture " << path <<" when loading the stage " << m_path << ".";
            core::logger::logm(oss.str(), core::logger::ERROR);
            return false;
        }

        /* Loading the lua script. */
        path = m_path + "/validate.lua";
        lua::Script script;
        lua::exposure::Save::expose(&script);
        lua::exposure::Graphics::expose(&script);
        if(!script.load(path)) {
            std::ostringstream oss;
            oss << "Couldn't load lua script " << path << " when loading the stage " << m_path << " : the stage will be automaticly validated.";
            core::logger::logm(oss.str(), core::logger::MSG);
            m_valid = true;
            return true;
        }

        if(!script.existsFunction("validate")) {
            std::ostringstream oss;
            oss << "The lua script " << path << " is invalid : the stage will be automaticly validated.";
            core::logger::logm(oss.str(), core::logger::MSG);
            m_valid = true;
            return true;
        }

        script.callFunction<bool>("validate", &m_valid);
        return true;
    }

    bool Stage::validated() const
    {
        return m_valid;
    }

    std::string Stage::name() const
    {
        return m_name;
    }

    void Stage::draw(const geometry::AABB& rect) const
    {
        global::gfx->enterNamespace(m_namespace);
        geometry::Point dec(0.0f, 0.0f);
        geometry::AABB used = rect;
        float ratioSize = rect.width / rect.height;
        float ratioPict = (float)global::gfx->getTextureWidth("preview") / (float)global::gfx->getTextureHeight("preview");

        if(ratioPict > ratioSize) {
            used.height = used.width / ratioPict;
            dec.y = (rect.height - used.height) / 2.0f;
        }
        else {
            used.width = used.height * ratioPict;
            dec.x = (rect.width - used.width) / 2.0f;
        }

        global::gfx->push();
        global::gfx->move(dec.x, dec.y);
        global::gfx->draw(used, "preview");
        global::gfx->pop();
    }

    std::string Stage::getNamespace()
    {
        std::ostringstream oss;
        oss << "/stages/" << m_count << "/";
        global::gfx->createNamespace(oss.str());

        ++m_count;
        return oss.str();
    }

    bool Stage::load(gameplay::Controler* ctrls[4])
    {
        /* Checking the number of players. */
        m_nbPlayers = 0;
        for(int i = 0; i < 4; ++i) {
            m_ctrls[i] = ctrls[i];
            if(m_ctrls[i])
                ++m_nbPlayers;
        }

        /* Creating the physic world. */
        m_world.setXGravity(0.0f);
        m_world.setYGravity(-10.0f);
        if(!m_world.existsNamespace(m_namespace))
            m_world.createNamespace(m_namespace);
        m_world.enterNamespace(m_namespace);

        /* Loading the lua script. */
        lua::exposure::Save::expose(&m_script);
        lua::exposure::Graphics::expose(&m_script);
        global::gfx->enterNamespace(m_namespace);
        lua::exposure::Path::expose(&m_script);
        lua::exposure::Character::expose(&m_script);
        lua::exposure::Stage::expose(&m_script);
        lua::exposure::Stage::setUsedStage(this);
        if(!m_script.load(m_path + "/stage.lua"))
            return false;
        m_script.setVariable("characterNB", m_nbPlayers);
        if(!m_script.existsFunction("init")) {
            std::ostringstream oss;
            oss << "The stage script for \"" << m_path << "\" doesn't have a init function.";
            core::logger::logm(oss.str(), core::logger::ERROR);
            return false;
        }
        m_drawbg       = m_script.existsFunction("drawBG");
        m_drawstaticbg = m_script.existsFunction("drawStaticBG");
        m_drawstaticfg = m_script.existsFunction("drawStaticFG");
        m_drawfg       = m_script.existsFunction("drawFG");
        if(!m_drawbg && !m_drawstaticbg && !m_drawstaticfg && !m_drawfg) {
            std::ostringstream oss;
            oss << "The stage script for \"" << m_path << "\" doesn't have any drawing function.";
            core::logger::logm(oss.str(), core::logger::ERROR);
            return false;
        }

        bool ret;
        m_script.callFunction<bool, std::string>("init", &ret, m_path);
        if(!ret) {
            std::ostringstream oss;
            oss << "The stage script for \"" << m_path << "\" failed on init.";
            core::logger::logm(oss.str(), core::logger::ERROR);
            return false;
        }

        /* Setting up the drawing. */
        global::gfx->invertYAxis(true);
        m_windowRect.width = (float)global::gfx->windowWidth() / (float)pixelsPerPhysic;
        m_windowRect.height = (float)global::gfx->windowHeight() / (float)pixelsPerPhysic;
        geometry::AABB toshow = ratioResize(m_maxSize, m_windowRect, true).first;
        global::gfx->setVirtualSize(toshow.width, toshow.height);
        for(int i = 0; i < m_nbPlayers; ++i)
            m_ctrls[i]->attached()->physicMSize(1.0f, true);
        m_world.enableDebugDraw(global::cfg->get<bool>("phdebug"));

        m_justLoaded = true;
        m_beggining = 0;
        for(int i = 0; i < m_nbPlayers; ++i) {
            m_ctrls[i]->attached()->appearancePos(m_appearPos[i]);
            m_ctrls[i]->attached()->world(&m_world);
            ((physics::Character*)m_ctrls[i]->attached()->entity())->setID(i);
        }

        return true;
    }

    void Stage::update(const events::Events&)
    {
        /* Controls update. */
        for(int i = 0; i < m_nbPlayers; ++i)
            m_ctrls[i]->update();

        /* First call. */
        if(m_justLoaded) {
            m_justLoaded = false;
            m_started = false;
            m_beggining = SDL_GetTicks();
            return;
        }

        /* Appearance and starts. */
        Uint32 time = SDL_GetTicks() - m_beggining;
        if(time <= appearTime)
            return;
        if(!m_started) {
            m_started = true;
            m_world.start();
        }

        /* Lua in callbacks. */
        for(auto it = m_callbacks.begin(); it != m_callbacks.end(); ++it) {
            EntityCallbacks cbs = it->second;
            if(!cbs.in.empty()) {
                for(int i = 0; i < m_nbPlayers; ++i) {
                    if(cbs.charas[i])
                        m_script.callFunction<void,int>(cbs.in, NULL, i);
                }
            }
        }

        m_world.step();

        /* Death of characters. */
        for(int i = 0; i < m_nbPlayers; ++i) {
            if(!isIn(m_ctrls[i]->attached()->getPos(), m_deathRect, m_center))
                m_ctrls[i]->attached()->die();
        }
    }
            
    float Stage::appearProgress() const
    {
        Uint32 time = SDL_GetTicks() - m_beggining;
        if(time > appearTime)
            return 1.0f;
        else
            return (float)time / (float)appearTime;
    }

    void Stage::draw()
    {
        /* Drawing the BG. */
        global::gfx->setVirtualSize(m_windowRect.width, m_windowRect.height);
        if(m_drawstaticbg) {
            global::gfx->enterNamespace(m_namespace);
            m_script.callFunction<void>("drawStaticBG", NULL);
        }

        centerView();
        if(m_drawbg) {
            global::gfx->enterNamespace(m_namespace);
            m_script.callFunction<void>("drawBG", NULL);
        }

        /* Drawing the appearance. */
        Uint32 time = SDL_GetTicks() - m_beggining;
        if(time <= appearTime) {
            float percent = (float)time / (float)appearTime * 100.0f;
            for(int i = 0; i < m_nbPlayers; ++i) {
                geometry::AABB size = m_ctrls[i]->attached()->phSize();
                global::gfx->push();
                global::gfx->move(m_appearPos[i].x, m_appearPos[i].y);
                m_ctrls[i]->attached()->appear(percent, size);
                global::gfx->pop();
            }
        }

        /* Drawing the game. */
        else {
            for(int i = 0; i < m_nbPlayers; ++i)
                m_ctrls[i]->attached()->draw();
        }

        /* Drawing the FG. */
        if(m_drawfg) {
            global::gfx->enterNamespace(m_namespace);
            m_script.callFunction<void>("drawFG", NULL);
        }
        m_world.debugDraw(global::gfx);

        global::gfx->identity();
        global::gfx->setVirtualSize(m_windowRect.width, m_windowRect.height);
        if(m_drawstaticfg) {
            global::gfx->enterNamespace(m_namespace);
            m_script.callFunction<void>("drawStaticFG", NULL);
        }
    }
            
    void Stage::centerView()
    {
        /* Getting all the centers of characters. */
        std::vector<geometry::Point> centers;
        centers.reserve(4);
        for(int i = 0; i < m_nbPlayers; ++i) {
            if(!m_ctrls[i]->attached()->dead())
                centers.push_back(project(m_ctrls[i]->attached()->getPos(), m_maxSize, m_center));
        }

        geometry::AABB englobe;
        geometry::Point center;

        /* If no character on screen, display the center of map with maxSize. */
        if(centers.size() == 0) {
            englobe = ratioResize(m_windowRect, m_maxSize, true).first;
            center = m_center;
        }
        /* If one character on screen, center on him with minSize. */
        else if(centers.size() == 1) {
            englobe = ratioResize(m_windowRect, m_minSize, true).first;
            center = centers[0];
        }

        /* Compute the extremities of the rect with all centers. */
        else {
            geometry::Point p1(centers[0]);
            geometry::Point p2(centers[0]);
            for(geometry::Point p : centers) {
                p1.x = std::min(p1.x, p.x);
                p1.y = std::min(p1.y, p.y);
                p2.x = std::max(p2.x, p.x);
                p2.y = std::max(p2.y, p.y);
            }

            /* Compute rect and its center. */
            center.x = (p1.x + p2.x) / 2.0f;
            center.y = (p1.y + p2.y) / 2.0f;
            geometry::AABB rect(p2.x - p1.x, p2.y - p1.y);
            /* Adding marging in the rect. */
            rect.width *= 1.5f;
            rect.height *= 1.5f;

            /* Apply m_minSize and m_maxSize. */
            geometry::AABB minWin  = ratioResize(m_windowRect, m_minSize, true).first;
            englobe = ratioResize(m_windowRect, rect, true).first;
            if(englobe.width < minWin.width)
                englobe = minWin;
        }

        /* Resize to fit in m_maxSize. */
        float resw = m_maxSize.width / englobe.width;
        float resh = m_maxSize.height / englobe.height;
        if(resw < 1.0f || resh < 1.0f) {
            float fact = (resw < resh ? resw : resh);
            englobe.width  *= fact;
            englobe.height *= fact;
        }

        /* Move to be in m_maxSize. */
        float decx = 0.0f;
        float decy = 0.0f;
        geometry::Point p1ms(m_center.x - m_maxSize.width / 2.0f, m_center.y - m_maxSize.height / 2.0f);
        geometry::Point p2ms(m_center.x + m_maxSize.width / 2.0f, m_center.y + m_maxSize.height / 2.0f);
        geometry::Point p1cv(center.x - englobe.width / 2.0f, center.y - englobe.height / 2.0f);
        geometry::Point p2cv(center.x + englobe.width / 2.0f, center.y + englobe.height / 2.0f);

        if(p1cv.y < p1ms.y)
            decy = (p1ms.y - p1cv.y);
        else if(p2cv.y > p2ms.y)
            decy = (p2ms.y - p2cv.y);

        if(p1cv.x < p1ms.x)
            decx = (p1ms.x - p1cv.x);
        else if(p2cv.x > p2ms.x)
            decx = (p2ms.x - p2cv.x);

        center.x += decx;
        center.y += decy;

        /* Apply in graphics. */
        global::gfx->setVirtualSize(englobe.width, englobe.height);
        global::gfx->move(-center.x + englobe.width/2.0f, -center.y + englobe.height/2.0f);
    }
            
    std::pair<geometry::AABB,geometry::Point> Stage::ratioResize(const geometry::AABB& res, const geometry::AABB& fit, bool large) const
    {
        geometry::AABB result;
        geometry::Point dec;

        /* Compute result size. */
        float r1 = res.width / res.height;
        float r2 = fit.width / fit.height;
        if(r1 < r2) {
            if(large) {
                result.width = fit.width;
                result.height = result.width / r1;
            } else {
                result.height = fit.height;
                result.width = result.height * r1;
            }
        } else {
            if(large) {
                result.height = fit.height;
                result.width = result.height * r1;
            } else {
                result.width = fit.width;
                result.height = result.width / r1;
            }
        }

        /* Compute dec. */
        dec.x = (fit.width - result.width) / 2.0f;
        dec.y = (fit.height - result.height) / 2.0f;

        return std::pair<geometry::AABB,geometry::Point>(result, dec);
    }
            
    bool Stage::isIn(const geometry::Point& p, const geometry::AABB& rect, const geometry::Point& center) const
    {
        geometry::Point p1(center.x - rect.width/2.0f, center.y - rect.height/2.0f);
        geometry::Point p2(center.x + rect.width/2.0f, center.y + rect.height/2.0f);
        if(p.x < p1.x
                || p.x > p2.x
                || p.y < p1.y
                || p.y > p2.y)
            return false;
        else
            return true;
    }

    void Stage::setWorldCenter(const geometry::Point& c)
    {
        m_center = c;
    }

    void Stage::setMaxSize(const geometry::AABB& ms)
    {
        m_maxSize = ms;
    }

    void Stage::setDeathRect(const geometry::AABB& dr)
    {
        m_deathRect = dr;
    }

    void Stage::setMinSize(const geometry::AABB& ms)
    {
        m_minSize = ms;
    }

    float Stage::getWindowWidth() const
    {
        return m_windowRect.width;
    }

    float Stage::getWindowHeight() const
    {
        return m_windowRect.height;
    }

    void Stage::setAppearPos(int id, const geometry::Point& p)
    {
        if(id >= 0 && id < 4)
            m_appearPos[id] = p;
    }

    bool Stage::addPlatform(const std::string& nm, const geometry::Point& center, const geometry::AABB& rect, float friction)
    {
        m_world.enterNamespace(m_namespace);
        return m_world.createPlatform(nm, center, rect, friction * 5.0f) != NULL;
    }

    bool Stage::addObstacle(const std::string& nm, const geometry::Point& center, const geometry::AABB& rect, float friction)
    {
        m_world.enterNamespace(m_namespace);
        return m_world.createObstacle(nm, center, rect, friction * 5.0f) != NULL;
    }
            
    bool Stage::addSensor(const std::string& nm, const geometry::Point& center, const geometry::AABB& rect)
    {
        m_world.enterNamespace(m_namespace);
        physics::Entity* ent = m_world.createEntity(nm, center, b2_staticBody);
        if(!ent)
            return false;
        if(!ent->createFixture("main", rect, 1, 1, physics::Entity::Type::ThisType,
                    physics::Entity::Type::ThisCollideWith, geometry::Point(0,0), true))
            return false;
        return true;
    }

    void Stage::removeEntity(const std::string& nm)
    {
        unsetEntityCallbacks(nm);
        m_world.destroyEntity(nm);
    }
            
    void Stage::phcallback(physics::Entity* ground, physics::Entity* chara, bool bg, void* data)
    {
        EntityCallbacks* cbs = (EntityCallbacks*)data;
        if(ground != cbs->ent) /* Shouldn't happen. */
            return;

        int id = 0;
        while(id < 4 && chara != cbs->st->m_ctrls[id]->attached()->entity())
            ++id;
        /* The entity in contact is not a character. */
        if(id >= 4)
            return;

        if(bg) {
            if(!cbs->begin.empty() && !cbs->charas[id])
                cbs->st->m_script.callFunction<void,int>(cbs->begin, NULL, id);
            cbs->charas[id] = true;
        } else {
            if(!cbs->end.empty() && cbs->charas[id])
                cbs->st->m_script.callFunction<void,int>(cbs->end, NULL, id);
            cbs->charas[id] = false;
        }
    }
            
    bool Stage::setEntityCallbacks(const std::string& nm, const std::string& begincontact, const std::string& endcontact, const std::string& incontact)
    {
        m_world.enterNamespace(m_namespace);
        if(!m_world.existsEntity(nm))
            return false;
        m_callbacks[nm].ent = m_world.getEntity(nm);
        m_callbacks[nm].st = this;

        m_callbacks[nm].begin = begincontact;
        if(!begincontact.empty() && !m_script.existsFunction(begincontact))
            m_callbacks[nm].begin.clear();

        m_callbacks[nm].end = endcontact;
        if(!endcontact.empty() && !m_script.existsFunction(endcontact))
            m_callbacks[nm].end.clear();

        m_callbacks[nm].in = incontact;
        if(!begincontact.empty() && !m_script.existsFunction(incontact))
            m_callbacks[nm].in.clear();

        m_world.setCallback(nm, &phcallback, &m_callbacks[nm]);

        return true;
    }
            
    void Stage::unsetEntityCallbacks(const std::string& nm)
    {
        auto it = m_callbacks.find(nm);
        if(it != m_callbacks.end()) {
            m_callbacks.erase(it);
            m_world.removeCallback(nm);
        }
    }
            
    gameplay::Character* Stage::getCharacter(int id) const
    {
        if(id < 0 || id >= m_nbPlayers)
            return NULL;
        else
            return m_ctrls[id]->attached();
    }
            
    geometry::Point Stage::project(const geometry::Point& tp, const geometry::AABB& rect, const geometry::Point& center) const
    {
        geometry::Point p1(center.x - rect.width/2.0f, center.y - rect.height/2.0f);
        geometry::Point p2(center.x + rect.width/2.0f, center.y + rect.height/2.0f);
        if(tp.x < p1.x) {
            if(tp.y < p1.y)
                return p1;
            else if(tp.y > p2.y)
                return geometry::Point(p1.x, p2.y);
            else
                return geometry::Point(p1.x, tp.y);
        }
        else if(tp.x > p2.x) {
            if(tp.y < p1.y)
                return geometry::Point(p2.x, p1.y);
            else if(tp.y > p2.y)
                return p2;
            else
                return geometry::Point(p2.x, tp.y);
        }
        else {
            if(tp.y < p1.y)
                return geometry::Point(tp.x, p1.y);
            else if(tp.y > p2.y)
                return geometry::Point(tp.x, p2.y);
            else
                return tp;
        }
    }

}

