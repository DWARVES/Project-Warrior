
#ifndef DEF_LUA_HELPER
#define DEF_LUA_HELPER

#include "lua/script.hpp"
#include <vector>

namespace lua
{
    /** @brief Define functions that helps and hide low-level lua usage in C functions exposed to lua. */
    namespace helper
    {
        /** @brief Return the list of the arguments passed to the function by lua. */
        std::vector<Script::VarType> listArguments(lua_State* st);

        /** @brief Use this function to return a string in lua from your c function.
         * @return A number which must be returned by your function.
         */
        int returnString(lua_State* st, const std::string& str);
        /** @brief Use this function to return a number in lua from your c function.
         * @return A number which must be returned by your function.
         */
        int returnNumber(lua_State* st, double d);
        /** @brief Use this function to return a boolean in lua from your c function.
         * @return A number which must be returned by your function.
         */
        int returnBoolean(lua_State* st, bool b);
    }
}

#endif

