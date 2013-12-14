
#ifndef DEF_GRAPHICS_EXTS
#define DEF_GRAPHICS_EXTS

#include <string>
#include <GL/glew.h>
#include <GL/glu.h>

namespace graphics
{
    namespace internal
    {
        /** @brief Manages OpenGL extensions. */
        class Extensions
        {
            public:
                Extensions();
                Extensions(const Extensions&) = delete;

                /** @brief Get the list of supported extensions, must be the first function called. */
                bool init();
                /** @brief Checks if the hardware supports an extension. */
                bool has(const std::string& name) const;
        };
    }
}

#endif

