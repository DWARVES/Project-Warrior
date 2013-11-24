
#ifndef DEF_LUA_EXCEPTION
#define DEF_LUA_EXCEPTION

#include <exception>

namespace lua
{
    /** @brief Pure virtual class, can be used to catch all the exceptions related to lua. */
    class exception : public std::exception
    {
        public:
            virtual const char* what() const noexcept = 0;
    };

    /** @brief Class for exceptions directly in the lua::Script class. */
    class program_exception : public exception
    {
        public:
            program_exception() = delete;
            /** @brief Only constructor allowed.
             * @param e The message of the exception.
             */
            explicit program_exception(const char* e) noexcept;
            virtual const char* what() const noexcept;
        private:
            const char* m_msg;
    };

    /** @brief Class for when the user try to use a non-loaded lua::Script class. */
    class nonloaded_exception : public program_exception
    {
        public:
            /** @brief The constructor set the generic error message, which  can't be changed. */
            nonloaded_exception() noexcept;
    };

    /** @brief Virtual pure class for errors in the lua script. */
    class script_exception : public exception
    {
        public:
            virtual const char* what() const noexcept = 0;
    };

    /** @brief Class for mismatching types when accessing a lua variable. */
    class vartype_exception : public script_exception
    {
        public:
            vartype_exception() = delete;
            /** @brief Only one constructor.
             * @param tC The c++ variable type's name.
             * @param tlua The lua variable type's name.
             * @param name The name of the lua variable.
             */
            vartype_exception(const char* tC, const char* tLua, const char* name) noexcept;
            /** @brief Returns a generic error message. */
            virtual const char* what() const noexcept;
            /** @brief Returns the c++ variable type's name. */
            const char* typeC() const noexcept;
            /** @brief Returns the lua variable type's name. */
            const char* typeLua() const noexcept;
            /** @brief Returns the lua variable name. */
            const char* nameVar() const noexcept;

        private:
            /** @brief The c++ variable type's name. */
            const char* m_c;
            /** @brief The lua variable type's name. */
            const char* m_lua;
            /** @brief The lua variable name. */
            const char* m_var;
    };

    /** @brief Class for exceptions for invalid lua function access. */
    class functionaccess_exception : public script_exception
    {
        public:
            functionaccess_exception() = delete;
            /** @brief Only one constructor.
             * @param fname The name of the lua function.
             */
            functionaccess_exception(const char* fname) noexcept;
            /** @brief Returns a generic error message. */
            virtual const char* what() const noexcept;
            /** @brief Returns the name of the lua function. */
            const char* function() const noexcept;

        private:
            /** @brief The name of the lua function. */
            const char* m_function;
    };
}

#endif

