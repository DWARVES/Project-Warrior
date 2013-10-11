
#include "config.hpp"
#include <fstream>
#include <boost/regex.hpp>

namespace core
{
    ConfigBadKeyException::ConfigBadKeyException(const char* mname) noexcept
        : m_name(mname)
    {}

    ConfigBadKeyException::~ConfigBadKeyException()
    {
        /* Nothing to do */
    }

    const char* ConfigBadKeyException::what() const noexcept
    {
        return "Tryed to acces a config option with an invalid key.";
    }

    const char* ConfigBadKeyException::name() const noexcept
    {
        return m_name;
    }

    Config::Config()
    {}

    Config::~Config()
    {}

    bool Config::args(int argc, char *argv[])
    {
        boost::regex longc  ("^--([^=]*)$");      /* Regex for a long-named argument without value in it */
        boost::regex longcv ("^--([^=]*)=(.*)$"); /* Regex for a long-named argument with value in it */
        boost::regex shortc ("^-([^-].*)$");          /* Regex for a short-named argument */
        boost::smatch results;
        std::string tostore; /* Name of the option in which storing the value */

        for(int i = 1; i < argc; ++i) {
            std::string arg(argv[i]);

            /* Is the arg a long-named option without value */
            if(boost::regex_match(arg, results, longc)) {
                std::string name = results[1];
                if(!m_fs.existsEntity(name)) {
                    std::ostringstream oss;
                    oss << "Passed an unrecognized argument : \"" << name << "\"."; 
                    core::logger::logm(oss.str(), core::logger::WARNING);
                    return false;
                }
                tostore = name;
                m_fs.setEntityValue(tostore, "1"); /* If a value is set, it will override this */
            }

            /* Is the arg a long-named option with value */
            else if(boost::regex_match(arg, results, longcv)) {
                std::string name = results[1];
                if(!m_fs.existsEntity(name)) {
                    std::ostringstream oss;
                    oss << "Passed an unrecognized argument : \"" << name << "\"."; 
                    core::logger::logm(oss.str(), core::logger::WARNING);
                    return false;
                }
                m_fs.setEntityValue(results[1], results[2]);
                tostore.clear();
            }

            /* Is the arg a short-named option */
            else if(boost::regex_match(arg, results, shortc)) {
                std::string lst = results[1];
                for(size_t j = 0; j < lst.size(); ++j) {
                    if(m_shorts.find(lst[j]) == m_shorts.end()) {
                        std::ostringstream oss;
                        oss << "Passed an unrecognized one-letter argument : '" << lst[j] << "'."; 
                        core::logger::logm(oss.str(), core::logger::WARNING);
                        return false;
                    }
                    tostore = m_shorts[lst[j]];
                    m_fs.setEntityValue(tostore, "1"); /* If a value is set, it will override this */
                }
            }

            /* Is the arg a value */
            else {
                if(tostore == "") {
                    std::ostringstream oss;
                    oss << "Option value \"" << arg << "\" witheout option corresponding in command line.";
                    logger::logm(oss.str(), logger::WARNING);
                    continue; /* Non-fatal error */
                }
                /* tostore existence should have been tested before */
                m_fs.setEntityValue(tostore, argv[i]);
                tostore.clear();
            }
        }

        return true;
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
        logger::logm("Loaded config file : " + path, logger::MSG);

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
            os << "\" : " << it->second.d << "\n";
        }
    }

    void Config::dump(std::ostream& os)
    {
        m_fs.save(os, [](const std::string& str) {return str;});
    }

}



