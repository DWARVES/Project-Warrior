
#include "graphics/exts.hpp"
#include "core/logger.hpp"
#include <SDL.h>
#include <sstream>

namespace graphics
{
    namespace internal
    {
        Extensions::Extensions()
        {}
                
        bool Extensions::init()
        {
            GLenum err = glewInit();
            if(err != GLEW_OK) {
                std::ostringstream oss;
                oss << "Coudn't init GLEW for openGL extension usage : \"" << glewGetErrorString(err) << "\"";
                core::logger::logm(oss.str(), core::logger::WARNING);
                return false;
            }
            else
                return true;
        }

        bool Extensions::has(const std::string& name) const
        {
            return glewIsSupported(name.c_str());
        }
    }
}


