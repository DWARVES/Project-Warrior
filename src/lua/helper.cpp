
#include "lua/helper.hpp"

namespace lua
{
    namespace helper
    {
        std::vector<Script::VarType> listArguments(lua_State* st)
        {
            int nb = lua_gettop(st);
            std::vector<Script::VarType> ret(nb);

            for(int i = 0; i < nb; ++i) {
                if(lua_isnumber(st, i+1))
                    ret[i] = Script::NUMBER;
                else if(lua_isstring(st, i+1))
                    ret[i] = Script::STRING;
                else if(lua_isboolean(st, i+1))
                    ret[i] = Script::BOOL;
                else if(lua_istable(st, i+1))
                    ret[i] = Script::TABLE;
                else if(lua_isuserdata(st, i+1))
                    ret[i] = Script::USER;
                else
                    ret[i] = Script::NIL;
            }

            return ret;
        }

        int returnString(lua_State* st, const std::string& str)
        {
            lua_pushstring(st, str.c_str());
            return 1;
        }

        int returnNumber(lua_State* st, double d)
        {
            lua_pushnumber(st, d);
            return 1;
        }

        int returnBoolean(lua_State* st, bool b)
        {
            lua_pushboolean(st, b);
            return 1;
        }
    }
}


