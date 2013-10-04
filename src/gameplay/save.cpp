
#include "gameplay/save.hpp"
#include "core/logger.hpp"
#include <fstream>

namespace gameplay
{
    core::FakeFS<Save::number_t>* Save::m_fs = NULL;

    Save::Save()
    {}

    Save::~Save()
    {}

    bool Save::init()
    {
        m_fs = new core::FakeFS<number_t>;
        return true;
    }

    void Save::quit()
    {
        if(m_fs)
            delete m_fs;
        m_fs = NULL;
    }

    bool Save::createNamespace(const std::string& name)
    {
        if(!m_fs)
            return false;
        else
            return m_fs->createNamespace(name);
    }

    bool Save::deleteNamespace(const std::string& name)
    {
        if(!m_fs)
            return false;
        else {
            m_fs->deleteNamespace(name);
            return true;
        }
    }

    bool Save::enterNamespace(const std::string& name)
    {
        if(!m_fs)
            return false;
        else
            return m_fs->enterNamespace(name);
    }

    bool Save::existsNamespace(const std::string& name)
    {
        if(!m_fs)
            return false;
        else
            return m_fs->existsNamespace(name);
    }

    bool Save::createVariable(const std::string& name, number_t value)
    {
        if(!m_fs)
            return false;
        else
            return m_fs->createEntity(name, value);
    }

    void Save::deleteVariable(const std::string& name)
    {
        if(!m_fs)
            return;
        else
            m_fs->deleteEntity(name);
    }

    bool Save::setVariable(const std::string& name, number_t nvalue)
    {
        if(!m_fs)
            return false;
        else
            return m_fs->setEntityValue(name, nvalue);
    }

    Save::number_t Save::getVariable(const std::string& name)
    {
        if(!m_fs)
            return 0;
        else
            return m_fs->getEntityValue(name);
    }

    bool Save::save(const std::string&path) const
    {
        std::ofstream ofs(path);
        if(!ofs) {
            core::logger::logm(std::string("Couldn't open file in writting mode : ") + path, core::logger::ERROR);
            return false;
        }

        return m_fs->save(ofs,
                [] (int v) {
                    std::ostringstream oss;
                    oss << v;
                    return oss.str();
                });
    }

    bool Save::load(const std::string& path)
    {
        std::ifstream ifs(path);
        if(!ifs) {
            core::logger::logm(std::string("Couldn't open file in reading mode : ") + path, core::logger::ERROR);
            return false;
        }

        return m_fs->load(ifs,
                [] (const std::string& str) {
                    std::istringstream iss(str); int v;
                    iss >> v;
                    return v;
                });
    }

    bool Save::existsVariable(const std::string& name)
    {
        return m_fs->existsEntity(name);
    }
            
    std::string Save::actualNamespace()
    {
        return m_fs->actualNamespace();
    }
}


