
#ifndef DEF_LUA_PATHEXPOSURE
#define DEF_LUA_PATHEXPOSURE

#include "lua/script.hpp"
#include "graphics/graphics.hpp"

namespace lua
{
    namespace exposure
    {
        /** @brief A class exposing the methods of core/pathParser files. */
        class Path
        {
            public:
                Path(lua_State*);

                /* Lua exposure. */
                static const char* className;
                static const Script::Methods<Path> methods[];
                static const Script::Properties<Path> properties[];
                bool isExisting;
                bool isPrecious;
                static void expose(Script* scr);

                /* Methods to expose. */
                int extension(lua_State* st);
                int head(lua_State* st);
                int body(lua_State* st);
                int absolute(lua_State* st);
                int is_dir(lua_State* st);
                int is_reg(lua_State* st);
                int exists(lua_State* st);
                int list_contents(lua_State* st);
                int has_content(lua_State* st);
                int get_content(lua_State* st);

            private:
                std::vector<std::string> m_contents; /**< @brief The contents of the directory actually listed. */
                size_t m_actual;                     /**< @brief The position in the listing of a directory. */
        };
    }
}

#endif

