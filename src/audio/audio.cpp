
#include "audio.hpp"
#include "core/logger.hpp"
#include <sstream>
#include <SDL_mixer.h>

#define NB_CHANNELS 64

namespace audio
{
    Audio::Audio()
        : m_freq(0), m_musV(0), m_sndV(0)
    {}

    Audio:: ~Audio()
    {}

    bool Audio::init(int freq)
    {
        if(Mix_OpenAudio(freq, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0) {
            std::ostringstream oss;
            oss << "Couldn't open audio with a frequency of " << freq << " : \"" << Mix_GetError() << "\".";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return false;
        }
        else {
            std::ostringstream oss;
            oss << "Audio opened with a frequency of " << freq << ".";
            core::logger::logm(oss.str(), core::logger::WARNING);
            return true;
        }
        Mix_AllocateChannels(NB_CHANNELS);
    }

    int Audio::frequency() const
    {
        return m_freq;
    }

    unsigned char Audio::musicVolume(unsigned char volume)
    {
        m_musV = volume;
        Mix_VolumeMusic(clamp(volume));
        return m_musV;
    }

    unsigned char Audio::musicVolume() const
    {
        return m_musV;
    }

    unsigned char Audio::soundsVolume(unsigned char volume)
    {
        m_sndV = volume;
        for(int i = 0; i < NB_CHANNELS; ++i) {
            Mix_Volume(i, clamp(volume));
        }
        return m_sndV;
    }

    unsigned char Audio::soundsVolume() const
    {
        return m_sndV;
    }

    bool Audio::enterNamespace(const std::string& name)
    {
        return m_fs.enterNamespace(name);
    }

    bool Audio::createNamespace(const std::string& name)
    {
        return m_fs.createNamespace(name);
    }

    void Audio::deleteNamespace(const std::string& name)
    {
        m_fs.deleteNamespace(name);
    }

    std::string Audio::actualNamespace() const
    {
        return m_fs.actualNamespace();
    }

    bool Audio::loadSound(const std::string& name, const std::string& path)
    {
    }

    bool Audio::loadMusic(const std::string& name, const std::string& path)
    {
    }

    bool Audio::loadRawMusic(const std::string& name, const std::string& path)
    {
    }

    void Audio::free(const std::string& name)
    {
        m_fs.deleteEntity(name);
    }

    Audio::RcType Audio::rctype(const std::string& name) const
    {
        if(!m_fs.existsEntity(name))
            return NONE;
        return m_fs.getEntityValue(name)->type;
    }

    bool Audio::link(const std::string& name, const std::string& target)
    {
        return m_fs.link(name, target);
    }

    size_t Audio::musicBeginLoop(const std::string& name, size_t secs)
    {
    }

    size_t Audio::musicBeginLoop(const std::string& name) const
    {
    }

    size_t Audio::musicEndLoop(const std::string& name, size_t secs)
    {
    }

    size_t Audio::musicEndLoop(const std::string& name) const
    {
    }

    void Audio::play(const std::string& name, int loops)
    {
    }

    void Audio::rewind(const std::string& name)
    {
    }

    void Audio::end(const std::string& name)
    {
    }
            
    void Audio::update()
    {
    }

    unsigned char Audio::clamp(unsigned char volume) const
    {
        float fact = MIX_MAX_VOLUME / 255;
        return static_cast<unsigned char>(float(volume) * fact);
    }
    
    void Audio::EntityLiberator::operator()(Entity* tofree) const
    {
        if(!tofree)
            return;
        if(tofree->type == MUSIC)
            delete tofree->stored.music;
        else if(tofree->type == SOUND)
            delete tofree->stored.sound;
        delete tofree;
    }
}



