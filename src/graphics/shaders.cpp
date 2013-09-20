
#include "graphics/shaders.hpp"
#include "core/logger.hpp"
#include <sstream>
#include <iostream>

/* OpenGL extensions */
#define GLEXT(type, name) (m_exts->call<type>(#name))
#define glCreateShader(type) GLEXT(PFNGLCREATESHADERPROC, glCreateShaderARB)(type)
#define glDeleteShader(shad) GLEXT(PFNGLDELETESHADERPROC, glDeleteShaderARB)(shad)
#define glIsShader(shad)     GLEXT(PFNGLISSHADERPROC,     glIsShaderARB)(shad)

namespace graphics
{
    namespace internal
    {
        Shaders::Shaders(Extensions* exts)
            : m_exts(exts), m_vertex(0), m_fragment(0)
        {}

        Shaders::~Shaders()
        {
            if(m_vertex != 0)
                glDeleteShader(m_vertex);
            if(m_fragment != 0)
                glDeleteShader(m_fragment);
        }

        bool Shaders::checkExtensions() const
        {
            if(!m_exts->has("GL_ARB_shading_language_100")) {
                core::logger::logm("Hardware does not support GL_ARB_shading_language_100, needed for shaders.", core::logger::WARNING);
                return false;
            }
            if(!m_exts->has("GL_ARB_shader_objects")) {
                core::logger::logm("Hardware does not support GL_ARB_shader_objects, needed for shaders.", core::logger::WARNING);
                return false;
            }
            if(!m_exts->has("GL_ARB_vertex_shader")) {
                core::logger::logm("Hardware does not support GL_ARB_vertex_shader, needed for shaders.", core::logger::WARNING);
                return false;
            }
            if(!m_exts->has("GL_ARB_fragment_shader")) {
                core::logger::logm("Hardware does not support GL_ARB_fragment_shader, needed for shaders.", core::logger::WARNING);
                return false;
            }
            return true;
        }

        bool Shaders::loadExtensions()
        {
            bool ret = true;
            ret &= m_exts->load("glCreateShaderARB");
            ret &= m_exts->load("glDeleteShaderARB");
            ret &= m_exts->load("glIsShaderARB");

            return ret;
        }
                
        bool Shaders::load()
        {
            m_vertex = glCreateShader(GL_VERTEX_SHADER);
            m_fragment = glCreateShader(GL_FRAGMENT_SHADER);

            if(glIsShader(m_vertex) != GL_TRUE) {
                std::cout << "Vertex couldn't be loaded." << std::endl;
                return false;
            }
            else if(!glIsShader(m_vertex) != GL_TRUE) {
                std::cout << "Fragment couldn't be loaded." << std::endl;
                return false;
            }
            else
                return true;
            /* TODO end */
        }
    }
}


