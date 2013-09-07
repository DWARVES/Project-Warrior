
#ifndef DEF_LUA_SCRIPT
#define DEF_LUA_SCRIPT

#include "lua/exception.hpp"
#include "luna.hpp"
#include "core/logger.hpp"
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
             * If the script wasn't loaded, it will throw an lua::nonloaded_exception
             */
            bool existsFunction(const std::string& name);
            /* The first template is the return type of the function
             * It only handle lua function with one or zero return value
             * If you don't want to get any result, just set ret to NULL
             * Implicit templates types won't works if you use const char* for strings as arguments
             * Args are the types of the arguments
             * It only handle some types for the functions : std::string (and const char*), bool and any number type (but only number, undetermined behaviour if not).
             * It will throw an lua::functionaccess_exception if the lua function does not exists and a lua::nonloaded_exception if the script is not loaded
             */
            template <typename Ret, typename... Args> void callFunction(const std::string& name, Ret* ret, Args... args);

            /*************************************************
             *             Variables access                  *
             *************************************************/

            /* Return the type of the variable
             * It will throw a lua::nonloaded_exception if the script is not loaded
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
            /* Store the value of a lua variable in a C variable.
             * It can store either std::string, bool or a number type (undefined behaviour if it is not, double is conseilled : native lua number type)
             * Can't return neither a table, an userdata nor a nil variale
             * It will throw an exception of type lua::vartype_exception if name refers to a variable like the one mentionned the line above
             * It will also throw an exception if the type of the dest variable in C is not the same as the lua variable
             * Finally, it will throw a lua::nonloaded_exception if the script is not loaded
             */
            template <typename T> void getVariable(const std::string& name, T& val);
            void getVariable(const std::string& name, std::string& val);
            void getVariable(const std::string& name, bool& val);
            /* If the script is not loaded, it will throw an exception of type lua::exception
             * If the variable didn't existed before, it will create it.
             */
            void setVariable(const std::string& name, const std::string& str);
            void setVariable(const std::string& name, const char* str);
            void setVariable(const std::string& name, double number);

            /*************************************************
             *      Functions and classes registering        *
             *************************************************/

            /* The function must return how many returns it has in lua
             * The arguments must be taken from the lua stack
             * name is the name the function will have in the lua script
             * It can be called even if the script is not loaded, but if the initialization failed, it will throw an lua::nonloaded_exception
             */
            typedef int (*luaFnPtr)(lua_State* st);
            void registerFunction(luaFnPtr fn, const std::string& name);
            /* Will register a class (passed as template parameter)
             * This class must contain some static members :
             *      const char* className -> the name of the class in lua
             *      const Luna<T>::FunctionType methods[] -> the members, which must have the same signature as a function (see registerFunction), must end by {NULL, NULL} : example {"MyCoolFunctionName", &Foo::function}
             *      const Luna<T>::PropertyType properties[] -> the variables, must be defined by a setter and a getter, must end by {NULL, NULL, NULL} : example {"MemberName", &Foo::getter, &Foo::Setter}
             * The class must also contains the following two members
             *      bool isExisting -> for internal use, do not modify nor set it
             *      bool isPrecious -> tell lua not to garbage collect the object, must be set on the constructor
             * It will work even if the script is not loaded, but it will throw a lua::nonloaded_exception if the script is not initialized.
             */
            template<typename T> void registerClass();


        private:
            lua_State* m_state;
            bool m_loaded;

            /* Used to recursively parse variadic template */
            template<typename T, typename... Args> void addArgs(T type, Args... args);
            template<typename... Args> void addArgs(const std::string& str, Args... args);
            template<typename... Args> void addArgs(const char* str, Args... args);
            template<typename... Args> void addArgs(bool b, Args... args);
            /* Get the return of a function */
            template<typename T> void getRet(T* r);
            void getRet(std::string* r);
            void getRet(bool* r);
            void getRet(void* v);
            /* Do nothing, just the end of the args */
            void addArgs();

            /* Put a lua variable to the top of the stack
             * Throw a lua::nonloaded_exception if the script is not loaded
             */
            void gettop(const std::string& name);
    };

    /******************************************
     *  Implementation of templates methods   *
     ******************************************/

    template <typename Ret, typename... Args> void Script::callFunction(const std::string& name, Ret* ret, Args... args)
    {
        if(!loaded())
            throw lua::nonloaded_exception();

        /* Prepare function */
        lua_getglobal(m_state, name.c_str());
        if(!lua_isfunction(m_state, -1)) {
            std::ostringstream oss;
            oss << "Tryed to call an unexisting lua function : \"" << name << "\".";
            core::logger::logm(oss.str(), core::logger::WARNING);
            throw lua::functionaccess_exception(name.c_str());
        }

        /* Parse arguments */
        unsigned short int nbArgs = sizeof...(Args);
        if(nbArgs != 0)
            addArgs(args...);

        /* Call the function */
        lua_call(m_state, nbArgs, (ret?1:0));

        /* Get return value */
        if(ret) {
            getRet(ret);
        }
    }

    template<typename T, typename... Args> void Script::addArgs(T type, Args... args)
    {
        lua_pushnumber(m_state, static_cast<double>(type));
        addArgs(args...);
    }

    template<typename... Args> void Script::addArgs(const std::string& str, Args... args)
    {
        lua_pushstring(m_state, str.c_str());
        addArgs(args...);
    }

    template<typename... Args> void Script::addArgs(const char* str, Args... args)
    {
        lua_pushstring(m_state, str);
        addArgs(args...);
    }

    template<typename... Args> void Script::addArgs(bool b, Args... args)
    {
        lua_pushboolean(m_state, static_cast<int>(b));
        addArgs(args...);
    }

    template<typename T> void Script::getRet(T* r)
    {
        *r = static_cast<T>(lua_tonumber(m_state, -1));
    }

    template <typename T> void Script::getVariable(const std::string& name, T& val)
    {
        gettop(name);
        if(lua_isnumber(m_state, 1)) {
            val = static_cast<T>( lua_tonumber(m_state, 1) );
        }
        else {
            std::ostringstream oss;
            oss << "Tryed to get " << name << " lua variable as a " << typeid(T).name() << ", which is not its type.";
            core::logger::logm(oss.str(), core::logger::WARNING);
            throw lua::vartype_exception(typeid(T).name(), "unknown", name.c_str());
        }
    }
    
    template<typename T> void Script::registerClass()
    {
        if(!m_state)
            throw lua::nonloaded_exception();
        Luna<T>::Register(m_state, NULL);
    }
}

#endif

