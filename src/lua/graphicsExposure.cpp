
#include "graphicsExposure.hpp"
#include "lua/helper.hpp"
#include <vector>

namespace lua
{
    namespace exposure
    {
        /* Exposure static memebers */
        const char* Graphics::className = "Graphics";
        const Script::Methods<Graphics> Graphics::methods[] = {
            {"namespace",   &Graphics::actualNamespace},
            {"create",      &Graphics::createNamespace},
            {"delete",      &Graphics::deleteNamespace},
            {"enter",       &Graphics::enterNamespace},
            {"loadTexture", &Graphics::loadTexture},
            {"loadMovie",   &Graphics::loadMovie},
            {"loadFont",    &Graphics::loadFont},
            {"exists",      &Graphics::existsEntity},
            {"free",        &Graphics::free},
            {"link",        &Graphics::link},
            {"hotpoint",    &Graphics::setTextureHotPoint},
            {"rewind",      &Graphics::rewindMovie},
            {"rotate",      &Graphics::rotate},
            {"scale",       &Graphics::scale},
            {"move",        &Graphics::move},
            {"identity",    &Graphics::identity},
            {"push",        &Graphics::push},
            {"pop",         &Graphics::pop},
            {"blit",        &Graphics::blitTexture},
            {"drawRect",    &Graphics::drawAABB},
            {"drawText",    &Graphics::drawText},
            {"play",        &Graphics::play},
            {NULL, NULL}
        };
        const Script::Properties<Graphics> Graphics::properties[] = {
            {NULL, NULL, NULL}
        };
                
        void Graphics::expose(Script* scr)
        {
            scr->registerClass<Graphics>();
        }

        /* static members and methods */
        graphics::Graphics* Graphics::m_gfx = NULL;
        void Graphics::setGraphicsInstance(graphics::Graphics* gfx)
        {
            m_gfx = gfx;
        }

        /* Methods */
        Graphics::Graphics(lua_State*)
            : isPrecious(false)
        {}

        int Graphics::enterNamespace(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::STRING)
                return 0;
            bool ret = m_gfx->enterNamespace(lua_tostring(st, 1));
            return helper::returnBoolean(st, ret);
        }

        int Graphics::createNamespace(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::STRING)
                return 0;
            bool ret = m_gfx->createNamespace(lua_tostring(st, 1));
            return helper::returnBoolean(st, ret);
        }

        int Graphics::deleteNamespace(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::STRING)
                return 0;
            m_gfx->deleteNamespace(lua_tostring(st, 1));
            return 0;
        }

        int Graphics::actualNamespace(lua_State* st)
        {
            return helper::returnString(st, m_gfx->actualNamespace());
        }

        int Graphics::loadTexture(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 2
                    || args[0] != Script::STRING
                    || args[1] != Script::STRING)
                return 0;
            bool ret = m_gfx->loadTexture(lua_tostring(st, 1), lua_tostring(st, 2));
            return helper::returnBoolean(st, ret);
        }

        int Graphics::loadMovie(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 2
                    || args[0] != Script::STRING
                    || args[1] != Script::STRING)
                return 0;
            bool ret = m_gfx->loadMovie(lua_tostring(st, 1), lua_tostring(st, 2));
            return helper::returnBoolean(st, ret);
        }

        int Graphics::loadFont(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 2
                    || args[0] != Script::STRING
                    || args[1] != Script::STRING)
                return 0;
            bool ret = m_gfx->loadFont(lua_tostring(st, 1), lua_tostring(st, 2));
            return helper::returnBoolean(st, ret);
        }

        int Graphics::existsEntity(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::STRING)
                return 0;
            graphics::Graphics::RcType rct = m_gfx->rctype(lua_tostring(st, 1));
            return helper::returnBoolean(st, (rct != graphics::Graphics::NONE));
        }

        int Graphics::free(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::STRING)
                return 0;
            m_gfx->free(lua_tostring(st, 1));
            return 0;
        }

        int Graphics::link(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 2
                    || args[0] != Script::STRING
                    || args[1] != Script::STRING)
                return 0;
            bool ret = m_gfx->link(lua_tostring(st, 1), lua_tostring(st, 2));
            return helper::returnBoolean(st, ret);
        }

        int Graphics::setTextureHotPoint(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 3
                    || args[0] != Script::STRING
                    || args[1] != Script::NUMBER
                    || args[2] != Script::NUMBER)
                return 0;
            bool ret = m_gfx->setTextureHotpoint(lua_tostring(st, 1), (int)lua_tointeger(st, 2), (int)lua_tointeger(st, 3));
            return helper::returnBoolean(st, ret);
        }

        int Graphics::rewindMovie(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::STRING)
                return 0;
            m_gfx->rewindMovie(lua_tostring(st, 1));
            return 0;
        }

        int Graphics::rotate(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::NUMBER)
                return 0;
            m_gfx->rotate((float)lua_tonumber(st, 1));
            return 0;
        }

        int Graphics::scale(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 2
                    || args[0] != Script::NUMBER
                    || args[1] != Script::NUMBER)
                return 0;
            m_gfx->scale((float)lua_tonumber(st, 1), (float)lua_tonumber(st, 2));
            return 0;
        }

        int Graphics::move(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 2
                    || args[0] != Script::NUMBER
                    || args[1] != Script::NUMBER)
                return 0;
            m_gfx->move((float)lua_tonumber(st, 1), (float)lua_tonumber(st, 2));
            return 0;
        }

        int Graphics::identity(lua_State*)
        {
            m_gfx->identity();
            return 0;
        }

        int Graphics::push(lua_State*)
        {
            m_gfx->push();
            return 0;
        }

        int Graphics::pop(lua_State*)
        {
            m_gfx->pop();
            return 0;
        }

        int Graphics::blitTexture(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::STRING)
                return 0;
            m_gfx->blitTexture(lua_tostring(st, 1), geometry::Point(0.0f, 0.0f));
            return 0;
        }

        int Graphics::drawAABB(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() < 3
                    || args[0] != Script::STRING  /* texture name */
                    || args[1] != Script::NUMBER  /* width */
                    || args[2] != Script::NUMBER) /* height */
                return 0;

            float repeatX = 1.0f;
            if(args.size() >= 4 && args[3] == Script::NUMBER)
                repeatX = (float)lua_tonumber(st, 4);
            float repeatY = 1.0f;
            if(args.size() >= 5 && args[4] == Script::NUMBER)
                repeatY = (float)lua_tonumber(st, 5);

            m_gfx->draw(geometry::AABB((float)lua_tonumber(st, 2), (float)lua_tonumber(st, 3)), lua_tostring(st, 1), repeatX, repeatY);
            return 0;
        }

        int Graphics::drawText(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() < 2
                    || args[0] != Script::STRING
                    || args[1] != Script::STRING)
                return 0;

            float pts = -1.0f;
            if(args.size() >= 3 && args[2] == Script::NUMBER)
                pts = (float)lua_tonumber(st, 3);

            m_gfx->draw(lua_tostring(st, 1), lua_tostring(st, 2), pts);
            return 0;
        }

        int Graphics::play(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() < 3
                    || args[0] != Script::STRING
                    || args[1] != Script::NUMBER
                    || args[2] != Script::NUMBER)
                return 0;

            bool ratio = true;
            if(args.size() > 4 && args[3] == Script::BOOL)
                ratio = lua_toboolean(st, 4);

            bool ret = m_gfx->play(lua_tostring(st, 1), geometry::AABB((float)lua_tonumber(st, 2), (float)lua_tonumber(st, 3)), ratio);
            return helper::returnBoolean(st, ret);
        }

    }
}


