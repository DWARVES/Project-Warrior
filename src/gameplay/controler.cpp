
#include "controler.hpp"
#include "global.hpp"
#include "core/logger.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

namespace gameplay
{
    core::FakeFS<std::string> Controler::m_evs;
    std::string Controler::m_path;

    Controler::Controler(const std::string& id)
        : m_loaded(false), m_ch(NULL)
    {
        m_namespace = "/" + id;
        for(unsigned int i = 0; i < (unsigned int)Last; ++i)
            m_ctrls[i] = NULL;
        /** @todo Open */
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
        /** @todo Implement */
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
            auto result = std::find(plug.begin(), plug.end(), *it);
            ret.push_back(*it);
            if(result == plug.end())
                continue;

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
        if(!m_loaded)
            return;

        /* Setting the new control. */
        if(m_ctrls[(unsigned int)ctrl])
            delete m_ctrls[(unsigned int)ctrl];
        m_ctrls[(unsigned int)ctrl] = nev;

        /* Saving the change. */
        m_evs.enterNamespace(m_namespace);
        /** @todo save. */
    }

}


