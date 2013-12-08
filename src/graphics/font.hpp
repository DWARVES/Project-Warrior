
#ifndef DEF_GRAPHICS_FONT
#define DEF_GRAPHICS_FONT

#include "graphics/shaders.hpp"
#include "graphics/texture.hpp"
#include "geometry/point.hpp"
#include "geometry/aabb.hpp"
#include <string>
#include <unordered_map>

namespace graphics
{
    namespace internal
    {
        class Font
        {
            public:
                Font() = delete;
                Font(const Font&) = delete;
                Font(Shaders* shads);
                ~Font();
                /* letters describe what letter is each part of the texture
                 * It's from left to right and up to bottom
                 */
                bool load(const std::string& path, const std::string& letters);

                /* Drawing */
                void draw(const std::string& str, const geometry::Point& pos, float size, bool smooth = true, bool invert = false);

                /* Information access */
                geometry::AABB stringSize(const std::string& str, float size) const;
                float widthLetter(unsigned int l) const;
                bool hasLetter(unsigned int l) const;
                bool isLoaded() const;

            private:
                struct Letter {
                    float w, h;
                    geometry::Point lt; /* Left-top of the letter in the texture */
                    geometry::Point rb; /* Right-bottom of the letter in the texture */
                };
                std::unordered_map<unsigned int, Letter> m_letters;
                float m_yspacing; /* Distance between lines */
                float m_xspacing; /* spaces width */
                Texture* m_text;
                Shaders* m_shads;

                /* Internal methods */
                Uint32 pixel(SDL_Surface* s, int x, int y);
                void pixel(SDL_Surface* s, int x, int y, Uint32 pix);
                void fitToChar(Letter* l, SDL_Surface* surf, Uint32 bg);
        };
    }
}

#endif

