
#ifndef DEF_AUDIO_MUSIC
#define DEF_AUDIO_MUSIC

#include <SDL_mixer.h>
#include <string>

namespace audio
{
    namespace internal
    {
        class Music
        {
            public:
                Music();
                Music(const Music&) = delete;
                ~Music();

                bool load(const std::string& path);
                bool loadraw(const std::string& path);

                int bloop(int nvalue);
                int bloop() const;
                int eloop(int nvalue);
                int eloop() const;

                Mix_Music* get();

            private:
                int m_bloop, m_eloop;
                Mix_Music* m_mus;
        };
    }
}

#endif

