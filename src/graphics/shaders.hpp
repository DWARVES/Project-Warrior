
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

                /* Check if the right extensions are available on the hardware and load them */
                bool checkAndLoadExtensions();
                /* Load the shaders, the extensions must have already been loaded */
                bool load();
                /* Enable/disable the shader for the rendering */
                void enable(bool e = true);
                /* Use texture in rendering ? */
                void text(bool t = true);

            private:
                Extensions* m_exts;
                GLuint m_vertex;
                GLuint m_fragment;
                GLuint m_program;

                /* Uniforms */
                GLint m_text;

                /* Internal methods */
                void logCompileError(GLuint shader);
                bool loadUniform(GLint* id, const char* name);
        };
    }
}

#endif

