
#ifndef DEF_LUA_SAVEEXPOSURE
#define DEF_LUA_SAVEEXPOSURE

#include "lua/script.hpp"
#include "gameplay/save.hpp"

namespace lua
{
    namespace exposure
    {
        /* See events::Save for the meaning of the following functions */
        class Save
        {
            public:
                Save(lua_State*);

                /* Namespace gesture functions are not exposed */
                int actualNamespace(lua_State* st);

                int createVariable(lua_State* st);
                int deleteVariable(lua_State* st);
                int setVariable(lua_State* st);
                int existsVariable(lua_State* st);
                int getVariable(lua_State* st);

                /* Saving and loading functions are not exposed */

                /* Lua exposure */
                static const char* className;
                static const Script::Methods<Save> methods[];
                static const Script::Properties<Save> properties[];
                bool isExisting;
                bool isPrecious;

                static void expose(Script* scr);

            private:
                gameplay::Save m_sav;
        };
    }
}

#endif

