
#ifndef DEF_LUA_EXCEPTION
#define DEF_LUA_EXCEPTION

#include <exception>

namespace lua
{
    /* Virtual pure class : global class for all lua exceptions */
    class exception : public std::exception
    {
        public:
            virtual const char* what() const noexcept = 0;
    };

    /* Class for exceptions directly in the lua::Script class */
    class program_exception : public exception
    {
        public:
            program_exception() = delete;
            explicit program_exception(const char* e) noexcept;
            virtual const char* what() const noexcept;
        private:
            const char* m_msg;
    };

    /* Class for when the user try to use a non-loaded lua::Script class */
    class nonloaded_exception : public program_exception
    {
        public:
            nonloaded_exception() noexcept;
    };

    /* Virtual pure class for errors in the lua script (invalid variable types...) */
    class script_exception : public exception
    {
        public:
            virtual const char* what() const noexcept = 0;
    };

    /* Class for exceptions for invalid variable access to lua type */
    class vartype_exception : public script_exception
    {
        public:
            vartype_exception() = delete;
            vartype_exception(const char* tC, const char* tLua, const char* name) noexcept;
            virtual const char* what() const noexcept;
            const char* typeC() const noexcept;   /* The name of the C/C++ type */
            const char* typeLua() const noexcept; /* The name of the lua type */
            const char* nameVar() const noexcept; /* The name of the lua variable */

        private:
            const char* m_c;
            const char* m_lua;
            const char* m_var;
    };

    /* Class for exceptions for invalid function access */
    class functionaccess_exception : public script_exception
    {
        public:
            functionaccess_exception() = delete;
            functionaccess_exception(const char* fname) noexcept;
            virtual const char* what() const noexcept;
            const char* function() const noexcept; /* Return the name of the function */

        private:
            const char* m_function;
    };
}

#endif

