
#include "pathExposure.hpp"
#include "helper.hpp"
#include "core/pathParser.hpp"

namespace lua
{
    namespace exposure
    {
        const char* Path::className = "Path";
        const Script::Methods<Path> Path::methods[] = {
            {"extension",     &Path::extension},
            {"head",          &Path::head},
            {"body",          &Path::body},
            {"absolute",      &Path::absolute},
            {"is_dir",        &Path::is_dir},
            {"is_reg",        &Path::is_reg},
            {"exists",        &Path::exists},
            {"list_contents", &Path::list_contents},
            {"has_content",   &Path::has_content},
            {"next_content",  &Path::get_content},
            {NULL, NULL}
        };
        const Script::Properties<Path> Path::properties[] = {
            {NULL, NULL, NULL}
        };

        Path::Path(lua_State*)
            : isPrecious(false)
        {}

        void Path::expose(Script* scr)
        {
            scr->registerClass<Path>();
        }

        int Path::extension(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::STRING)
                return 0;
            std::string ret = core::path::extension(lua_tostring(st, 1));
            return helper::returnString(st, ret);
        }

        int Path::head(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::STRING)
                return 0;
            std::string ret = core::path::head(lua_tostring(st, 1));
            return helper::returnString(st, ret);
        }

        int Path::body(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::STRING)
                return 0;
            std::string ret = core::path::body(lua_tostring(st, 1));
            return helper::returnString(st, ret);
        }

        int Path::absolute(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::STRING)
                return 0;
            bool ret = core::path::absolute(lua_tostring(st, 1));
            return helper::returnBoolean(st, ret);
        }

        int Path::is_dir(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::STRING)
                return 0;
            bool ret = core::path::type(lua_tostring(st, 1)) == core::path::Type::Dir;
            return helper::returnBoolean(st, ret);
        }

        int Path::is_reg(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::STRING)
                return 0;
            bool ret = core::path::type(lua_tostring(st, 1)) == core::path::Type::Reg;
            return helper::returnBoolean(st, ret);
        }

        int Path::exists(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::STRING)
                return 0;
            bool ret = core::path::type(lua_tostring(st, 1)) != core::path::Type::Unexistant;
            return helper::returnBoolean(st, ret);
        }

        int Path::list_contents(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::STRING)
                return 0;
            m_contents = core::path::dirContents(lua_tostring(st, 1));
            m_actual = 0;
            return helper::returnBoolean(st, !m_contents.empty());
        }

        int Path::has_content(lua_State* st)
        {
            bool ret = m_actual < m_contents.size();
            return helper::returnBoolean(st, ret);
        }

        int Path::get_content(lua_State* st)
        {
            if(m_actual >= m_contents.size())
                return 0;
            return helper::returnString(st, m_contents[m_actual++]);
        }

    }
}


