
#ifndef DEF_CORE_SYSTEMTIME
#define DEF_CORE_SYSTEMTIME

#include <ctime>
#include <string>
#include <ostream>

namespace core
{
    class SystemTime
    {
        public:
            SystemTime(); /* Will use the system time */
            SystemTime(const SystemTime& cp);
            SystemTime& operator=(const SystemTime& cp);
            ~SystemTime();

            /* Set own timestamp
             * If utc, then the time will be the UTC's, else it will be local's
             */
            void set(const time_t& seconds, bool utc = false);
            /* Set own date */
            void set(int year,     /* Years since JC 0-? */
                    int month,     /* Months since January 9-12 */
                    int day,       /* Day of the month 1-31 */
                    int hour = 0,  /* Hours since midnight 0-23 */
                    int min = 0,   /* Minutes after the hour 0-59 */
                    int sec = 0    /* Seconds after the minute 0-59 */
                    );
            /* Use the system time */
            void useSystemTime();
            /* Use UTC time */
            void useUTCTime();

            enum TimePart {
                YEAR,      /* Year since JC 0-?*/
                LYEAR,     /* Year since 1900 -1970-? */
                MDAY,      /* Day of the month 1-31 */
                WDAY,      /* Day of the week 0-6 */
                YDAY,      /* Day since jan 1 0-365 */
                MONTH,     /* Month since january 0-11 */
                HOUR,      /* Hours since midnight 0-23 */
                MINUTE,    /* Minutes after the hour 0-59 */
                SECONDS,   /* Seconds after the minute 0-59 */
            };
            /* Get one of the time parts */
            int get(TimePart part) const;

            /* Return date string with default format ex : Sun Feb 27 21:39:40 2011 */
            std::string format() const;
            /* Create your own format 
             * fmt has the same syntax as strftime from time.h
             * Return length can never exceed maxSize characters
             */
            std::string format(const std::string& format, size_t maxSize) const;

            /* Diff between t and this
             * If t > this, this will be set to 0 (no negative time)
             */
            SystemTime& operator-=(const SystemTime& t);

        private:
            struct tm* m_tm;
            bool m_local;
    };

    /* Diff between t1 and t2
     * If t2 > t1, it will return 0 (no negative time)
     */
    SystemTime operator-(const SystemTime& t1, const SystemTime& t2);
}

/* Print to an ostream using t.format() */
std::ostream& operator<<(std::ostream& os, const core::SystemTime& t);

#endif

