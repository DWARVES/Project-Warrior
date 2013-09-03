
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
            explicit Script(const std::string& path);
            Script(const Script&) = delete;
            ~Script();

            /* Load a script from it's path.
             * Only one script can be loaded at once.
             * Returns false if a script has already been loaded or if the script couldn't be loaded
             */
            bool load(const std::string& path);
            /* Check if the script has been correctly loaded */
            bool loaded() const;

            /*************************************************
             *         Functions check and call              *
             *************************************************/

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
             * It will throw an lua::exception if the lua function does not exists or if the script is not loaded
             */
            template <typename Ret, typename... Args> Ret callFunction(const std::string& name, bool ret, Args... args);

            /*************************************************
             *             Variables access                  *
             *************************************************/

            /* Return the type of the variable
             * Always return none if the script is not loaded
             */
            enum VarType {
                NUMBER, /* Any type of number : there is no distinction between int, double, float ... in lua */
                STRING, /* A simple string */
                BOOL,   /* Boolean value */
                TABLE,  /* A lua table */
                USER,   /* An userdata variable */
                NIL,    /* Nil value : the variable doesn't exist or it has been declared as nil */
            };
            VarType typeVariable(const std::string& name);
            /* Returns a variable value.
             * T must be either std::string, bool or a number type (undefined behaviour if it is not, double is conseilled : native lua number type)
             * Can't return neither a table, an userdata nor a nil variale
             * It will throw an exception of type lua::exception if name refers to a variable like the one mentionned the line above
             * It will also throw an exception if the script is not loaded
             */
            template <typename T> T getVariable(const std::string& name);
            /* If the script is not loaded, it will throw an exception of type lua::exception
             * If the variable didn't existed before, it will create it.
             */
            void setVariable(const std::string& name, const std::string& str);
            void setVariable(const std::string& name, const char* str);
            void setVariable(const std::string& name, double number);

            // TODO registering functions and classes

        private:
            lua_State* m_state;
            bool m_loaded;

            /* Used to recursively parse variadic template */
            template<typename T, typename... Args> void addArgs(T type, Args... args);
            /* Do nothing, just the end of the args */
            void addArgs();
            /* Used to add a string argument */
            void addArg(const std::string& str);
            /* Used to add a number */
            void addArg(double number);
    };

    /******************************************
     *  Implementation of templates methods   *
     ******************************************/

    /* Use of boost::lexical_cast prevents compile-time errors */

    template <typename Ret, typename... Args> Ret Script::callFunction(const std::string& name, bool ret, Args... args)
    {
        if(!loaded())
            throw lua::exception("Tryed to execute a function from a non loaded script.");

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
            addArgs(args...);

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

    template<typename T, typename... Args> void Script::addArgs(T type, Args... args)
    {
        addArg(type); /* Use overload to determine the right treatment */
        addArgs(args...);
    }
            
    template <typename T> T Script::getVariable(const std::string& name)
    {
        if(!loaded()) {
            core::logger::logm("Tryed to get a variable from a non loaded script", core::logger::WARNING);
            throw lua::exception("Tryed to use a non loaded script");
        }

        lua_settop(m_state, 0);
        lua_getglobal(m_state, name.c_str());

        if(typeid(T) == typeid(std::string)) {
            if(lua_isstring(m_state, 1)) {
                return boost::lexical_cast<T>( lua_tostring(m_state, 1) );
            }
            else {
                std::ostringstream oss;
                oss << "Tryed to get " << name << " lua variable as string, which is not its type.";
                core::logger::logm(oss.str(), core::logger::WARNING);
                throw lua::exception("Invalid type when reading variable form lua script");
            }
        }
        else if(typeid(T) == typeid(bool)) {
            if(lua_isboolean(m_state, 1)) {
                return boost::lexical_cast<T>( lua_toboolean(m_state, 1) );
            }
            else {
                std::ostringstream oss;
                oss << "Tryed to get " << name << " lua variable as boolean, which is not its type.";
                core::logger::logm(oss.str(), core::logger::WARNING);
                throw lua::exception("Invalid type when reading variable form lua script");
            }
        }
        else { /* Let's assume the type is a number */
            if(lua_isnumber(m_state, 1)) {
                return boost::lexical_cast<T>( lua_tonumber(m_state, 1) );
            }
            else {
                std::ostringstream oss;
                oss << "Tryed to get " << name << " lua variable as a " << typeid(T).name() << ", which is not its type.";
                core::logger::logm(oss.str(), core::logger::WARNING);
                throw lua::exception("Invalid type when reading variable form lua script");
            }
        }
    }
}

#endif

