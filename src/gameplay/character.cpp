
#include "character.hpp"
#include "global.hpp"
#include "core/pathParser.hpp"
#include "core/logger.hpp"
#include "graphics/graphics.hpp"
#include "lua/graphicsExposure.hpp"
#include "lua/saveExposure.hpp"

#include <sstream>
#include <fstream>

namespace gameplay
{
    Character::Character(const std::string& path)
        : m_namespace("/characters/" + core::path::head(path)), m_path(path),
        m_name("broken"), m_desc("Couldn't load."), m_valid(false)
    {}

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

}

