
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
    int Stage::m_count = 0;

    Stage::Stage(const std::string& path)
        : m_path(path), m_namespace(getNamespace()), m_valid(false), m_nbPlayers(0)
    {}

    Stage::~Stage()
    {}

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

        for(int i = 0; i < m_nbPlayers; ++i)
            m_ctrls[i]->attached()->world(&m_world);

        /* Loading the lua script. */
        lua::exposure::Save::expose(&m_script);
        lua::exposure::Graphics::expose(&m_script);
        lua::exposure::Path::expose(&m_script);
        lua::exposure::Character::expose(&m_script);
        if(!m_script.load(m_path + "/stage.lua"))
            return false;
        m_script.setVariable("characterNB", m_nbPlayers);
        if(!m_script.existsFunction("init")) {
            std::ostringstream oss;
            oss << "The stage script for \"" << m_path << "\" doesn't have a init function.";
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

        return true;
    }

    void Stage::update(const events::Events& ev)
    {
        /* TODO */
    }

    void Stage::draw()
    {
        /* TODO */
    }
            
    void Stage::centerView()
    {
        /* TODO */
    }

}

