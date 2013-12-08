
#include "utf8.hpp"
#include "utf8/utf8.h"
#include <cstring>

namespace core
{
    UTF8String::UTF8String()
    {}

    UTF8String::UTF8String(const UTF8String& cp)
        : m_src(cp.m_src)
    {}

    UTF8String::UTF8String(const std::string& src)
        : m_src(src)
    {}

    UTF8String::~UTF8String()
    {
    }

    size_t UTF8String::size() const
    {
        utf8::iterator<std::string::const_iterator> it(m_src.cbegin(), m_src.cbegin(), m_src.cend());
        utf8::iterator<std::string::const_iterator> end(m_src.cend(), m_src.cbegin(), m_src.cend());
        size_t count = 0;
        while(it != end) {
            ++count;
            ++it;
        }
        return count;
    }

    void UTF8String::clear()
    {
        m_src.clear();
    }

    bool UTF8String::empty() const
    {
        return m_src.empty();
    }

    bool UTF8String::valid() const
    {
        return utf8::is_valid(m_src.begin(), m_src.end());
    }

    void UTF8String::removeErrors()
    {
        std::string temp;
        utf8::replace_invalid(m_src.begin(), m_src.end(), std::back_inserter(temp));
        m_src = temp;
    }

    UTF8String& UTF8String::operator=(const UTF8String& cp)
    {
        m_src = cp.m_src;
        return *this;
    }

    std::string UTF8String::getSrc() const
    {
        return m_src;
    }

    UTF8String::operator std::string() const
    {
        return getSrc();
    }
            
    unsigned int UTF8String::operator[](size_t idx) const
    {
        utf8::iterator<std::string::const_iterator> it(m_src.cbegin(), m_src.cbegin(), m_src.cend());
        for(size_t i = 0; i < idx; ++i)
            ++it;
        return *it;
    }

    bool operator==(const UTF8String& s1, const UTF8String& s2)
    {
        return s1.getSrc() == s2.getSrc();
    }

    std::ostream& operator<<(std::ostream& os, const UTF8String& str)
    {
        os << str.getSrc();
        return os;
    }

}



