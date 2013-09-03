
#ifndef DEF_LUA_EXCEPTION
#define DEF_LUA_EXCEPTION

#include <exception>

namespace lua
{
    class exception : public std::exception
    {
        public:
            exception() = delete;
            /* msg will not be delete'd */
            explicit exception(const char* msg) noexcept;

            exception(const exception& cp) noexcept;
            exception& operator=(const exception& cp) noexcept;
            virtual ~exception();

            /* Can return NULL */
            virtual const char* what() const noexcept;

        private:
            const char* m_msg;
    };
}

#endif

