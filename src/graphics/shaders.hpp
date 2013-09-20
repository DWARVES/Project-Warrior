
#ifndef DEF_GRAPHICS_SHADERS
#define DEF_GRAPHICS_SHADERS

#include "graphics/exts.hpp"
#include <GL/glu.h>

namespace graphics
{
    namespace internal
    {
        class Shaders
        {
            public:
                Shaders(Extensions* exts);
                Shaders() = delete;
                Shaders(const Shaders&) = delete;
                ~Shaders();

                /* Check if the right extensions are available on the hardware */
                bool checkExtensions() const;
                /* Assume the extensions are available (must be checked by checkExtensions */
                bool loadExtensions();

                /* Load the shaders, the extensions must have already been loaded */
                bool load();

            private:
                Extensions* m_exts;
                GLuint m_vertex;
                GLuint m_fragment;
        };
    }
}

#endif
