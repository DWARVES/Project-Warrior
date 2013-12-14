
#ifndef DEF_GRAPHICS_MOVIE
#define DEF_GRAPHICS_MOVIE

#include <string>
#include <SDL.h>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}
#include "geometry/aabb.hpp"
#include "graphics/shaders.hpp"
#include "graphics/texture.hpp"

namespace graphics
{
    namespace internal
    {
        /** @brief Manages a movie. */
        class Movie
        {
            public:
                Movie(Shaders* s);
                Movie() = delete;
                Movie(const Movie&) = delete;
                ~Movie();
                /** @brief Loads the movie from a file. */
                bool load(const std::string& path);

                /** @brief Init libavcodec. */
                static void init();
                /** @brief Close libavcodec. */
                static void free();

                /** @brief Display the actual frame.
                 * @param rect The part of screen in which the frame will be rendered.
                 * @param r If true, the ratio will be preserved (nothing will be drawn on the bands).
                 * @param invert If true, the frame will be flipped vertically.
                 */
                void displayFrame(const geometry::AABB& rect, bool r = true, bool invert = false) const;
                /** @brief Go to the next frame, return false if the end is reached. */
                bool nextFrame();
                /** @brief Go to the next frame if enough time has been spent (respect speed video). Returns false if the end is reached. */
                bool updateFrame();
                /** @brief Restarts playing from the beggining. */
                void replay();

                /** @brief Sets the speed factor. For example, a factor of 2 will double the speed. */
                void speed(float fact);
                /** @brief Returns the speed factor. */
                float speed() const;

                /** @brief Return the period in milliseconds of a framer. */
                unsigned int frameTime() const;
                /** @brief Return the ratio of the video. */
                float ratio() const;

            private:
                bool m_playing; /**< @brief Is the movie playing. */
                float m_speed;  /**< @brief The speed factor. */
                Uint32 m_ltime; /**< @brief The timestamp of the last time a frame changed. */
                Uint32 m_stime; /**< @brief The time remaining before the next change of frame. */
                Shaders* m_s;   /**< @brief The shaders of the program, used to draw the movie. */

                bool m_begin;      /**< @brief Must the video be restarted. */
                AVPacket m_packet; /**< @brief The libavcodec packet, used to read the video stream. */
                int m_sbytes;      /**< @brief Number of bytes to be read from m_packet. */

                AVFormatContext* m_ctx;     /**< @brief The input video stream. */
                AVCodecContext* m_codecCtx; /**< @brief Data about the codec of the video. */
                AVCodec* m_codec;           /**< @brief The codec of the video. */
                AVFrame* m_frame;           /**< @brief The frame. */
                int m_video;                /**< @brief The channel read. */
                
                Texture m_text;       /**< @brief The texture used to display a frame. */
                AVPicture m_rgb;      /**< @brief The libavcodec picture of a frame. */
                SwsContext* m_swsCtx; /**< @brief Used for the convertion from the frame to the picture. */
                bool m_first;         /**< @brief Indicates if the openGL texture must be created. */

                /* Internal methods */
                void clean(); /**< @brief Cleans the packet, freing its contents. */
        };
    }
}

#endif

