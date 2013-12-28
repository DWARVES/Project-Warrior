
#include "audio.hpp"
#include "core/logger.hpp"
#include <sstream>
#include <SDL_mixer.h>
#include <SDL.h>

#define NB_CHANNELS 64

namespace audio
{
    Audio::Audio()
        : m_freq(0), m_musV(0), m_sndV(0), m_btime(0), m_cb(NULL),
        m_loops(0), m_beg(0), m_end(0), m_playing(false), m_tofree(false)
    {}

    Audio:: ~Audio()
    {
        /* If the audio was destroyed before the callback could be called and free'd */
        if(m_cb && m_tofree)
            delete m_cb;
    }

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
            core::logger::logm(oss.str(), core::logger::MSG);
            m_freq = freq;
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
        /* Load the sound */
        internal::Sound* snd = new internal::Sound;
        if(!snd->load(path)) {
            delete snd;
            return false;
        }

        /* Add it to the Fake-FS */
        Entity* ent = new Entity;
        ent->stored.sound = snd;
        ent->type = SOUND;
        if(!m_fs.createEntity(name, ent)) {
            std::ostringstream oss;
            oss << "Couldn't add " << path << " sound to the fake-fs as \"" << name << "\".";
            core::logger::logm(oss.str(), core::logger::WARNING);

            EntityLiberator lib;
            lib(ent);
            return false;
        }
        else
            return true;
    }

    bool Audio::loadMusic(const std::string& name, const std::string& path)
    {
        /* Load the music */
        internal::Music* mus = new internal::Music;
        if(!mus->load(path)) {
            delete mus;
            return false;
        }

        /* Add it to the Fake-FS */
        Entity* ent = new Entity;
        ent->stored.music = mus;
        ent->type = MUSIC;
        if(!m_fs.createEntity(name, ent)) {
            std::ostringstream oss;
            oss << "Couldn't add " << path << " music to the fake-fs as \"" << name << "\".";
            core::logger::logm(oss.str(), core::logger::WARNING);

            EntityLiberator lib;
            lib(ent);
            return false;
        }
        else
            return true;
    }

    bool Audio::loadRawMusic(const std::string& name, const std::string& path)
    {
        /* Load the music */
        internal::Music* mus = new internal::Music;
        if(!mus->loadraw(path)) {
            delete mus;
            return false;
        }

        /* Add it to the Fake-FS */
        Entity* ent = new Entity;
        ent->stored.music = mus;
        ent->type = MUSIC;
        if(!m_fs.createEntity(name, ent)) {
            std::ostringstream oss;
            oss << "Couldn't add " << path << " raw music to the fake-fs as \"" << name << "\".";
            core::logger::logm(oss.str(), core::logger::WARNING);

            EntityLiberator lib;
            lib(ent);
            return false;
        }
        else
            return true;
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

    int Audio::musicBeginLoop(const std::string& name, int secs)
    {
        if(rctype(name) != MUSIC)
            return 0;
        return m_fs.getEntityValue(name)->stored.music->bloop(static_cast<Uint32>(secs));
    }

    int Audio::musicBeginLoop(const std::string& name) const
    {
        return m_fs.getEntityValue(name)->stored.music->bloop();
    }

    int Audio::musicEndLoop(const std::string& name, int secs)
    {
        if(rctype(name) != MUSIC)
            return 0;
        return m_fs.getEntityValue(name)->stored.music->eloop(static_cast<Uint32>(secs));
    }

    int Audio::musicEndLoop(const std::string& name) const
    {
        return m_fs.getEntityValue(name)->stored.music->eloop();
    }

    void Audio::play(const std::string& name, bool replay, int loops, Audio::Callback* cb, bool tofree)
    {
        RcType t = rctype(name);
        if(t == NONE) {
            std::ostringstream oss;
            oss << "Tryed to play an unexistant audio ressource : \"" << name << "\".";
            core::logger::logm(oss.str(), core::logger::WARNING);
        }

        else if(t == SOUND) {
            Mix_PlayChannel(-1, m_fs.getEntityValue(name)->stored.sound->get(), 0);
        }

        else if(t == MUSIC) {
            std::string toplay = actualNamespace() + "/" + name;
            toplay = core::path::cleanSep(toplay);
            if(toplay == m_played && !replay)
                return;
            m_played = toplay;

            if(loops <= 0) loops = -1;
            internal::Music* mus = m_fs.getEntityValue(name)->stored.music;
            Mix_PlayMusic(mus->get(), 1);
            m_loops = loops;
            m_cb = cb;
            m_beg = mus->bloop() * 1000;
            m_end = mus->eloop() * 1000;
            m_btime = SDL_GetTicks();
            m_playing = true;
            m_tofree = tofree;
        }

        else {
            std::ostringstream oss;
            oss << "Tryed to play an unhandled type of audio ressource : \"" << name << "\".";
            core::logger::logm(oss.str(), core::logger::ERROR);
        }
    }

    void Audio::end(const std::string& name, Audio::Callback* cb, bool tofree)
    {
        if(rctype(name) != MUSIC)
            return;
        play(name, true, 0, cb, tofree);
        Mix_RewindMusic(); /* Only necessary for mp3 musics */
        Mix_SetMusicPosition(m_end);
    }
            
    void Audio::update()
    {
        if(!m_playing)
            return;

        Uint32 time = SDL_GetTicks() - m_btime;
        if(time > m_end && m_loops != 0) { /* The music has reached the end of a loop */
            m_btime = SDL_GetTicks() - m_beg;
            Mix_RewindMusic(); /* Only necessary for mp3 music */
            Mix_SetMusicPosition(m_beg/1000);
            if(m_loops > 0)
                --m_loops;
            return;
        }

        if(!Mix_PlayingMusic()) { /* the music has ended */
            if(m_cb) {
                (*m_cb)(this);
                if(m_tofree) {
                    delete m_cb;
                    m_cb = NULL;
                }
            }
            m_playing = false;
        }
    }

    unsigned char Audio::clamp(unsigned char volume) const
    {
        float fact = (float)MIX_MAX_VOLUME / 255.0f;
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
                   
    Audio::Callback::~Callback()
    {
        /* Nothing to do */
    }
}



