
#ifndef DEF_AUDIO_AUDIO
#define DEF_AUDIO_AUDIO

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
            bool init(int frequency);
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

            /*************************
             *  Ressources loading   *
             *************************/

            /*************************
             *   Music managment     *
             *************************/

            /*************************
             *       Playing         *
             *************************/

        private:
    };
}

#endif

