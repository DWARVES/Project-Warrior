
#ifndef DEF_GRAPHICS_MOVIE
#define DEF_GRAPHICS_MOVIE

#include <string>
#include <SDL.h>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}
#include "geometry/aabb.hpp"
#include "graphics/shaders.hpp"
#include "graphics/texture.hpp"

namespace graphics
{
    namespace internal
    {
        class Movie
        {
            public:
                Movie(Shaders* s);
                Movie() = delete;
                Movie(const Movie&) = delete;
                ~Movie();
                bool load(const std::string& path);

                /* Init and free libavcodec */
                static void init();
                static void free();

                /* Display the actual frame 
                 * If ratio is true, the ratio will be preserved (nothing will be drawn on the bands)
                 */
                void displayFrame(const geometry::AABB& rect, bool r = true) const;
                /* Go to the next frame, return false if the end is reached */
                bool nextFrame();
                /* Go to the next frame if enough time has been spent (respect speed video) */
                bool updateFrame();
                void replay();

                /* Speed factor (a factor of two will make the video twice as fast as it is normally) */
                void speed(float fact);
                float speed() const;

                /* Return the time in milliseconds of a framerate */
                unsigned int frameTime() const;
                /* Return the ratio of the video */
                float ratio() const;

            private:
                bool m_playing;
                float m_speed;
                Uint32 m_ltime;
                Uint32 m_stime;
                Shaders* m_s;

                bool m_begin;
                AVPacket m_packet;
                int m_sbytes;

                AVFormatContext* m_ctx;
                AVCodecContext* m_codecCtx;
                AVCodec* m_codec;
                AVFrame* m_frame;
                int m_video;
                
                Texture m_text;
                AVPicture m_rgb;
        };
    }
}

#endif

