
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
            {"worldCenter", &Stage::worldCenter},
            {"maxSize",     &Stage::maxSize},
            {"deathRect",   &Stage::deathRect},
            {"minSize",     &Stage::minSize},
            {"width",       &Stage::windowWidth},
            {"height",      &Stage::windowHeight},
            {"appearPos",   &Stage::appearPos},
            {"obstacle",    &Stage::obstacle},
            {"sensor",      &Stage::sensors},
            {"platform",    &Stage::platform},
            {"watch",       &Stage::setCallback},
            {"unwatch",     &Stage::unsetCallbacks},
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
                
        int Stage::obstacle(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(!(args.size() == 5
                    || (args.size() == 6 && args[5] == Script::NUMBER))
                    || args[0] != Script::STRING
                    || args[1] != Script::NUMBER
                    || args[2] != Script::NUMBER
                    || args[3] != Script::NUMBER
                    || args[4] != Script::NUMBER)
                return helper::returnBoolean(st, false);
            geometry::Point center((float)lua_tonumber(st, 2), (float)lua_tonumber(st, 3));
            geometry::AABB  rect  ((float)lua_tonumber(st, 4), (float)lua_tonumber(st, 5));
            std::string name = lua_tostring(st, 1);
            float friction = 1.0f;
            if(args.size() == 6)
                friction = (float)lua_tonumber(st, 6);
            bool ret = m_used->addObstacle(name, center, rect, friction);
            return helper::returnBoolean(st, ret);
        }

        int Stage::platform(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(!(args.size() == 5
                    || (args.size() == 6 && args[5] == Script::NUMBER))
                    || args[0] != Script::STRING
                    || args[1] != Script::NUMBER
                    || args[2] != Script::NUMBER
                    || args[3] != Script::NUMBER
                    || args[4] != Script::NUMBER)
                return helper::returnBoolean(st, false);
            geometry::Point center((float)lua_tonumber(st, 2), (float)lua_tonumber(st, 3));
            geometry::AABB  rect  ((float)lua_tonumber(st, 4), (float)lua_tonumber(st, 5));
            std::string name = lua_tostring(st, 1);
            float friction = 1.0f;
            if(args.size() == 6)
                friction = (float)lua_tonumber(st, 6);
            bool ret = m_used->addPlatform(name, center, rect, friction);
            return helper::returnBoolean(st, ret);
        }
                
        int Stage::sensors(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 5
                    || args[0] != Script::STRING
                    || args[1] != Script::NUMBER
                    || args[2] != Script::NUMBER
                    || args[3] != Script::NUMBER
                    || args[4] != Script::NUMBER)
                return helper::returnBoolean(st, false);
            geometry::Point center((float)lua_tonumber(st, 2), (float)lua_tonumber(st, 3));
            geometry::AABB  rect  ((float)lua_tonumber(st, 4), (float)lua_tonumber(st, 5));
            std::string name = lua_tostring(st, 1);
            bool ret = m_used->addSensor(name, center, rect);
            return helper::returnBoolean(st, ret);
        }
                
        int Stage::setCallback(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 4
                    || args[0] != Script::STRING
                    || args[1] != Script::STRING
                    || args[2] != Script::STRING
                    || args[3] != Script::STRING)
                return helper::returnBoolean(st, false);
            bool ret = m_used->setEntityCallbacks(lua_tostring(st, 1), lua_tostring(st, 2),
                    lua_tostring(st, 3), lua_tostring(st, 4));
            return helper::returnBoolean(st, ret);
        }

        int Stage::unsetCallbacks(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::STRING)
                return 0;
            m_used->unsetEntityCallbacks(lua_tostring(st, 1));
            return 0;
        }

    }
}

