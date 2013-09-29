
#include "config.hpp"
#include "logger.hpp"
#include <fstream>
#include <sstream>

namespace core
{
    Config::Config()
    {}

    Config::~Config()
    {}

    bool Config::args(int argc, char *argv[])
    {
        /* TODO */
    }

    bool Config::cfg(const std::string& path)
    {
        /* Opening the file */
        std::ifstream ifs(path);
        if(!ifs) {
            std::ostringstream oss;
            oss << "Couldn't open " << path << " config file.";
            logger::logm(oss.str(), logger::ERROR);
            return false;
        }

        /* Parsing it */
        FakeFS<std::string> file;
        if(!file.load(ifs, [](const std::string& str) {return str;} )) {
            std::ostringstream oss;
            oss << "Bad syntax in " << path << " config file.";
            logger::logm(oss.str(), logger::ERROR);
            return false;
        }
        logger::logm("Load config file : " + path, logger::ERROR);

        /* Merging it */
        std::vector<std::string> ents = file.listEntities();
        for(size_t i = 0; i < ents.size(); ++i) {
            if(!m_fs.existsEntity(ents[i])) {
                std::ostringstream oss;
                oss << "Unhandled option \"" << ents[i] << "\" found in " << path << " config file.";
                logger::logm(oss.str(), logger::ERROR);
                return false;
            }
            else
                m_fs.setEntityValue(ents[i], file.getEntityValue(ents[i]));
        }

        return true;
    }

    void Config::help(std::ostream& os)
    {
        for(auto it = m_descs.begin(); it != m_descs.end(); ++it) {
            os << "\"" << it->first;
            if(it->second.s != 0)
                os << "," << it->second.s;
            os << "\" : " << it->second.d;
        }
    }

    void Config::dump(std::ostream& os)
    {
        m_fs.save(os, [](const std::string& str) {return str;});
    }

}




