
#include "graphics/movie.hpp"
#include "geometry/point.hpp"
#include "core/logger.hpp"
extern "C" {
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}
#include <sstream>
#include <algorithm>

namespace graphics
{
    namespace internal
    {
        void SaveFrame(AVPicture *pFrame, int width, int height)
        {
            static int iFrame = 0;
            FILE *pFile;
            char szFilename[32];
            int  y;

            // Open file
            sprintf(szFilename, "frames/frame%d.ppm", iFrame);
            pFile=fopen(szFilename, "wb");
            if(pFile==NULL)
                return;

            // Write header
            fprintf(pFile, "P6\n%d %d\n255\n", width, height);

            // Write pixel data
            for(y=0; y<height; y++)
                fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);

            // Close file
            fclose(pFile);
            ++iFrame;
        }

        void saveTexture(GLuint id) {
            static int iFrame = 0;
            /* Get the texture pixmap */
            int width, height;
            glBindTexture(GL_TEXTURE_2D, id);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &height);
            char* buffer = new char[width * height * 3];
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_BYTE, buffer);
            
            /* Convert it to an SDL picture */
            SDL_Surface* surf = SDL_CreateRGBSurfaceFrom(buffer, width, height, 24, width * 3, 0, 0, 0, 0);
            if(surf == NULL) {
                core::logger::logm("Couldn't save the opengl texture.", core::logger::MSG);
                return;
            }

            /* Save it */
            char file[32];
            sprintf(file, "texts/frame%d.bmp", iFrame);
            SDL_SaveBMP(surf, file);
            delete[] buffer;
            ++iFrame;
        }

        Movie::Movie(Shaders* s)
            : m_playing(false), m_speed(1.0f), m_ltime(0), m_stime(0), m_s(s),
            m_begin(true), m_sbytes(0),
            m_ctx(NULL), m_codecCtx(NULL), m_codec(NULL), m_frame(NULL), m_video(-1)
        {
            m_rgb.data[0] = NULL;
        }

        Movie::~Movie()
        {
            if(m_frame != NULL)
                av_free(m_frame);
            if(m_rgb.data[0] != NULL)
                avpicture_free(&m_rgb);
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

            /* Allocate the frames */
            m_frame = avcodec_alloc_frame();
            if(avpicture_alloc(&m_rgb, PIX_FMT_RGB24, 256, 256) < 0) {
                core::logger::logm("Couldn't generate rgb AVPicture for movie playing.", core::logger::WARNING);
                return false;
            }

            /* Create the GL texture */
            GLuint text;
            glGenTextures(1, &text);
            m_text.loadgl(text, m_codecCtx->width, m_codecCtx->height);

            return true;
        }

        void Movie::displayFrame(const geometry::AABB& rect, bool r) const
        {
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
            m_s->yuv(false);
            m_s->text(true);
            glBindTexture(GL_TEXTURE_2D, m_text.glID());
            glTranslatef(dec.x, dec.y, 0.0f);
            glColor3ub(255, 255, 255);
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
                        goto to_rgb;
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

            if(frameFinished == 0) {
                core::logger::logm("Couldn't decode the next video frame.", core::logger::WARNING);
                return false;
            }

to_rgb:
            /* Converting the frame to rgb */
            /* FIXME must cvtCtx be free'd ? */
            /* FIXME find the right type for cvtCtx */
            auto cvtCtx = sws_getContext(m_codecCtx->width, m_codecCtx->height, m_codecCtx->pix_fmt, 
                    256, 256, PIX_FMT_RGB24, SWS_BICUBIC,
                    NULL, NULL, NULL);
            if(cvtCtx == NULL) {
                core::logger::logm("Couldn't convert the frame, undefined graphic behaviour may happen.", core::logger::WARNING);
                /* Not considered as a fatal error (jumping one frame shouldn't impact too much, so return true */
                return true;
            }
            sws_scale(cvtCtx, (const uint8_t* const*)m_frame->data, m_frame->linesize, 0, m_codecCtx->height,
                    m_rgb.data, m_rgb.linesize);
            SaveFrame(&m_rgb, 256, 256);

            /* Sending it to openGL texture */
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, m_text.glID());
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0,
                    GL_RGB, GL_UNSIGNED_BYTE, m_frame->data[0]);
            saveTexture(m_text.glID());

            return true;
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
            unsigned int ret = static_cast<unsigned int>(1000.0f * (float)m_codecCtx->time_base.num / (float)m_codecCtx->time_base.den);
            if(ret > 5)
                return ret;
            else /* If the frametime is lower than five, set it to 30fps = 33frametime */
                return 33;
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


