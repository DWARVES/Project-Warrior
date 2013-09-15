
#ifndef DEF_GRAPHICS_TEXTURE
#define DEF_GRAPHICS_TEXTURE

#include <string>
#include <GL/glu.h>
#include <SDL.h>
#include "geometry/point.hpp"

namespace graphics
{
    namespace internal
    {
        class Texture
        {
            public:
                /* Memory managment */
                Texture();
                Texture(const Texture& cp) = delete;
                ~Texture();
                bool load(const std::string& path);
                SDL_Surface* preload(const std::string& path);
                bool loadsdl(SDL_Surface* src); /* src wont be free'd */
                bool loadgl(GLuint id); /* id mustn't be free'd by the user */

                /* Get infos about the texture */
                bool loaded() const;
                GLuint glID() const;
                int width() const;
                int height() const;

                void hotpoint(const geometry::Point& hp);
                geometry::Point hotpoint() const;

            private:
                bool m_loaded;
                GLuint m_id;
                geometry::Point m_hp;
                int m_w, m_h;
        };
    }
}

#endif

