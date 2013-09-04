
#ifndef DEF_LUA_HELPER
#define DEF_LUA_HELPER

#include "lua/script.hpp"
#include <vector>

namespace lua
{
    /* Define functions that helps and hide low-level lua usage in C functions exposed to lua */
    namespace helper
    {
        /* Return the list of the arguments passed to the function by lua */
        std::vector<Script::VarType> listArguments(lua_State* st);

        /* Use one of thhe nexts function to return a value in your function
         * Just call it on the return instruction
         */
        int returnString(lua_State* st, const std::string& str);
        int returnNumber(lua_State* st, double d);
        int returnBoolean(lua_State* st, bool b);
    }
}

#endif

