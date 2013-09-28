
#include "sound.hpp"
#include "core/logger.hpp"
#include <sstream>

namespace audio
{
    namespace internal
    {
        Sound::Sound()
            : m_snd(NULL)
        {}

        Sound::~Sound()
        {
            if(m_snd != NULL)
                Mix_FreeChunk(m_snd);
        }

        bool Sound::load(const std::string& path)
        {
            m_snd = Mix_LoadWAV(path.c_str());
            if(!m_snd) {
                std::ostringstream oss;
                oss << "Couldn't load \"" << path << "\" sound file : \"" << Mix_GetError() << "\".";
                core::logger::logm(oss.str(), core::logger::WARNING);
                return false;
            }
            else
                return true;
        }

        Mix_Chunk* Sound::get() const
        {
            return m_snd;
        }

    }
}


