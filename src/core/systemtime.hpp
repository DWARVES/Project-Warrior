
#ifndef DEF_CORE_SYSTEMTIME
#define DEF_CORE_SYSTEMTIME

#include <ctime>
#include <string>
#include <ostream>

namespace core
{
    /** @brief Represent a date. */
    class SystemTime
    {
        public:
            /** @brief Default constructor : will use the system time. */
            SystemTime();
            SystemTime(const SystemTime& cp);
            SystemTime& operator=(const SystemTime& cp);
            ~SystemTime();

            /** @brief Set to a fixed timestamp/
             * @param utc If true, then the time will be the UTC's, else it will be local's.
             */
            void set(const time_t& seconds, bool utc = false);
            /** @brief Set to a fixed date?
             * @param year Year since JC 0-?.
             * @param day Day of the month 1-31.
             * @param hour Hours since midnight 0-23.
             * @param min Minutes after the hour 0-59.
             * @param sec Seconds after the minute 0-59.
             */
            void set(int year, int month, int day, int hour = 0, int min = 0, int sec = 0);
            /** @brief Set to the system time */
            void useSystemTime();
            /** @brief Set to UTC time */
            void useUTCTime();

            /** @brief The differents part of the date. */
            enum TimePart {
                YEAR,      /**< @brief Year since JC 0-?. */
                LYEAR,     /**< @brief Year since 1970 -1970-?. */
                MDAY,      /**< @brief Day of the month 1-31. */
                WDAY,      /**< @brief Day of the week 0-6. */
                YDAY,      /**< @brief Day since jan 1 0-365. */
                MONTH,     /**< @brief Month since january 0-11. */
                HOUR,      /**< @brief Hours since midnight 0-23. */
                MINUTE,    /**< @brief Minutes after the hour 0-59. */
                SECONDS,   /**< @brief Seconds after the minute 0-59. */
            };
            /** @brief Get one of the time parts. */
            int get(TimePart part) const;

            /** @brief Return date string with default format ex : 'Sun Feb 27 21:39:40 2011'. */
            std::string format() const;
            /** @brief  Get the date with a prsonalized format.
             * @param fmt Describe the format. It has the same syntax as strftime from time.h.
             * @param maxSize Maximum size of returned length.
             */
            std::string format(const std::string& fmt, size_t maxSize) const;

            /** @brief Diff between t and this.
             * If t > this, this will be set to 0 (no negative time).
             */
            SystemTime& operator-=(const SystemTime& t);

        private:
            struct tm* m_tm; /**< @brief The structure storing the date. */
            bool m_local;    /**< @brief If true, using localtime, else using UTC. */
    };

    /** @brief Diff between t1 and t2.
     * If t2 > t1, it will return 0 (no negative time).
     */
    SystemTime operator-(const SystemTime& t1, const SystemTime& t2);
}

/** @brief Print to an ostream using t.format(). */
std::ostream& operator<<(std::ostream& os, const core::SystemTime& t);

#endif

