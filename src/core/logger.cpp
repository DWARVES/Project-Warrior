
#include "core/logger.hpp"
#include "core/systemtime.hpp"
#include <vector>
#include <sstream>

namespace core
{
    namespace logger
    {
        /* Global variables definitions */
        /** @brief Describe an output for logging. */
        struct _output {
            std::ostream* os; /**< @brief The pointer to the output. */
            bool todel;       /**< @brief Indicate if the output must be free'd. **/
        };
        /** @brief Vector storing all the outputs. */
        static std::vector<_output> outputs;
        /** @brief The actual block. */
        static unsigned int blocks;
        /** @brief The minimum level under which nothing must be logged. */
        static Level lvlmin;
        /** @brief Indicate if the logging has been initialized.
         * If false, nothing will be logged.
         */
        static bool initializated = false;

        void init()
        {
            blocks = 0;
            lvlmin = MSG;
            initializated = true;
        }

        void free()
        {
            if(!initializated)
                return;

            for(size_t i = 0; i < outputs.size(); ++i) {
                if(outputs[i].todel)
                    delete outputs[i].os;
            }
            initializated = false;
        }

        void addOutput(std::ostream* os, bool todel)
        {
            if(!initializated)
                return;

            _output op;
            op.os = os;
            op.todel = todel;
            outputs.push_back(op);
        }

        void pushBlock()
        {
            if(!initializated)
                return;
            ++blocks;
        }

        void popBlock()
        {
            if(!initializated)
                return;
            if(blocks > 0)
                --blocks;
        }

        void minLevel(Level min)
        {
            lvlmin = min;
        }

        void logmsg(const std::string& file, unsigned int line, const std::string& msg, Level lvl, bool date)
        {
            if(!initializated || lvl < lvlmin)
                return;

            std::ostringstream whole; /* Complete msg (with date, blocks and level indication */

            if(date) {
                SystemTime st; /* Constructor to local time */
                whole << "[" << st.format("%m/%d/%Y %H:%M:%S", 20) << "] ";
            }

            switch(lvl) {
                case MSG:
                    whole << "        ";
                    break;
                case WARNING:
                    whole << "  /!\\   ";
                    break;
                case ERROR:
                    whole << "  (E)   ";
                    break;
                case SERIOUS:
                    whole << " (!E!)  ";
                    break;
                case FATAL:
                    whole << "//!E!\\\\ ";
                    break;
                default: /* Shouldn't happen, avoid warnings */
                    break;
            }

            for(size_t i = 0; i < blocks; ++i)
                whole << ">";
            whole << " " << msg << "\t[" << file << ":" << line << "] ";

            for(size_t i = 0; i < outputs.size(); ++i) {
                (*outputs[i].os) << whole.str() << std::endl;
            }
        }

    }
}


