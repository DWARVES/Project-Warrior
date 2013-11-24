
#ifndef DEF_AUDIO_MUSIC
#define DEF_AUDIO_MUSIC

#include <SDL_mixer.h>
#include <string>

namespace audio
{
    /** @brief Contains internal class and methods internally used by audio::Audio. */
    namespace internal
    {
        /** @brief Represents a music. */
        class Music
        {
            public:
                Music();
                Music(const Music&) = delete;
                ~Music();

                /** @brief Load a music in the actual namespace and and read its begin and end loop from a file.
                 * @param path The path to a text file indicating the begin and end of the repetition loop,
                 * and the path to the real music file (which can be of any format handled by Mix_LoadMUS).
                 */
                bool load(const std::string& path);
                /** @brief Load a music and set its repetition loop to the whole music. */
                bool loadraw(const std::string& path);

                /** @brief Change the beggining of the repetion loop.
                 * @param nvalue The new value, mustn't be after the end of the repetition loop nor under 0.
                 * @return The value really setted.
                 */
                int bloop(int nvalue);
                /** @brief Returns the beggining of the repetition loop. */
                int bloop() const;
                /** @brief Change the end of the repetion loop.
                 * @param nvalue The new value, mustn't be before the beggining.
                 * @return The value really setted.
                 */
                int eloop(int nvalue);
                /** @brief Returns the end of the repetition loop. */
                int eloop() const;

                /** @brief Provides direct access to the Mix_Music m_mus pointer. */
                Mix_Music* get();

            private:
                int m_bloop,      /**< @brief Beggining of the repetion loop. */
                    m_eloop;      /**< @brief End of the repetion loop. */
                Mix_Music* m_mus; /**< @brief The music stored by SDL_Mixer. */
        };
    }
}

#endif

