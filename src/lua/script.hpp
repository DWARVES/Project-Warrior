
#ifndef DEF_LUA_SCRIPT
#define DEF_LUA_SCRIPT

#include "lua/exception.hpp"
#include "luna.hpp"
#include "core/logger.hpp"
#include <lua.hpp>
#include <string>
#include <sstream>
#include <typeinfo>

/** @brief Contains all class and methods used to interface with Lua. */
namespace lua
{
    /** @brief A class representing a lua script, allow interaction with it. */
    class Script
    {
        public:
            /** @brief Initialise the class and load a lua context.
             *
             * It will throw an exception of type lua::program_exception if it couldn't load the lua context.
             */
            Script();
            /** @brief Init and directly load a script. See lua::Script::load for more details.
             *
             * It will throw a lua::program_exception if couldn't init, but simply set loaded to false if couldn't load the script.
             */
            explicit Script(const std::string& path);
            Script(const Script&) = delete;
            ~Script();

            /** @brief Load a script from it's path.
             *
             * Only one script can be loaded at once.
             * @return False if a script has already been loaded or if the script couldn't be loaded.
             */
            bool load(const std::string& path);
            /** @brief Indicates if a script has already been succesfully loaded. */
            bool loaded() const;

            /*************************************************
             *         Functions check and call              *
             *************************************************/

            /** @brief Check if the function exists in the script.
             *
             * If the script wasn't loaded, it will throw a lua::nonloaded_exception.
             */
            bool existsFunction(const std::string& name);
            /** @brief Call a lua function.
             *
             * It will throw an lua::functionaccess_exception if the lua function does not exists and a lua::nonloaded_exception if the script is not loaded
             * @tparam Ret The return type of the lua function (if ret = NULL, this value is unimportant).
             * @tparam Args The types of the arguments for the lua function.
             * It only handle some types for the functions : std::string (and const char*), bool and any number type. If you use another type, it will lead to an undefined behaviour.
             *
             * @param name The name of the lua function.
             * @param ret A pointer to where store the lua function return value. If NULL, the return of the lua function will be ignored.
             * It won't handle a lua function with more than one return value.
             * @param args The parameters of the lua function.
             */
            template <typename Ret, typename... Args> void callFunction(const std::string& name, Ret* ret, Args... args);

            /*************************************************
             *             Variables access                  *
             *************************************************/

            /** @brief Represents the type of a lua variable. */
            enum VarType {
                NUMBER,   /**< @brief Any type of number : there is no distinction between int, double, float ... in lua. */
                STRING,   /**< @brief A simple string. */
                BOOL,     /**< @brief Boolean value. */
                TABLE,    /**< @brief A lua table. */
                USER,     /**< @brief An userdata variable. */
                FUNCTION, /**< @brief A function, which is also a variable in lua. */
                NIL,      /**< @brief Nil value : the variable doesn't exist, it has been declared as nil or it's a type not declared above. */
            };
            /** @brief Return the type of a lua variable.
             *
             * It will throw a lua::nonloaded_exception if the script has not been loaded.
             * @param name The name of the lua variable.
             */
            VarType typeVariable(const std::string& name);
            /** @brief Get the value of a lua number variable.
             *
             * It will throw a lua::nonloaded_exception if the script is not loaded
             * It will throw a lua::vartype_exception if name refers to a variable which is not a number.
             * @tparam T The type of the lua variable.
             * It must be a number type (double is conseilled because the lua native number type).
             * If T is another type, it may leads to an undefined behaviour.
             * @param name The name of the lua variable.
             * @param val The c++ variable where the value is stored.
             */
            template <typename T> void getVariable(const std::string& name, T& val);
            /** @brief Get the value of a lua string variable. It works as lua::Script::getVariable<T>, excepts the lua variable must be a string and not a number. */
            void getVariable(const std::string& name, std::string& val);
            /** @brief Get the value of a lua boolean variable. It works as lua::Script::getVariable<T>, excepts the lua variable must be a boolean and not a number. */
            void getVariable(const std::string& name, bool& val);
            /** @brief Change the value of a lua variable to a string.
             * If the variable didn't existed before, it will create it.
             * 
             * If the script isn't loaded, it will throw a lua::nonloaded_exception.
             */
            void setVariable(const std::string& name, const std::string& str);
            /** @brief Change the value of a lua variable to a string.
             * If the variable didn't existed before, it will create it.
             * 
             * If the script isn't loaded, it will throw a lua::nonloaded_exception.
             */
            void setVariable(const std::string& name, const char* str);
            /** @brief Change the value of a lua variable to a number.
             * If the variable didn't existed before, it will create it.
             * 
             * If the script isn't loaded, it will throw a lua::nonloaded_exception.
             */
            void setVariable(const std::string& name, double number);

