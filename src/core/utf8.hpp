
#ifndef DEF_CORE_UTF8
#define DEF_CORE_UTF8

#include <string>
#include <vector>

namespace core
{
    /** @brief A string with utf-8 based methods.
     *
     * You should use this only when you really need precise utf-8 handling,
     * because a simple std::string can do the job most of the time.
     */
    class UTF8String
    {
        public:
            UTF8String();
            UTF8String(const UTF8String& cp);
            /** @brief Creates an UTF8String based on a plain string. */
            UTF8String(const std::string& src);
            ~UTF8String();

            /** @brief The number of characters. */
            size_t size() const;
            /** @brief Empty the string. */
            void clear();
            /** @brief Check if the string is empty. */
            bool empty() const;

            /** @brief Check if there is an error in the utf-8 codage. */
            bool valid() const;
            /** @brief Will removes any character with encoding errors, replacing it by standart unicode. */
            void removeErrors();

            UTF8String& operator=(const UTF8String& cp);

            /** @brief Returns the plain string representing the string. */
            std::string getSrc() const;
            operator std::string() const;

            /** @brief Access utf8 elements of the string. The value returned can't be printed directly : it's the unicode number of the character.
             * Undefined behaviour may happen if idx is outside range.
             */
            unsigned int operator[](size_t idx) const;

        private:
            std::string m_src; /**< @brief The plain string stored. */
    };

    bool operator==(const UTF8String& s1, const UTF8String& s2);
    std::ostream& operator<<(std::ostream& os, const UTF8String& str);
}

#endif

