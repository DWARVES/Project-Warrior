
#ifndef DEF_GRAPHICS_COLOR
#define DEF_GRAPHICS_COLOR

#include <SDL.h>

namespace graphics
{
    class Color
    {
        public:
            Color();
            Color(Uint8 mr, Uint8 mg, Uint8 mb, Uint8 ma = 255);

            void set(Uint8 mr, Uint8 mg, Uint8 mb, Uint8 ma = 255);
            void set(float mr, float mg, float mb, float ma = 1.0f);
            void set(Uint32 pix, SDL_PixelFormat* fmt, bool alpha = false);

            float fr() const;
            float fg() const;
            float fb() const;
            float fa() const;
            Uint32 pixel(SDL_PixelFormat* fmt, bool alpha = false) const;

            Uint8 r, g, b, a;
    };
}

#endif

