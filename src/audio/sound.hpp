
#ifndef DEF_AUDIO_SOUND
#define DEF_AUDIO_SOUND

#include <SDL_mixer.h>
#include <string>

namespace audio
{
    namespace internal
    {
        /** @brief Represents a sound. */
        class Sound
        {
            public:
                Sound();
                Sound(const Sound&) = delete;
                ~Sound();
                /** @brief Loads the sound from a file, accepts any format handled by Mix_LoadWAV. */
                bool load(const std::string& path);
                /** @brief Provides direct access to the Mix_Chunk m_snd pointer. */
                Mix_Chunk* get() const;

            private:
                Mix_Chunk* m_snd; /**< The sound stored by SDL_Mixer. */
        };
    }
}

#endif

