
#ifndef DEF_LUA_GRAPHICSEXPOSURE
#define DEF_LUA_GRAPHICSEXPOSURE

#include "lua/script.hpp"
#include "graphics/graphics.hpp"

namespace lua
{
    namespace exposure
    {
        /* See graphics::Graphics for the meaning of the following functions */
        class Graphics
        {
            public:
                Graphics(lua_State*);
                static void setGraphicsInstance(graphics::Graphics* gfx);

                /* Lua exposure */
                static const char* className;
                static const Script::Methods<Graphics> methods[];
                static const Script::Properties<Graphics> properties[];
                bool isExisting;
                bool isPrecious;
                static void expose(Script* scr);

                /* Window manipulation methods are not exposed */
                /* Virtual size methods are not exposed */

                /* Namespace management */
                int enterNamespace(lua_State* st);
                int createNamespace(lua_State* st);
                int deleteNamespace(lua_State* st);
                int actualNamespace(lua_State* st);

                /* Ressources loading */
                int loadTexture(lua_State* st);
                int loadMovie(lua_State* st);
                int loadFont(lua_State* st);
                int existsEntity(lua_State* st);
                int free(lua_State* st);
                int link(lua_State* st);

                /* Ressources management */
                int setTextureHotPoint(lua_State* st);
                int rewindMovie(lua_State* st);

                /* Transformations */
                int rotate(lua_State* st);
                int scale(lua_State* st);
                int move(lua_State* st);
                int identity(lua_State*);
                int push(lua_State*);
                int pop(lua_State*);

                /* Drawing */
                int blitTexture(lua_State* st);
                /* drawAABB is the textured version of the draw method */
                int drawAABB(lua_State* st);
                int drawText(lua_State* st);
                int play(lua_State* st);

            private:
                static graphics::Graphics* m_gfx;
        };
    }
}

#endif

