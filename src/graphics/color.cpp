
#include "graphics/color.hpp"

namespace graphics
{
    Color::Color()
        : r(0), g(0), b(0), a(0)
    {}

    void Color::set(Uint8 mr, Uint8 mg, Uint8 mb, Uint8 ma)
    {
        r = mr;
        g = mg;
        b = mb;
        a = ma;
    }

    void Color::set(float mr, float mg, float mb, float ma)
    {
        r = static_cast<Uint8>(mr * 255.0f);
        g = static_cast<Uint8>(mg * 255.0f);
        b = static_cast<Uint8>(mb * 255.0f);
        a = static_cast<Uint8>(ma * 255.0f);
    }

    void Color::set(Uint32 pix, SDL_PixelFormat* fmt, bool alpha)
    {
        if(alpha)
            SDL_GetRGBA(pix, fmt, &r, &g, &b, &a);
        else {
            SDL_GetRGB(pix, fmt, &r, &g, &b);
            a = 0;
        }
    }

    float Color::fr() const
    {
        return float(r)/255.0f;
    }

    float Color::fg() const
    {
        return float(g)/255.0f;
    }

    float Color::fb() const
    {
        return float(b)/255.0f;
    }

    float Color::fa() const
    {
        return float(a)/255.0f;
    }

    Uint32 Color::pixel(SDL_PixelFormat* fmt, bool alpha) const
    {
        if(alpha)
            return SDL_MapRGBA(fmt, r, g, b, a);
        else
            return SDL_MapRGB(fmt, r, g, b);
    }

}

