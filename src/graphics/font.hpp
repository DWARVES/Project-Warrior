
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
        /** @brief Manages a font, used to render text. */
        class Font
        {
            public:
                Font() = delete;
                Font(const Font&) = delete;
                Font(Shaders* shads);
                ~Font();
                /** @brief Loads a font file, a personnalized filetype storing the picture and the symbols. */
                bool load(const std::string& path);

                /** @brief Draw a text.
                 * @param str The text to draw. It handles \n and any characters not handled by the font is replaced by a space.
                 * @param pos Were it must be drawn.
                 * @param size The height of a line, the font is scaled according to that.
                 * @param smooth If true, the font will be smoothed when drawn.
                 * @param invert If true, the drawn text will be flipped vertically.
                 */
                void draw(const std::string& str, const geometry::Point& pos, float size, bool smooth = true, bool invert = false);

                /* Information access */
                /** @brief Get the size of a text, setting a line height to size. */
                geometry::AABB stringSize(const std::string& str, float size) const;
                /** @brief Get the width of a letter (UTF number of the letter). */
                float widthLetter(unsigned int l) const;
                /** @brief Is the letter handled (UTF number of the letter). */
                bool hasLetter(unsigned int l) const;
                /** @brief Indicates if the font has been loaded. */
                bool isLoaded() const;

            private:
                /** @brief Represents a letter. */
                struct Letter {
                    float w;            /**< @brief The width of the letter in pixels. */
                    float h;            /**< @brief The height of the letter in pixels. */
                    geometry::Point lt; /**< @brief Left-top corner of the letter in the texture. */
                    geometry::Point rb; /**< @brief Right-bottom corner of the letter in the texture. */
                };
                std::unordered_map<unsigned int, Letter> m_letters; /**< @brief The map of all letters letters : symbol->Letter. */
                float m_yspacing; /** @brief Distance between lines. */
                float m_xspacing; /** @brief Space size. */
                Texture* m_text;  /** @brief The texture managing the font texture. */
                Shaders* m_shads; /** @brief The shaders, used to render text. */

                /* Internal methods */
                /** @brief Get the color of a pixel in an SDL_Surface. */
                Uint32 pixel(SDL_Surface* s, int x, int y);
                /** @brief Set the color of a pixel in an SDL_Surface. */
                void pixel(SDL_Surface* s, int x, int y, Uint32 pix);
                /** @brief Adapt the letter l to only fit the drawn letter. */
                void fitToChar(Letter* l, SDL_Surface* surf, Uint32 bg);
        };
    }
}

#endif

