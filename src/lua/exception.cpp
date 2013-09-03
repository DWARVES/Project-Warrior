
#include "lua/exception.hpp"

namespace lua
{
    exception::exception(const char* msg) noexcept
    {
        m_msg = msg;
    }

    exception::exception(const exception& cp) noexcept
    {
        m_msg = cp.m_msg;
    }

    exception& exception::operator=(const exception& cp) noexcept
    {
        m_msg = cp.m_msg;
        return *this;
    }

    exception::~exception()
    {}

    const char* exception::what() const noexcept
    {
        return m_msg;
    }

}


