
#include "music.hpp"
#include "core/logger.hpp"
#include "core/fakefs.hpp"
#include "core/pathParser.hpp"
#include <sstream>
#include <fstream>

namespace audio
{
    namespace internal
    {
        Music::Music()
            /* eloop is set to a big value to be sure it will englob the music */
            : m_bloop(0), m_eloop(10000), m_mus(NULL)
        {}

        Music::~Music()
        {
            if(m_mus != NULL)
                Mix_FreeMusic(m_mus);
        }

        bool Music::load(const std::string& path)
        {
            /* Opening the file */
            std::ifstream ifs(path);
            if(!ifs) {
                std::ostringstream oss;
                oss << "Couldn't open " << path << " descriptive music file.";
                core::logger::logm(oss.str(), core::logger::WARNING);
                return false;
            }

            /* Parsing the file */
            core::FakeFS<std::string> file;
            if(!file.load(ifs, [](const std::string& str) { return str; } )) {
                std::ostringstream oss;
                oss << "Couldn't parse " << path << " descriptiive music file.";
                core::logger::logm(oss.str(), core::logger::WARNING);
                return false;
            }

            /* Get the path to the music */
            if(!file.existsEntity("music")) {
                std::ostringstream oss;
                oss << "Descriptive music file " << path << " does not have \"music\" field.";
                core::logger::logm(oss.str(), core::logger::WARNING);
                return false;
            }
            std::string musPath = file.getEntityValue("music");

            /* Make the path relative */
            std::string filePath;
            if(core::path::absolute(musPath))
                filePath = musPath;
            else {
                filePath = core::path::body(path);
                filePath += '/';
                filePath += musPath;
            }

            if(!loadraw(filePath))
                return false;

            /* Tests and gets the values */
            if(file.existsEntity("bloop")) {
                std::istringstream iss(file.getEntityValue("bloop"));
                iss >> m_bloop;
            }
            else
                m_bloop = 0;

            if(file.existsEntity("eloop")) {
                std::istringstream iss(file.getEntityValue("eloop"));
                iss >> m_eloop;
            }
            else {
                /* eloop is set to a big value to be sure it will englob the music */
                m_eloop = 10000;
            }

            return true;
        }

        bool Music::loadraw(const std::string& path)
        {
            m_mus = Mix_LoadMUS(path.c_str());
            if(!m_mus) {
                std::ostringstream oss;
                oss << "Couldn't load the music file \"" << path << "\" : \"" << Mix_GetError() << "\".";
                core::logger::logm(oss.str(), core::logger::WARNING);
                return false;
            }
            else
                return true;
        }

        int Music::bloop(int nvalue)
        {
            if(nvalue < m_eloop)
                m_bloop = nvalue;
            return m_bloop;
        }

        int Music::bloop() const
        {
            return m_bloop;
        }

        int Music::eloop(int nvalue)
        {
            if(nvalue > m_bloop)
                m_eloop = nvalue;
            return m_eloop;
        }

        int Music::eloop() const
        {
            return m_eloop;
        }

        Mix_Music* Music::get()
        {
            return m_mus;
        }

    }
}



