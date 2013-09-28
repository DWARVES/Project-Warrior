
#ifndef DEF_AUDIO_SOUND
#define DEF_AUDIO_SOUND

#include <SDL_mixer.h>
#include <string>

namespace audio
{
    namespace internal
    {
        class Sound
        {
            public:
                Sound();
                Sound(const Sound&) = delete;
                ~Sound();
                bool load(const std::string& path);
                Mix_Chunk* get() const;

            private:
                Mix_Chunk* m_snd;
        };
    }
}

#endif

