
#include "graphics/shaders.hpp"
#include "core/logger.hpp"
#include <sstream>

namespace graphics
{
    namespace internal
    {
        /***************************
         *      Shaders src        *
         ***************************/
        /** @brief The fragment shader used by the program. */
        static const char* fragmentSrc =
            "uniform sampler2D tex;\n"
            "uniform float texture;\n"
            "void main(void) {\n"
            "    vec4 color;\n"
            "    if(texture > 0.0f) color = texture2D(tex, gl_TexCoord[0].st);\n"
            "    else color = gl_Color;\n"
            "    gl_FragColor = color;\n"
            "}";

        /** @brief The vertex shader used by the program. */
        static const char* vertexSrc =
            "uniform float texture;\n"
            "void main(void) {\n"
            "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
            "    gl_FrontColor = gl_Color;\n"
            "    if(texture > 0.0f) gl_TexCoord[0] = gl_MultiTexCoord0;\n"
            "}";

        /***************************
         *     Shaders class       *
         ***************************/

        Shaders::Shaders(Extensions* exts)
            : m_exts(exts), m_vertex(0), m_fragment(0), m_program(0),
            m_text(-1)
        {}

        Shaders::~Shaders()
        {
            if(m_vertex != 0)
                glDeleteShader(m_vertex);
            if(m_fragment != 0)
                glDeleteShader(m_fragment);
            if(m_program != 0)
                glDeleteProgram(m_program);
        }

        bool Shaders::checkAndLoadExtensions()
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

        void Shaders::logCompileError(GLuint shader)
        {
            GLint logsize;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logsize);
            if(logsize <= 0)
                return;

            char* log = new char [logsize];
            glGetShaderInfoLog(shader, logsize, &logsize, log);
            log[logsize - 1] = '\0';

            std::ostringstream oss;
            oss << "Error while compiling a shader : \"" << log << "\"";
            core::logger::logm(oss.str(), core::logger::MSG);
            delete[] log;
        }

        bool Shaders::load()
        {
            /* Creating the vertex shader */
            m_vertex = glCreateShader(GL_VERTEX_SHADER);
            if(glIsShader(m_vertex) != GL_TRUE) {
                core::logger::logm("Couldn't create vertex shader.", core::logger::WARNING);
                return false;
            }
            glShaderSource(m_vertex, 1, &vertexSrc, NULL);

            /* Compiling the vertex shader */
            glCompileShader(m_vertex);
            GLint result;
            glGetShaderiv(m_vertex, GL_COMPILE_STATUS, &result);
            if(result != GL_TRUE) {
                core::logger::logm("Couldn't compile vertex shader.", core::logger::WARNING);
                logCompileError(m_vertex);
                return false;
            }

            /* Creating the fragment shader */
            m_fragment = glCreateShader(GL_FRAGMENT_SHADER);
            if(glIsShader(m_fragment) != GL_TRUE) {
                core::logger::logm("Couldn't create fragment shader.", core::logger::WARNING);
                return false;
            }
            glShaderSource(m_fragment, 1, &fragmentSrc, NULL);

            /* Compiling the fragment shader */
            glCompileShader(m_fragment);
            glGetShaderiv(m_fragment, GL_COMPILE_STATUS, &result);
            if(result != GL_TRUE) {
                core::logger::logm("Couldn't compile fragment shader.", core::logger::WARNING);
                logCompileError(m_fragment);
                return false;
            }

            /* Creating the openGL program */
            m_program = glCreateProgram();
            if(m_program == 0) {
                core::logger::logm("Couldn't create the shader program.", core::logger::WARNING);
                return false;
            }

            /* Attaching the shaders to the program */
            glAttachShader(m_program, m_vertex);
            glAttachShader(m_program, m_fragment);

            /* Linking the program */
            glLinkProgram(m_program);
            glGetProgramiv(m_program, GL_LINK_STATUS, &result);
            if(result != GL_TRUE) {
                core::logger::logm("Couldn't link the shader program.", core::logger::WARNING);
                GLint logsize;
                glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logsize);
                if(logsize > 0) {
                    char* log = new char [logsize];
                    glGetProgramInfoLog(m_program, logsize, &logsize, log);
                    log[logsize - 1] = '\0';
                    std::ostringstream oss;
                    oss << "Error while linking a shader program \"" << log << "\"";
                    core::logger::logm(oss.str(), core::logger::MSG);
                    delete[] log;
                }
                return false;
            }
            glUseProgram(m_program);

            /* Getting the uniforms */
            if(!loadUniform(&m_text, "texture")) return false;

            /* Preparing the texture */
            GLint tex = -1;
            if(!loadUniform(&tex, "tex")) return false;
            glUniform1i(tex, 0);

            return true;
        }

        bool Shaders::loadUniform(GLint* id, const char* name)
        {
            *id = glGetUniformLocation(m_program, name);
            if(*id < 0) {
                std::ostringstream oss;
                oss << "Couldn't get \"" << name << "\" uniform from shader program : \"" << gluErrorString(glGetError()) << "\".";
                core::logger::logm(oss.str(), core::logger::WARNING);
                return false;
            }
            else
                return true;
        }

        void Shaders::enable(bool e)
        {
            if(e)
                glUseProgram(m_program);
            else
                glUseProgram(0);
        }

        void Shaders::text(bool t)
        {
            glUseProgram(m_program);
            if(t)
                glUniform1f(m_text, 1.0f);
            else
                glUniform1f(m_text, -1.0f);
        }
                
        Extensions* Shaders::exts() const
        {
            return m_exts;
        }

    }
}


