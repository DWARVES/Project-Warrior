
#include "evsave.hpp"
#include "events.hpp"
#include <boost/regex.hpp>

namespace events
{
    std::map<std::string,EvSave*> EvSave::m_registered;

    EvSave::EvSave()
    {}

    EvSave::~EvSave()
    {}

    void EvSave::regist(EvSave* sv)
    {
        if(m_registered.find(sv->id()) != m_registered.end())
            return;
        else
            m_registered[sv->id()] = sv;
    }

    EvSave* EvSave::parse(const std::string& sv)
    {
        boost::regex type ("^\\s*\\((.+?)\\).*");
        boost::smatch results;
        if(!boost::regex_match(sv, results, type))
            return NULL;

        if(m_registered.find(results[1]) == m_registered.end())
            return NULL;
        
        EvSave* ev = m_registered[results[1]];
        if(!ev->load(sv))
            return NULL;
        return ev->copy();
    }

    void EvSave::freeAll()
    {
        for(auto p : m_registered)
            delete p.second;
        m_registered.clear();
    }

}

