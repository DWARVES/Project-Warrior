
#ifndef DEF_GRAPHICS_MOVIE
#define DEF_GRAPHICS_MOVIE

#include <string>

namespace graphics
{
    namespace internal
    {
        class Movie
        {
            public:
                Movie();
                ~Movie();
                bool load(const std::string& path);

                /* TODO */
        };
    }
}

#endif

