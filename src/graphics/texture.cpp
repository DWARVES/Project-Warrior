
#include "graphics/texture.hpp"
#include <SDL_image.h>

namespace graphics
{
    namespace internal
    {
        Texture::Texture()
            : m_loaded(false), m_id(0), m_w(0), m_h(0)
        {
            m_hp.x = m_hp.y = 0.0f;
        }

        Texture::~Texture()
        {
            if(m_loaded)
                glDeleteTextures(1, &m_id);
        }
                
        SDL_Surface* Texture::preload(const std::string& path)
        {
            /* Load the picture with SDL_image */
            SDL_Surface* src = IMG_Load(path.c_str());
            if(src == NULL)
                return NULL;

            /* Convert it to the right format */
            Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            rmask = 0xff000000;
            gmask = 0x00ff0000;
            bmask = 0x0000ff00;
            amask = 0x000000ff;
#else
            rmask = 0x000000ff;
            gmask = 0x0000ff00;
            bmask = 0x00ff0000;
            amask = 0xff000000;
#endif

            SDL_PixelFormat fmt = *(src->format);
            fmt.BitsPerPixel = 32;
            fmt.BytesPerPixel = 4;
            fmt.Rmask = rmask;
            fmt.Gmask = gmask;
            fmt.Bmask = bmask;
            fmt.Amask = amask;

            SDL_Surface* converted = SDL_ConvertSurface(src, &fmt, 0);
            SDL_FreeSurface(src);
            if(converted == NULL )
                return NULL;
            return converted;
        }

        bool Texture::loadsdl(SDL_Surface* src)
        {
            /* Convert it to the opengl format */
            GLuint id;
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexImage2D(GL_TEXTURE_2D, 0, 4, src->w,
                    src->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                    src->pixels);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            /* Store it */
            m_id = id;
            m_w = src->w;
            m_h = src->h;
            m_loaded = true;

            return true;
        }
                
        bool Texture::loadgl(GLuint id)
        {
            m_id = id;
            return true;
        }

        bool Texture::load(const std::string& path)
        {
            SDL_Surface* surf = preload(path);
            if(surf == NULL)
                return false;
            if(!loadsdl(surf))
                return false;
            SDL_FreeSurface(surf);
            return true;
        }

        bool Texture::loaded() const
        {
            return m_loaded;
        }

        GLuint Texture::glID() const
        {
            return m_id;
        }

        int Texture::width() const
        {
            return m_w;
        }

        int Texture::height() const
        {
            return m_h;
        }

        void Texture::hotpoint(const geometry::Point& hp)
        {
            m_hp = hp;
        }

        geometry::Point Texture::hotpoint() const
        {
            return m_hp;
        }

    }
}


