
#ifndef DEF_GRAPHICS_EXTS
#define DEF_GRAPHICS_EXTS

#include <string>
#include <map>
#include <exception>
#include <sstream>
#include "core/logger.hpp"

namespace graphics
{
    namespace internal
    {
        /* Exception class for Extensions */
        class ext_not_loaded : public std::exception
        {
            public:
                ext_not_loaded() = delete;
                ext_not_loaded(const char* mname) throw();

                virtual const char* what() const throw();
                /* Returns the name of the openGL function which couldn't be loaded */
                const char* name() const throw();

            private:
                const char* m_name;
        };

        /* Manages OpenGL extensions */
        class Extensions
        {
            public:
                Extensions();
                Extensions(const Extensions&) = delete;

                /* Get the list of supported extensions, must be the first function called */
                bool loadList();
                /* Check if the hardware supports an extension */
                bool has(const std::string& name) const;
                /* Load a function from an extension */
                bool load(const std::string& name);
                bool isLoaded(const std::string& name) const;

                /* Returns a pointer to an extension function which must be loaded
                 * Will throw an ext_not_loaded exception if the function was not loaded
                 */
                template<typename T> T call(const std::string& name) const;

            private:
                const char* m_exts;
                std::map<std::string, void*> m_funcs;
        };
                
        template<typename FN> union PtrToFn {
            FN fn;
            void* ptr;
        };

        template<typename T> T Extensions::call(const std::string& name) const
        {
            if(!isLoaded(name)) {
                std::ostringstream oss;
                oss << "Tryed to call an OpenGL extension function which was not loaded : " << name;
                core::logger::logm(oss.str(), core::logger::ERROR);
                throw ext_not_loaded(name.c_str());
            }

            PtrToFn<T> tr;
            tr.ptr = m_funcs.at(name);
            return tr.fn;
        }
    }
}

#endif

