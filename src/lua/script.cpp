
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
            
    void Script::addArg()
    {
        /* Do nothing, for internal use */
    }
}


