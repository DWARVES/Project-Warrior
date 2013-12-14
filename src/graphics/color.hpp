
#ifndef DEF_GRAPHICS_COLOR
#define DEF_GRAPHICS_COLOR

#include <SDL.h>

namespace graphics
{
    /** @brief Describes a color. */
    class Color
    {
        public:
            Color();
            Color(Uint8 mr, Uint8 mg, Uint8 mb, Uint8 ma = 255);

            /** @brief Sets the color from its (r;g;b;a) componants, in range 0-255. */
            void set(Uint8 mr, Uint8 mg, Uint8 mb, Uint8 ma = 255);
            /** @brief Sets the color from its (r;g;b;a) componants, in range 0-1. */
            void set(float mr, float mg, float mb, float ma = 1.0f);
            /** @brief Sets the color from the SDL representation of a pixel. */
            void set(Uint32 pix, SDL_PixelFormat* fmt, bool alpha = false);

            /** @brief Get the red componant of the color. */
            float fr() const;
            /** @brief Get the green componant of the color. */
            float fg() const;
            /** @brief Get the blue componant of the color. */
            float fb() const;
            /** @brief Get the alpha componant of the color. */
            float fa() const;
            /** @brief Returns the SDL representation of this color. */
            Uint32 pixel(SDL_PixelFormat* fmt, bool alpha = false) const;

            /** @name The color components.
             * @{
             */
            Uint8 r, g, b, a;
            /** @} */
    };
}

#endif

