
#include "graphics/movie.hpp"
#include "geometry/point.hpp"
#include "core/logger.hpp"
extern "C" {
#include <libavutil/avutil.h>
}
#include <sstream>
#include <algorithm>

namespace graphics
{
    namespace internal
    {
        Movie::Movie()
            : m_playing(false), m_speed(1.0f), m_ltime(0), m_stime(0),
            m_begin(true), m_sbytes(0),
            m_ctx(NULL), m_codecCtx(NULL), m_codec(NULL), m_frame(NULL), m_video(-1)
        {}

        Movie::~Movie()
        {
            if(m_frame != NULL)
                av_free(m_frame);
            if(m_codecCtx != NULL)
                avcodec_close(m_codecCtx);
            if(m_ctx != NULL)
                avformat_close_input(&m_ctx);
        }

        bool Movie::load(const std::string& path)
        {
            /* Load the file */
            if(avformat_open_input(&m_ctx, path.c_str(), NULL, NULL) != 0) {
                core::logger::logm(std::string("Couldn't load movie : ") + path, core::logger::WARNING);
                return false;
            }

            if(avformat_find_stream_info(m_ctx, NULL) < 0) {
                std::ostringstream oss;
                oss << "Couldn't find video information for \"" << path << "\"" << std::endl;
                core::logger::logm(oss.str(), core::logger::WARNING);
                return false;
            }

            /* Find the first video stream */
            for(unsigned int i = 0; i < m_ctx->nb_streams; ++i) {
                if(m_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
                    m_video = i;
                    break;
                }
            }

            if(m_video < 0) {
                std::ostringstream oss;
                oss << "Couldn't find any video stream in \"" << path << "\" video file" << std::endl;
                core::logger::logm(oss.str(), core::logger::WARNING);
                return false;
            }
            m_codecCtx = m_ctx->streams[m_video]->codec;

            /* Find the right codec */
            m_codec = avcodec_find_decoder(m_codecCtx->codec_id);
            if(m_codec == NULL) {
                std::ostringstream oss;
                oss << "Couldn't find codec for \"" << path << "\" video file" << std::endl;
                core::logger::logm(oss.str(), core::logger::WARNING);
                return false;
            }

            /* Enable truncated bitstreams */
            if(m_codec->capabilities & CODEC_CAP_TRUNCATED)
                m_codecCtx->flags |= CODEC_FLAG_TRUNCATED;

            /* Open the codec */
            if(avcodec_open2(m_codecCtx, m_codec, NULL) < 0) {
                std::ostringstream oss;
                oss << "Couldn't open codec for \"" << path << "\" video file" << std::endl;
                core::logger::logm(oss.str(), core::logger::WARNING);
                return false;
            }

            /* Allocate the frame */
            m_frame = avcodec_alloc_frame();

            /* Create the GL texture */
            GLuint text;
            glGenTextures(1, &text);
            m_text.loadgl(text, m_codecCtx->width, m_codecCtx->height);

            return true;
        }

        void Movie::displayFrame(const geometry::AABB& rect, bool r) const
        {
            /* Sending it to openGL texture */
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, m_text.glID());
            /* FIXME convert YUV to RGB */
            glTexImage2D(GL_TEXTURE_2D, 0, 3, m_codecCtx->width, m_codecCtx->height, 0,
                    GL_RGB, GL_UNSIGNED_BYTE, m_frame->data);

            /* Compute the size */
            geometry::AABB applied;
            geometry::Point dec;
            float rat1 = ratio();
            float rat2 = rect.width / rect.height;
            if(!r || std::abs(rat1 - rat2) < 0.0001f) {
                applied = rect;
                dec.x = dec.y = 0;
            }
            else if(rat1 > rat2) {
                applied.width = rect.width;
                applied.height = (float)m_codecCtx->height * rect.width / (float)m_codecCtx->width;
                dec.x = 0;
                dec.y = (rect.height - applied.height) / 2;
            }
            else {
                applied.height = rect.height;
                applied.width = (float)m_codecCtx->width * rect.height / (float)m_codecCtx->height;
                dec.y = 0;
                dec.x = (rect.width - applied.width) / 2;
            }

            /* Draw the frame */
            glTranslatef(dec.x, dec.y, 0.0f);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f,0.0f); glVertex2f(0.0f,          0.0f);
            glTexCoord2f(1.0f,0.0f); glVertex2f(applied.width, 0.0f);
            glTexCoord2f(1.0f,1.0f); glVertex2f(applied.width, applied.height);
            glTexCoord2f(0.0f,1.0f); glVertex2f(0.0f,          applied.height);
            glEnd();
            glTranslatef(-dec.x, -dec.y, 0.0f);
        }

        bool Movie::nextFrame()
        {
            int bytesDecoded;
            int frameFinished;

            if(m_begin) {
                m_begin = false;
                m_packet.data = NULL;
            }

            /* Decode packets until we have decoded a complete frame */
            while(true) {
                /* Work on the current packet */
                while(m_sbytes > 0) {
                    bytesDecoded = avcodec_decode_video2(m_codecCtx, m_frame, &frameFinished, &m_packet);
                    if(bytesDecoded < 0) {
                        core::logger::logm("Failed decoding a video frame, may be the end of the video", core::logger::MSG);
                        return false;
                    }

                    m_sbytes -= bytesDecoded;

                    if(frameFinished)
                        return true;
                }

                /* Get the next packet, skipping all packets that are not for this stream */
                do {
                    if(m_packet.data != NULL) {
                        av_free_packet(&m_packet);
                        m_packet.data = NULL;
                    }
                    if(av_read_frame(m_ctx, &m_packet) < 0)
                        goto loop_exit;
                } while(m_packet.stream_index != m_video);

                m_sbytes = m_packet.size;
            }

loop_exit:
            /* Decode the end of the last frame */
            bytesDecoded = avcodec_decode_video2(m_codecCtx, m_frame, &frameFinished, &m_packet);
            if(m_packet.data != NULL) {
                av_free_packet(&m_packet);
                m_packet.data = NULL;
            }

            return frameFinished != 0;
        }

        bool Movie::updateFrame()
        {
            if(m_begin) {
                m_ltime = SDL_GetTicks();
                m_stime = static_cast<Uint32>(m_speed * (float)frameTime());
                return nextFrame();
            }

            Uint32 etime = SDL_GetTicks() - m_ltime;
            while(etime > m_stime) {
                if(!nextFrame())
                    return false;
                etime -= m_stime;
                m_stime = static_cast<Uint32>(m_speed * (float)frameTime());
            }
            m_stime -= etime;
            m_ltime = SDL_GetTicks();
            return true;
        }

        void Movie::replay()
        {
            m_begin = true;
            /* TODO */
        }

        void Movie::speed(float fact)
        {
            m_speed = fact;
        }

        float Movie::speed() const
        {
            return m_speed;
        }

        unsigned int Movie::frameTime() const
        {
            return static_cast<unsigned int>(1000.0f * (float)m_codecCtx->time_base.num / (float)m_codecCtx->time_base.den);
        }

        float Movie::ratio() const
        {
            return (float)m_codecCtx->width / (float)m_codecCtx->height;
        }

        void Movie::init()
        {
            av_register_all();
        }

        void Movie::free()
        {
            /* Nothing to do */
        }
    }
}


