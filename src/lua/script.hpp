
#ifndef DEF_LUA_SCRIPT
#define DEF_LUA_SCRIPT

#include "lua/exception.hpp"
#include "core/logger.hpp"
#include <boost/lexical_cast.hpp>
#include <lua.hpp>
#include <string>
#include <sstream>
#include <typeinfo>

namespace lua
{
    class Script
    {
        public:
            /* Will throw an exception of type lua::exception if can't load lua context */
            Script();
            /* If path can't be loaded, it will just set loaded to false */
            Script(const std::string& path);
            Script(const Script&) = delete;
            ~Script();

            /* Load a script from it's path.
             * Only one script can be loaded at once.
             * Returns false if a script has already been loaded or if the script couldn't be loaded
             */
            bool load(const std::string& path);
            /* Check if the script has been correctly loaded */
            bool loaded() const;

            /* Check if the function exists in the script
             * If the script wasn't loaded, it always return false
             */
            bool existsFunction(const std::string& name);
            /* The first template is the return type of the function
             * It only handle lua function with one or zero return value
             * For no return, do NOT put void in the template, put any value (not important, as long it is not void), and set ret to false
             * Implicit templates types won't works if you use const char* for strings as arguments
             * Args are the types of the arguments
             * It only handle some types for the functions : std::string and any number type (but only number, undetermined behaviour if not, may launch boost::bad_lexical_cast).
             * It will throw an lua::exception if the lua function does not exists
             */
            template <typename Ret, typename... Args> Ret callFunction(const std::string& name, bool ret, Args... args);

            // TODO variable access function
            // TODO registering functions

        private:
            lua_State* m_state;
            bool m_loaded;

            /* Add an arg to a lua state */
            template<typename T, typename... Args> void addArg(T type, Args... args);
            /* Do nothing, just the end of the args */
            void addArg();
    };

    /* Implementation of templates methods */
    /* Use of boost::lexical_cast prevents compile-time errors */

    template <typename Ret, typename... Args> Ret Script::callFunction(const std::string& name, bool ret, Args... args)
    {
        /* Prepare function */
        lua_getglobal(m_state, name.c_str());
        if(!lua_isfunction(m_state, -1)) {
            std::ostringstream oss;
            oss << "Tryed to call an unexisting lua function : \"" << name << "\".";
            core::logger::logm(oss.str(), core::logger::WARNING);
            throw lua::exception("Tryed to call an unexisting lua function.");
        }

        /* Parse arguments */
        unsigned short int nbArgs = sizeof...(Args);
        if(nbArgs != 0)
            addArg(args...);

        /* Call the function */
        lua_call(m_state, nbArgs, ret);

        /* Get return value */
        if(ret) {
            if(typeid(Ret) == typeid(std::string)) {
                if(!lua_isstring(m_state, -1)) {
                    std::ostringstream oss;
                    oss << "Invalid return type for lua function : \"" << name << "\".";
                    core::logger::logm(oss.str(), core::logger::WARNING);
                    return boost::lexical_cast<Ret>("");
                }
                else
                    return boost::lexical_cast<Ret>(lua_tostring(m_state, -1));
            }
            else {
                if(!lua_isstring(m_state, -1)) {
                    std::ostringstream oss;
                    oss << "Invalid return type for lua function : \"" << name << "\".";
                    core::logger::logm(oss.str(), core::logger::WARNING);
                    return boost::lexical_cast<Ret>(0);
                }
                else
                    return boost::lexical_cast<Ret>( lua_tonumber(m_state, -1) );
            }
        }
        else
            return boost::lexical_cast<Ret>(0);
    }

    template<typename T, typename... Args> void Script::addArg(T type, Args... args)
    {
        if(typeid(T) == typeid(std::string))
            lua_pushstring(m_state, boost::lexical_cast<std::string>(type).c_str());
        else
            lua_pushnumber(m_state, boost::lexical_cast<double>(type));

        addArg(args...);
    }
}

#endif

