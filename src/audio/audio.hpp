
#ifndef DEF_AUDIO_AUDIO
#define DEF_AUDIO_AUDIO

#include <string>
#include "core/fakefs.hpp"
#include "music.hpp"
#include "sound.hpp"

/** @brief Contains all the functions and class related to audio functionnalities. */
namespace audio
{
    /** @brief Class used to manage audio.
     *
     * It can load and play sounds and musics. It also manages volume.
     */
    class Audio
    {
        public:
            Audio();
            Audio(const Audio&) = delete;
            ~Audio();

            /*************************
             *   Opening the audio   *
             *************************/
            /** @brief Init the audio at a specified frequence.
             *
             * Must be called only once each execution.
             * @param freq The frequence used when playing sounds and musics.
             * @return False if couldn't open audio. If that's the case, you mustn't use any other methods of audio::Audio, which may lead to an undefined behaviour.
             */
            bool init(int freq);
            /** @brief Returns the actual frequency. */
            int frequency() const;

            /*************************
             *       Volume          *
             *************************/
            /** @brief Set the music volume.
             * @param volume The volume, between 0 (no sound) and 255 (max volume).
             */
            unsigned char musicVolume(unsigned char volume);
            /** @brief Returns the actual music volume. */
            unsigned char musicVolume() const;
            /** @brief Set the sound volume.
             * @param volume The volume, between 0 (no sound) and 255 (max volume).
             */
            unsigned char soundsVolume(unsigned char volume);
            /** @brief Returns the actual sound volume. */
            unsigned char soundsVolume() const;

            /*************************
             * Namespaces managment  *
             *************************/
            /** @brief Set a new actual namespace, works as core::FakeFS::enterNamespace. */
            bool enterNamespace(const std::string& name);
            /** @brief Create a namespace, works as core::FakeFS::createNamespace. */
            bool createNamespace(const std::string& name);
            /** @brief Delete an existing namespace, works as core::FakeFS::deleteNamespace. */
            void deleteNamespace(const std::string& name);
            /** @brief Get the absolute path of the actual namespace. */
            std::string actualNamespace() const;

            /*************************
             *  Ressources loading   *
             *************************/
            /** @brief Load a sound in the actual namespace.
             * @param name The name of the sound, mustn't be already used.
             * @param path The path to the sound file. Accepts any format accepted by SDL_LoadWAV.
             * @return False if something went wrong. If that's the case, nothing will have been loaded.
             */
            bool loadSound(const std::string& name, const std::string& path);
            /** @brief Load a music in the actual namespace and and read its begin and end loop from a file.
             * @param name The name of the music, mustn't be already used.
             * @param path The path to a text file indicating the begin loop, the end loop and the path to the real music file (accepts any format accepted by SDL_LoadMUS).
             * @return False if something went wrong. If that's the case, nothing will have been loaded.
             */
            bool loadMusic(const std::string& name, const std::string& path);
            /** @brief Load a music in the actual namespace and sets its begin loop to 0 and its end loop to the end of the music.
             * @param name The name of the music, mustn't be already used.
             * @param path The path to the music file. Accepts any format accepted by SDL_LoadMUS.
             * @return False if something went wrong. If that's the case, nothing will have been loaded.
             */
            bool loadRawMusic(const std::string& name, const std::string& path);
            /** @brief Free and delete a music/sound (no distinction done). */
            void free(const std::string& name);

            /** @brief The type of a ressource. */
            enum RcType {SOUND, /**< @brief The ressource is a sound. */
                MUSIC,          /**< @brief The ressource is a music. */
                NONE            /**< @brief Value used to represent an error and/or an unexisting value. */
            };
            /** @brief Return the type of a ressource or NONE if name doesn't exists. */
            RcType rctype(const std::string& name) const;
            /** @brief Links name to target, works as core::FakeFS::link. */
            bool link(const std::string& name, const std::string& target);

