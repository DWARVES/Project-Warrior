
#include "lua/saveExposure.hpp"
#include "lua/helper.hpp"
#include <vector>

namespace lua
{
    namespace exposure
    {
        /* Static members */
        const char* Save::className = "EventsVariables";
        const Script::Methods<Save> Save::methods[] = {
            {"namespace", &Save::actualNamespace},
            {"create",    &Save::createVariable},
            {"delete",    &Save::deleteVariable},
            {"set",       &Save::setVariable},
            {"get",       &Save::getVariable},
            {"exists",    &Save::existsVariable},
            {NULL, NULL}
        };
        const Script::Properties<Save> Save::properties[] = {
            {NULL, NULL, NULL}
        };

        /* Methods */
        Save::Save(lua_State*)
            : isPrecious(false)
        {}

        int Save::actualNamespace(lua_State* st)
        {
            return helper::returnString(st, m_sav.actualNamespace());
        }

        int Save::createVariable(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 2
                    || args[0] != Script::STRING
                    || args[1] != Script::NUMBER)
                return helper::returnBoolean(st, false);
            bool ret = m_sav.createVariable(lua_tostring(st, 1), lua_tonumber(st, 2));
            return helper::returnBoolean(st, ret);
        }

        int Save::deleteVariable(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::STRING)
                return 0;
            m_sav.deleteVariable(lua_tostring(st, 1));
            return 0;
        }

        int Save::setVariable(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 2
                    || args[0] != Script::STRING
                    || args[1] != Script::NUMBER)
                return helper::returnBoolean(st, false);
            bool ret = m_sav.setVariable(lua_tostring(st, 1),
                    static_cast<events::Save::number_t>(lua_tonumber(st, 2)));
            return helper::returnBoolean(st, ret);
        }

        int Save::existsVariable(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::STRING)
                return helper::returnBoolean(st, false);
            bool ret = m_sav.existsVariable(lua_tostring(st, 1));
            return helper::returnBoolean(st, ret);
        }

        int Save::getVariable(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::STRING)
                return helper::returnNumber(st, 0);
            events::Save::number_t ret = m_sav.getVariable(lua_tostring(st, 1));
            return helper::returnNumber(st, static_cast<double>(ret));
        }
        
        void Save::expose(Script* scr)
        {
            scr->registerClass<Save>();
        }
    }
}


