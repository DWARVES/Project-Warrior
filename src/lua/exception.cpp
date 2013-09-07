
#include "lua/exception.hpp"

namespace lua
{
    /*** Program exception ***/
    program_exception::program_exception(const char* e) noexcept
        : m_msg(e)
        {}

    const char* program_exception::what() const noexcept
    {
        return m_msg;
    }

    /*** Non loaded exception ***/
    nonloaded_exception::nonloaded_exception() noexcept
        : program_exception("Tryed to use a non-loaded lua script.")
        {}

    /*** Variable type exception ***/
    vartype_exception::vartype_exception(const char* tC, const char* tLua, const char* name) noexcept
        : m_c(tC), m_lua(tLua), m_var(name)
        {}

    const char* vartype_exception::what() const noexcept
    {
        return "Tryed to access a lua variable with a wrong type.";
    }

    const char* vartype_exception::typeC() const noexcept
    {
        return m_c;
    }

    const char* vartype_exception::typeLua() const noexcept
    {
        return m_lua;
    }

    const char* vartype_exception::nameVar() const noexcept
    {
        return m_var;
    }

    /*** Function access exception ***/
    functionaccess_exception::functionaccess_exception(const char* fname) noexcept
        : m_function(fname)
        {}

    const char* functionaccess_exception::what() const noexcept
    {
        return "Invalid access to a lua function.";
    }

    const char* functionaccess_exception::function() const noexcept
    {
        return m_function;
    }

}


