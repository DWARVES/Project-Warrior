
#ifndef DEF_LUA_STAGEEXPOSURE
#define DEF_LUA_STAGEEXPOSURE

#include "lua/script.hpp"
#include "geometry/aabb.hpp"
#include "geometry/point.hpp"
namespace gameplay
{
    class Stage;
}

namespace lua
{
    namespace exposure
    {
        /** @brief Class to expose Stage to its lua script. */
        class Stage
        {
            public:
                Stage(lua_State*);
                static void setUsedStage(gameplay::Stage* st);

                /* Lua exposure. */
                static const char* className;
                static const Script::Methods<Stage> methods[];
                static const Script::Properties<Stage> properties[];
                bool isExisting;
                bool isPrecious;
                static void expose(Script* scr);

                /* Exposed methods. */
                int worldCenter(lua_State* st);
                int maxSize(lua_State* st);
                int deathRect(lua_State* st);
                int minSize(lua_State* st);
                int windowWidth(lua_State* st);
                int windowHeight(lua_State* st);
                int appearPos(lua_State* st);
                int appearView(lua_State* st);
                int obstacle(lua_State* st);
                int platform(lua_State* st);


            private:
                static gameplay::Stage* m_used; /**< @brief Stage actually used in this script. */
                /** @brief Indicates if the arguments are an AABB and get it. */
                bool isAABB(lua_State* st, geometry::AABB& get);
                /** @brief Indicates if the arguments are a Point and get it. */
                bool isPoint(lua_State* st, geometry::Point& get);
        };
    }
}

#endif

