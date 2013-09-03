
#include "lua/script.hpp"

namespace lua
{
    Script::Script()
        : m_state(NULL), m_loaded(false)
    {
        m_state = luaL_newstate();
        if(!m_state) {
            core::logger::logm("Couldn't open lua state.", core::logger::WARNING);
            throw lua::exception("Lua state couldn't be opened.");
        }
        luaL_openlibs(m_state);
    }

    Script::Script(const std::string& path)
        : Script()
    {
        load(path);
    }

    Script::~Script()
    {
        if(m_state)
            lua_close(m_state);
    }

    bool Script::load(const std::string& path)
    {
        if(loaded())
            return false;

        if(luaL_dofile(m_state, path.c_str()) != 0) {
            std::ostringstream oss;
            oss << "Couldn't execute lua script : \"" << path << "\"";
            core::logger::logm(oss.str(), core::logger::WARNING);
        }
        else
            m_loaded = true;

        return m_loaded;
    }

    bool Script::loaded() const
    {
        return (m_state && m_loaded);
    }

    bool Script::existsFunction(const std::string& name)
    {
        if(!loaded())
            return false;

        lua_getglobal(m_state, name.c_str());
        bool ret = lua_isfunction(m_state, -1);
        lua_pop(m_state, -1);

        return ret;
    }
            
    void Script::addArgs()
    {
        /* Do nothing, for internal use */
    }
            
    void Script::addArg(const std::string& str)
    {
        lua_pushstring(m_state, str.c_str());
    }
            
    void Script::addArg(double number)
    {
        lua_pushnumber(m_state, number);
    }
            
    Script::VarType Script::typeVariable(const std::string& name)
    {
        if(!loaded())
            return NIL;

        lua_settop(m_state, 0);
        lua_getglobal(m_state, name.c_str());

        VarType ret = NIL;
        if(lua_isnumber(m_state, 1))
            ret = NUMBER;
        else if(lua_isstring(m_state, 1))
            ret = STRING;
        else if(lua_isboolean(m_state, 1))
            ret = BOOL;
        else if(lua_istable(m_state, 1))
            ret = TABLE;
        else if(lua_isuserdata(m_state, 1))
            ret = USER;

        lua_pop(m_state, 1);
        return ret;
    }
            
    void Script::setVariable(const std::string& name, const std::string& str)
    {
        if(!loaded())
            throw lua::exception("Tryed to use a non loaded script");

        lua_pushstring(m_state, str.c_str());
        lua_setglobal(m_state, name.c_str());
    }
            
    void Script::setVariable(const std::string& name, const char* str)
    {
        setVariable(name, std::string(str));
    }
            
    void Script::setVariable(const std::string& name, double number)
    {
        if(!loaded())
            throw lua::exception("Tryed to use a non loaded script");

        lua_pushnumber(m_state, number);
        lua_setglobal(m_state, name.c_str());
    }
}


