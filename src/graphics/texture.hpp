
#ifndef DEF_GRAPHICS_TEXTURE
#define DEF_GRAPHICS_TEXTURE

#include <string>
#include <GL/glu.h>
#include <SDL.h>
#include "geometry/point.hpp"

namespace graphics
{
    /** @brief Classes and methods internally used by graphics::Graphics. */
    namespace internal
    {
        /** @brief Manages a graphical texture. */
        class Texture
        {
            public:
                Texture();
                Texture(const Texture& cp) = delete;
                ~Texture();
                /** @brief Loads the texture from a file. */
                bool load(const std::string& path);
                /** @brief Loads the texture to an SDL_Surface from a path. */
                SDL_Surface* preload(const std::string& path);
                /** @brief Loads the texture to an SDL_Surface from a SDL_RWops. */
                SDL_Surface* preload(SDL_RWops* rw, bool freerw = false);
                /** @brief Loads the texture from an SDL_Surface. src won't be free'd. */
                bool loadsdl(SDL_Surface* src);
                /** @brief Loads the texture from an opengl texture. id mustn't be free'd by the user. */
                bool loadgl(GLuint id, int w, int h);

                /** @brief Indicates if the texture has been loaded. */
                bool loaded() const;
                /** @brief Returns the id of the corresponding openGL texture. */
                GLuint glID() const;
                /** @brief Returns the width in pixels of the texture. */
                int width() const;
                /** @brief Returns the height in pixels of the texture. */
                int height() const;

                /** @brief Sets the hotpoint of the texture. */
                void hotpoint(const geometry::Point& hp);
                /** @brief Get the hotpoint of the texture. */
                geometry::Point hotpoint() const;

            private:
                bool m_loaded;        /**< @brief Indicates if the texture is loaded. */
                GLuint m_id;          /**< @brief The id of the opengl texture. */
                geometry::Point m_hp; /**< @brief The hotpoint. */
                int m_w;              /**< @brief The width in pixels. */
                int m_h;              /**< @brief The height in pixels. */
        };
    }
}

#endif