            /*************************
             *   Music managment     *
             *************************/
            /** @brief Set the begin of the repetition loop of a music in seconds.
             * @param name The name of the music. Nothing will happen if name designate a sound or nothing.
             * @param secs The begin of the repetition loop in seconds from the beginning of the music. If begin is after the end or outside the music, nothing will be changed.
             * @return 0 if an error happened or the new value of the beggining of the repetition loop. Be careful, it may also returns 0 if that's the new value setted.
             */
            int musicBeginLoop(const std::string& name, int secs);
            /** @brief Returns the begin of the repetition loop of a music. */
            int musicBeginLoop(const std::string& name) const;
            /** @brief Set the end of the repetition loop of a music in seconds.
             * @param name The name of the music. Nothing will happen if name designate a sound or nothing.
             * @param secs The end of the repetition loop in seconds from the beginning of the music. If end is after the beginning, nothing will be changed.
             * @return 0 if an error happened or the new value of the end of the repetition loop. Be careful, it may also returns 0 if that's the new value setted.
             */
            int musicEndLoop(const std::string& name, int secs);
            /** @brief Returns the end of the repetition loop of a music. */
            int musicEndLoop(const std::string& name) const;

            /*************************
             *   Callbacks type      *
             *************************/
            /** @brief A pure virtual class for callbacks. Any callback (see audio::Audio::play) must inherit from this class. */
            class Callback
            {
                public:
                    /** @brief The method called as callback.
                     * Its parameter is a pointer to the Audio structure playing the music.
                     */
                    virtual void operator()(Audio*) = 0;
                    virtual ~Callback();
            };

            /*************************
             *       Playing         *
             *************************/
            /** @brief Play a music or a sound.
             *
             * Only one music can be played at the same time. A new call to this function will simply replace the played music.
             * @param name The name of the music to play. If it's a sound, it will be palyed to the first free chennal, witheout discarding anything. In this case, the others parameters are meaningless, and will be ignored.
             * @param replay If true, if the playing music is the same as name, it will be played again from beggining.
             * @param loops The number of repetition (using the repetition loop borders). If <= 0, it will be played indefinitly.
             * @param cb A callback to be called at the end of the playing. Its use is only valid if loops > 0.
             * @param tofree If true, cb will be deleted after being called.
             */
            void play(const std::string& name, bool replay = true, int loops = 0, Callback* cb = NULL, bool tofree = false);
            /** @brief Plays only the end of a music (can be used for transitions for example).
             *
             * It will discard the music played to replace it.
             * @param name The music which end will be played.
             * @param cb The callback to call after everything is played (works as in audio::Audio::play).
             * @param tofree If true, cb will be deleted after being called.
             */
            void end(const std::string& name, Callback* cb = NULL, bool tofree = false);
            /** @brief Checks the position of the playing music and call the necessary callbacks.
             * Must be called in every frame
             */
            void update();

        private:
            int m_freq;           /**< @brief The frequence of the output. */
            unsigned char m_musV; /**< @brief The music volume (0-255). */
            unsigned char m_sndV; /**< @brief The sound volume (0-255). */
            Uint32 m_btime;       /**< @brief Time at the beggining of the previous playing loop. */
            Callback* m_cb;       /**< @brief The callback actually setted. */
            int m_loops;          /**< @brief Number of loops still needing to be played, negative if music must be playing indefinitely. */
            int m_beg,            /**< @brief The beggining of the repetition loop for the actual music. */
                m_end;            /**< @brief The end of the repetition loop for the actual music. */
            bool m_playing;       /**< @brief Indicates if a music is actually being played. */
            std::string m_played; /**< @brief The path to the playing music. */
            bool m_tofree;        /**< @brief Indicate if m_cb must be free'd after being called. */

            /*************************
             *   Fake-Fs structure   *
             *************************/
            /** @brief Represents an entity, either a sound or a music, stored. */
            struct Entity
            {
                /** @brief Union of possibles value of the entity. */
                union Stored {
                    internal::Music* music; /**< @brief Value accessed if the entity is a music. */
                    internal::Sound* sound; /**< @brief Value accessed if the entity is a sound. */
                };
                Stored stored; /**< @brief The real value of the entity. */
                RcType type;   /**< @brief Indicates the type of the entity (which field of stored must be accessed). */
            };

            /** @brief Call used to free an entity. */
            class EntityLiberator
            {
                public:
                    /** @brief Free tofree and its contents. */
                    void operator()(Entity* tofree) const;
            };

            /** @brief FakeFS storing the musics and sounds. */
            core::FakeFS<Entity*, EntityLiberator> m_fs;

            /*************************
             *  Internal functions   *
             *************************/
            /** @brief Convert a volume in range 0-255 (setted by the user) to range 0-MIX_MAX_VALUME (used by SDL_Mixer). */
            unsigned char clamp(unsigned char volume) const;
    };
}

#endif

