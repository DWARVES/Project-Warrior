
#ifndef DEF_CORE_LOGGER
#define DEF_CORE_LOGGER

#include <ostream>
#include <string>

namespace core
{
    /** @brief Contain all methods related to logging. */
    namespace logger
    {
        /** @brief Init logging, must be called only once, before all other calls. */
        void init();
        /** @brief Free everything that was used by logging. */
        void free();

        /** @brief Add an output.
         * @param os A pointer to the output, which must inheritate from std::ostream.
         * @param todel If true, the ostream will be delete'd.
         */
        void addOutput(std::ostream* os, bool todel = false);

        /** @brief Enter a new block : add a '>' at the beggining of the line */
        void pushBlock();
        /** @brief Get out of the actual block. */
        void popBlock();

        /** @brief Differents levels of importance of messages. */
        enum Level {
            MSG,     /**< @brief Simple info */
            WARNING, /**< @brief Warning */
            ERROR,   /**< @brief Error not fatal, will not really matter the execution*/
            SERIOUS, /**< @brief Error not fatal, but will disturb the execution */
            FATAL    /**< @brief Fatal error */
        };
        /** @brief Set a minimum level of importance.
         * From now on, only messages with at least this level will be printed.
         */
        void minLevel(Level min);

        /** @brief Log a message to all the outputs setted.
         * @param file The source code file from which this function is called.
         * @param line The line in file of the call to this function.
         * @param msg The message to log. Must be only one line.
         * @param lvl The level of the message. If inferior to the minimum level, nothing will be logged.
         * @param date If true, the date in iso format will be printed.
         */
        void logmsg(const std::string& file, unsigned int line, const std::string& msg, Level lvl, bool date = true);

        /* Macros helping logmsg use. */
#define logd(msg, lvl, date) logmsg(__FILE__, __LINE__, msg, lvl, date)
#define logm(msg, lvl)        logmsg(__FILE__, __LINE__, msg, lvl)
    }
}

#endif

