
#ifndef DEF_AUDIO_AUDIO
#define DEF_AUDIO_AUDIO

#include <string>
#include "core/fakefs.hpp"
#include "music.hpp"
#include "sound.hpp"

namespace audio
{
    class Audio
    {
        public:
            Audio();
            Audio(const Audio&) = delete;
            ~Audio();

            /*************************
             *   Opening the audio   *
             *************************/
            bool init(int freq);
            int frequency() const;

            /*************************
             *       Volume          *
             *************************/
            /* The volume must be between 0 and 255 */
            unsigned char musicVolume(unsigned char volume);
            unsigned char musicVolume() const;
            unsigned char soundsVolume(unsigned char volume);
            unsigned char soundsVolume() const;

            /*************************
             * Namespaces managment  *
             *************************/
            bool enterNamespace(const std::string& name);
            bool createNamespace(const std::string& name);
            void deleteNamespace(const std::string& name);
            std::string actualNamespace() const;

            /*************************
             *  Ressources loading   *
             *************************/
            bool loadSound(const std::string& name, const std::string& path);
            /* Will load a text file with the path to the music and the loop_begin and loop_end indications */
            bool loadMusic(const std::string& name, const std::string& path);
            /* Load directly a music file */
            bool loadRawMusic(const std::string& name, const std::string& path);
            void free(const std::string& name);
            /* Return the type of a ressource or NONE if name doesn't exists */
            enum RcType {SOUND, MUSIC, NONE};
            RcType rctype(const std::string& name) const;
            bool link(const std::string& name, const std::string& target);

            /*************************
             *   Music managment     *
             *************************/
            /* Set the begin of the repetition loop of a music in seconds
             * If begin is after the end or outside the music, it will not be changed
             */
            int musicBeginLoop(const std::string& name, int secs);
            int musicBeginLoop(const std::string& name) const;
            /* If the end is before the beggining, it will not be changed */
            int musicEndLoop(const std::string& name, int secs);
            int musicEndLoop(const std::string& name) const;

            /*************************
             *   Callbacks type      *
             *************************/
            /* Callbacks must inherits from this class */
            class Callback
            {
                public:
                    virtual void operator()(Audio*) = 0;
                    virtual ~Callback();
            };

            /*************************
             *       Playing         *
             *************************/
            /* Only one music can be played once */
            /* If loops <= 0, the music will be played witheout end
             * Else, the end will be played after the loops are finished, and then cb is called if not NULL
             * If name designate a sound, loops will be ignored
             * If tofree, cb is free'd after use
             */
            void play(const std::string& name, int loops = 0, Callback* cb = NULL, bool tofree = false);
            /* Play the end of a music, cb is called at the end */
            void end(const std::string& name, Callback* cb = NULL, bool tofree = false);
            /* Will check the position of the playing music and call the necessary callbacks
             * Must be called in every frame
             */
            void update();

        private:
            int m_freq;
            unsigned char m_musV;
            unsigned char m_sndV;
            Uint32 m_btime;
            Callback* m_cb;
            int m_loops;
            int m_beg, m_end;
            bool m_playing;
            bool m_tofree;

            /*************************
             *   Fake-Fs structure   *
             *************************/
            struct Entity
            {
                union Stored { /* The value really stored */
                    internal::Music* music;
                    internal::Sound* sound;
                };
                Stored stored;
                RcType type;
            };

            class EntityLiberator
            {
                public:
                    void operator()(Entity* tofree) const;
            };

            core::FakeFS<Entity*, EntityLiberator> m_fs;

            /*************************
             *  Internal functions   *
             *************************/
            unsigned char clamp(unsigned char volume) const;
    };
}

#endif

