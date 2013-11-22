
#include "lua/script.hpp"

namespace lua
{
    Script::Script()
        : m_state(NULL), m_loaded(false)
    {
        m_state = luaL_newstate();
        if(!m_state) {
            core::logger::logm("Couldn't open lua state.", core::logger::WARNING);
            throw lua::program_exception("Lua state couldn't be opened.");
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
            throw nonloaded_exception();

        lua_getglobal(m_state, name.c_str());
        bool ret = lua_isfunction(m_state, -1);
        lua_pop(m_state, -1);

        return ret;
    }
            
    void Script::addArgs()
    {
        /* Do nothing, for internal use */
    }
            
    void Script::getRet(std::string* r)
    {
        *r = lua_tostring(m_state, -1);
    }
            
    void Script::getRet(bool* r)
    {
        *r = lua_toboolean(m_state, -1);
    }
            
    void Script::getRet(void* v)
    {
        /* Do nothing, just to handle void return */
        if(v) {}
    }
            
    Script::VarType Script::typeVariable(const std::string& name)
    {
        if(!loaded())
            throw nonloaded_exception();

        gettop(name);
        VarType ret = typeTop();
        lua_pop(m_state, 1);
        return ret;
    }
            
    Script::VarType Script::typeTop()
    {
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
        return ret;
    }
            
    const char* Script::nameType(Script::VarType t) const
    {
        switch(t) {
            case NUMBER:
                return "number";
            case STRING:
                return "string";
            case BOOL:
                return "bool";
            case TABLE:
                return "table";
            case USER:
                return "userdata";
            case NIL:
                return "nil/unexistant";
            default: /* Shouldn't happen */
                return "unknown";
        }
    }
            
    void Script::getVariable(const std::string& name, std::string& val)
    {
        gettop(name);
        if(lua_isstring(m_state, 1)) {
            val = lua_tostring(m_state, 1);
            lua_pop(m_state, 1);
        }
        else {
            std::ostringstream oss;
            oss << "Tryed to get " << name << " lua variable as string, which is not its type.";
            core::logger::logm(oss.str(), core::logger::WARNING);
            const char* type = nameType(typeTop());
            lua_pop(m_state, 1);
            throw lua::vartype_exception("std::string", type, name.c_str());
        }
    }
            
    void Script::getVariable(const std::string& name, bool& val)
    {
        gettop(name);
        if(lua_isboolean(m_state, 1)) {
            val = lua_toboolean(m_state, 1);
            lua_pop(m_state, 1);
        }
        else {
            std::ostringstream oss;
            oss << "Tryed to get " << name << " lua variable as boolean, which is not its type.";
            core::logger::logm(oss.str(), core::logger::WARNING);
            const char* type = nameType(typeTop());
            lua_pop(m_state, 1);
            throw lua::vartype_exception("bool", type, name.c_str());
        }
    }
            
    void Script::setVariable(const std::string& name, const std::string& str)
    {
        if(!loaded())
            throw lua::nonloaded_exception();

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
            throw lua::nonloaded_exception();

        lua_pushnumber(m_state, number);
        lua_setglobal(m_state, name.c_str());
    }
            
    void Script::registerFunction(luaFnPtr fn, const std::string& name)
    {
        if(!m_state)
            throw lua::nonloaded_exception();
        lua_register(m_state, name.c_str(), fn);
    }
            
    void Script::gettop(const std::string& name)
    {
        if(!loaded()) {
            core::logger::logm("Tryed to get a variable from a non loaded script", core::logger::WARNING);
            throw lua::nonloaded_exception();
        }

        lua_settop(m_state, 0);
        lua_getglobal(m_state, name.c_str());
    }
}


