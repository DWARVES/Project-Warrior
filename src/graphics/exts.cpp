
#include "graphics/exts.hpp"
#include <GL/glu.h>
#include <SDL.h>
#include <cstring>

namespace graphics
{
    namespace internal
    {
        ext_not_loaded::ext_not_loaded(const char* mname) throw()
        {
            m_name = mname;
        }

        const char* ext_not_loaded::what() const throw()
        {
            return "Tryed to use an OpenGL extension function which was not loaded.";
        }

        const char* ext_not_loaded::name() const throw()
        {
            return m_name;
        }


        Extensions::Extensions()
            : m_exts((const char*)glGetString(GL_EXTENSIONS))
        {}

        bool Extensions::has(const std::string& name) const
        {
            return strstr(m_exts, name.c_str()) != NULL;
        }

        bool Extensions::load(const std::string& name)
        {
            /* The function was already loaded, so no need to load it again */
            if(isLoaded(name))
                return true;
            m_funcs[name] = SDL_GL_GetProcAddress(name.c_str());
            return true;
        }

        bool Extensions::isLoaded(const std::string& name) const
        {
            return m_funcs.find(name) != m_funcs.end();
        }
                
        void* Extensions::operator()(const std::string& name) const
        {
            if(!isLoaded(name)) {
                std::ostringstream oss;
                oss << "Tryed to call an OpenGL extension function which was not loaded : " << name;
                core::logger::logm(oss.str(), core::logger::ERROR);
                throw ext_not_loaded(name.c_str());
            }

            return m_funcs.at(name);
        }

    }
}


