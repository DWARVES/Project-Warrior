
#include "events/save.hpp"

namespace events
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

    bool Save::save(const std::string& name) const
    {
        // TODO needs file parsing functions
    }

    bool Save::load(const std::string& name)
    {
        // TODO needs file parsing functions
    }
}


