
#include "core/systemtime.hpp"
#include "core/logger.hpp"
#include <cstring>

namespace core
{
    SystemTime::SystemTime()
        : m_local(true)
    {
        m_tm = new struct tm;
        useSystemTime();
    }

    SystemTime::SystemTime(const SystemTime& cp)
        : m_local(true)
    {
        m_tm = new struct tm;
        memcpy(m_tm, cp.m_tm, sizeof(*m_tm));
    }

    SystemTime& SystemTime::operator=(const SystemTime& cp)
    {
        memcpy(m_tm, cp.m_tm, sizeof(*m_tm));
    }

    SystemTime::~SystemTime()
    {
        if(m_tm)
            delete m_tm;
    }

    void SystemTime::set(const time_t& seconds, bool utc)
    {
        struct tm* tm = NULL;
        if(utc)
            tm = gmtime(&seconds);
        else
            tm = localtime(&seconds);

        if(tm == NULL) {
            memset(m_tm, sizeof(*m_tm), 0);
            logger::log("Couldn't get time structure from timestamp.", logger::ERROR, false);
            return;
        }

        memcpy(m_tm, tm, sizeof(*m_tm));
        m_local = !utc;
    }

    void SystemTime::set(int year, int month, int day, int hour, int min, int sec)
    {
        struct tm tm;
        tm.tm_year  = year - 1900;
        tm.tm_mon   = month - 1;
        tm.tm_mday  = day;
        tm.tm_hour  = hour;
        tm.tm_min   = min;
        tm.tm_sec   = sec;
        tm.tm_isdst = -1;

        time_t t = mktime(&tm);
        t = std::max((time_t)0, t);
        set(t, false);
    }

    void SystemTime::useSystemTime()
    {
        time_t t = time(NULL);
        t = std::max((time_t)0, t);
        set(t, false);
    }

    void SystemTime::useUTCTime()
    {
        time_t t = time(NULL);
        t = std::max((time_t)0, t);
        set(t, true);
    }

    int SystemTime::get(SystemTime::TimePart part) const
    {
        switch(part) {
            case YEAR:
                return m_tm->tm_year + 1900;
            case LYEAR:
                return m_tm->tm_year;
            case MDAY:
                return m_tm->tm_mday;
            case WDAY:
                return m_tm->tm_wday;
            case YDAY:
                return m_tm->tm_yday;
            case MONTH:
                return m_tm->tm_mon;
            case HOUR:
                return m_tm->tm_hour;
            case MINUTE:
                return m_tm->tm_min;
            case SECONDS:
                return m_tm->tm_sec;
        }
    }

    std::string SystemTime::format() const
    {
        char* fmt = asctime(m_tm);
        if(fmt == NULL) {
            logger::log("Couldn't convert time structure to ascii string.", logger::ERROR, false);
            return "";
        }

        for(size_t i = 0; i < strlen(fmt); ++i) {
            if(fmt[i] == '\n') {
                fmt[i] = '\0';
                break;
            }
        }
        return std::string(fmt);
    }

    std::string SystemTime::format(const std::string& format, size_t maxSize) const
    {
        char* buffer = new char[maxSize];
        size_t written = strftime(buffer, maxSize, format.c_str(), m_tm);

        std::string ret;
        if(written != 0)
            ret = buffer;
        else
            logger::log("Couldn't convert time structure to ascii string.", logger::ERROR, false);

        delete[] buffer;
        return ret;
    }

    SystemTime& SystemTime::operator-=(const SystemTime& t)
    {
        time_t time1, time2;
        time1 = mktime(m_tm);
        time2 = mktime(t.m_tm);

        time_t diff = std::max(0.0, difftime(time1, time2));
        set(diff, !m_local);
    }

    SystemTime operator-(const SystemTime& t1, const SystemTime& t2)
    {
        SystemTime n(t1);
        n -= t2;
        return n;
    }

}

std::ostream& operator<<(std::ostream& os, const core::SystemTime& t)
{
    std::string toprint = t.format();
    os << toprint;
    return os;
}