            /*************************************************
             *      Functions and classes registering        *
             *************************************************/

            /** @brief Defines the pointer to a c++ function which can be exposed to lua. */
            typedef int (*luaFnPtr)(lua_State* st);
            /** @brief Expose a c++ function to lua.
             * It can be called even if the script is not loaded, but if the initialization failed, it will throw an lua::nonloaded_exception
             * @param fn A pointer to the c++ function.
             * It must takes its argument from the lua stack and put its returns onto the lua stack.
             * It must returns the number of returns it has.
             * @param name The name of function in lua.
             */
            void registerFunction(luaFnPtr fn, const std::string& name);
            /** @brief Hide luna usage to the user (see internal::Luna::FunctionType for details on functionnement). */
            template<typename T> using Methods = typename internal::Luna<T>::FunctionType;
            /** @brief Hide luna usage to the user (see internal::Luna::PropertyType for details on functionnement). */
            template<typename T> using Properties = typename internal::Luna<T>::PropertyType;
            /** @brief Expose a class to lua.
             * It will work even if the script is not loaded, but it will throw a lua::nonloaded_exception if the script is not initialized.
             *
             * @tparam T The class to expose.
             * This class must contain some members :
             *  - static const char* className -> the name of the class in lua.
             *  - static const Script::Methods<T> methods[] -> the members, which must have the same signature as a function (see lua::Script::registerFunction), must end by {NULL, NULL} : example {"MyCoolFunctionName", &Foo::function}.
             *  - static const Script::Properties<T> properties[] -> the variables, must be defined by a setter and a getter, must end by {NULL, NULL, NULL} : example {"MemberName", &Foo::getter, &Foo::Setter}.
             *  - bool isExisting -> for internal use, do not modify nor set it
             *  - bool isPrecious -> tell lua not to garbage collect the object, must be set on the constructor
             */
            template<typename T> void registerClass();


        private:
            lua_State* m_state; /**< @brief The lua state, representing and controlling the script and its execution. */
            bool m_loaded;      /**< @brief Indicates if the script has successfully been loaded. */

            /** @brief Used to recursively parse variadic template of lua::Script::callFunction : add a number to the lua stack. */
            template<typename T, typename... Args> void addArgs(T type, Args... args);
            /** @brief Used to recursively parse variadic template of lua::Script::callFunction : add a string to the lua stack. */
            template<typename... Args> void addArgs(const std::string& str, Args... args);
            /** @brief Used to recursively parse variadic template of lua::Script::callFunction : add a string (as const char*) to the lua stack. */
            template<typename... Args> void addArgs(const char* str, Args... args);
            /** @brief Used to recursively parse variadic template of lua::Script::callFunction : add a boolean to the lua stack. */
            template<typename... Args> void addArgs(bool b, Args... args);
            /** @brief Do nothing, it ends the recursion over the variadic tempaltes of lua::Script::callFunction. */
            void addArgs();
            /** @brief Get the number return of a lua function in lua::Script::callFunction. */
            template<typename T> void getRet(T* r);
            /** @brief Get the string return of a lua function in lua::Script::callFunction. */
            void getRet(std::string* r);
            /** @brief Get the boolean return of a lua function in lua::Script::callFunction. */
            void getRet(bool* r);
            /** @brief Do nothing, prevents error when using void for the return type of a function in lua::Script::callFunction. */
            void getRet(void* v);

            /** @brief Put a lua variable/function to the top of the stack.
             * Throw a lua::nonloaded_exception if the script is not loaded.
             */
            void gettop(const std::string& name);

            /** @brief Return the name of a lua type. */
            const char* nameType(VarType t) const;
            /** @brief Return the type of the variable on the top of the stack. */
            VarType typeTop();
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
            lua_pop(m_state, 1);
        }
        else {
            std::ostringstream oss;
            oss << "Tryed to get " << name << " lua variable as a " << typeid(T).name() << ", which is not its type.";
            core::logger::logm(oss.str(), core::logger::WARNING);
            const char* type = nameType(typeTop());
            lua_pop(m_state, 1);
            throw lua::vartype_exception(typeid(T).name(), type, name.c_str());
        }
    }
    
    template<typename T> void Script::registerClass()
    {
        if(!m_state)
            throw lua::nonloaded_exception();
        internal::Luna<T>::Register(m_state, NULL);
    }
}

#endif

