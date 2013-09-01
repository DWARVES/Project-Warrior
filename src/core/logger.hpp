
#ifndef DEF_CORE_LOGGER
#define DEF_CORE_LOGGER

#include <ostream>
#include <string>

namespace core
{
    namespace logger
    {
        /* Init logging, must be called only once */
        void init();
        /* Free everything */
        void free();

        /* Add output
         * If todel, the ostream will be delete'd*/
        void addOutput(std::ostream* os, bool todel = false);

        /* Blocks manipulation : one more block will add a '>' at the beggining of the line */
        void pushBlock();
        void popBlock();

        /* Print a message */
        enum Level {
            MSG,     /* Simple info */
            WARNING, /* Warning */
            ERROR,   /* Error not fatal, will not really matter the execution*/
            SERIOUS, /* Error not fatal, but will disturb the execution */
            FATAL    /* Fatal error */
        };
        /* Level from where to print 
         * ex : if min == ERROR, will print ERROR and FATAL
         */
        void minLevel(Level min);

        /* msg will be displayed to all the outputs given,
         * If lvl < min, msg will not be displayed
         * msg must be only one line
         * if date = false, the date won't be showed
         * file and line indicate the line of the code from where the message is written
         */
        void logmsg(const std::string& file, unsigned int line, const std::string& msg, Level lvl, bool date = true);

        /* Log helping macros */
#define logd(msg, lvl, date) logmsg(__FILE__, __LINE__, msg, lvl, date)
#define logm(msg, lvl)        logmsg(__FILE__, __LINE__, msg, lvl)
    }
}

#endif

