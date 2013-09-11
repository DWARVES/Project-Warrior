
#ifndef DEF_GRAPHICS_FONT
#define DEF_GRAPHICS_FONT

#include <string>

namespace graphics
{
    namespace internal
    {
        class Font
        {
            public:
                Font();
                ~Font();
                bool load(const std::string& path);

                /* TODO */
        };
    }
}

#endif

