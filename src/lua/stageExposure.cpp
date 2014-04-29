
#include "stageExposure.hpp"
#include "lua/helper.hpp"
#include "gameplay/stage.hpp"

namespace lua
{
    namespace exposure
    {
        /* Lua exposure. */
        const char* Stage::className = "Stage";
        const Script::Methods<Stage> Stage::methods[] = {
            {NULL, NULL}
        };
        const Script::Properties<Stage> Stage::properties[] = {
            {NULL, NULL, NULL},
        };
        gameplay::Stage* Stage::m_used = NULL;

        void Stage::expose(Script* scr)
        {
            scr->registerClass<Stage>();
        }

        /* Methods definition. */
        Stage::Stage(lua_State*)
            : isPrecious(false)
        {}

        void Stage::setUsedStage(gameplay::Stage* st)
        {
            m_used = st;
        }

        int Stage::worldCenter(lua_State* st)
        {
            geometry::Point p;
            if(isPoint(st, p))
                m_used->setWorldCenter(p);
            return 0;
        }

        int Stage::maxSize(lua_State* st)
        {
            geometry::AABB rect;
            if(isAABB(st, rect))
                m_used->setMaxSize(rect);
            return 0;
        }

        int Stage::deathRect(lua_State* st)
        {
            geometry::AABB rect;
            if(isAABB(st, rect))
                m_used->setDeathRect(rect);
            return 0;
        }

        int Stage::minSize(lua_State* st)
        {
            geometry::AABB rect;
            if(isAABB(st, rect))
                m_used->setMinSize(rect);
            return 0;
        }

        int Stage::windowWidth(lua_State* st)
        {
            float w = m_used->getWindowWidth();
            return helper::returnNumber(st, w);
        }

        int Stage::windowHeight(lua_State* st)
        {
            float h = m_used->getWindowHeight();
            return helper::returnNumber(st, h);
        }

        int Stage::appearPos(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 3
                    || args[0] != Script::NUMBER
                    || args[1] != Script::NUMBER
                    || args[2] != Script::NUMBER)
                return 0;

            int id = (int)lua_tonumber(st, 1);
            if(id < 0 || id >= 4)
                return 0;

            geometry::Point pos((float)lua_tonumber(st, 2), (float)lua_tonumber(st, 3));
            m_used->setAppearPos(id, pos);
            return 0;
        }

        int Stage::appearSize(lua_State* st)
        {
            geometry::AABB rect;
            if(isAABB(st, rect))
                m_used->setAppearSize(rect);
            return 0;
        }

        int Stage::appearView(lua_State* st)
        {
            geometry::AABB rect;
            if(isAABB(st, rect))
                m_used->setAppearView(rect);
            return 0;
        }

        bool Stage::isAABB(lua_State* st, geometry::AABB& get)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 2
                    || args[0] != Script::NUMBER
                    || args[1] != Script::NUMBER)
                return false;
            get.width  = (float)lua_tonumber(st, 1);
            get.height = (float)lua_tonumber(st, 2);
            return true;
        }

        bool Stage::isPoint(lua_State* st, geometry::Point& get)
        {
            geometry::AABB rect;
            if(!isAABB(st, rect))
                return false;
            get.x = rect.width;
            get.y = rect.height;
            return true;
        }

    }
}

