
#ifndef DEF_GRAPHICS_SHADERS
#define DEF_GRAPHICS_SHADERS

#include "graphics/exts.hpp"
#include <GL/glu.h>

namespace graphics
{
    namespace internal
    {
        /** @brief Manages the shaders of the program. */
        class Shaders
        {
            public:
                Shaders(Extensions* exts);
                Shaders() = delete;
                Shaders(const Shaders&) = delete;
                ~Shaders();

                /** @brief Check if the right extensions are available on the hardware and load them. */
                bool checkAndLoadExtensions();
                /** @brief Load the shaders, checkAndLoadExtensions must have already been called. */
                bool load();
                /** @brief Enable/disable the shader for the rendering. */
                void enable(bool e = true);
                /** @brief Enable/disable texture rendering in the shaders. */
                void text(bool t = true);
                /** @brief Get the extensions used by the shaders. */
                Extensions* exts() const;

            private:
                Extensions* m_exts; /**< @brief Used to manage the extensions. */
                GLuint m_vertex;    /**< @brief The glID of the vertex shader. */
                GLuint m_fragment;  /**< @brief The glID of the fragment shader. */
                GLuint m_program;   /**< @brief The glID of the program shader. */
                GLint m_text;       /**< @brief The glID of the uniform boolean enabling/disabling texture rendering in shaders. */

                /* Internal methods */
                /** @brief Prints to the logger the compilation errors of a shader (if any). */
                void logCompileError(GLuint shader);
                /** @brief Load the uniform name and store it in id. Return false if an error happened. */
                bool loadUniform(GLint* id, const char* name);
        };
    }
}

#endif

