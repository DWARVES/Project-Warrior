
#ifndef DEF_CORE_CONFIG
#define DEF_CORE_CONFIG

#include "core/fakefs.hpp"
#include "core/logger.hpp"
#include <sstream>
#include <string>
#include <iostream>
#include <unordered_map>
#include <exception>

namespace core
{
    /** @brief Exception used in core::Config.
     *
     * Represent an error when trying accessing an option inexistant.
     */
    class ConfigBadKeyException : public std::exception
    {
        public:
            /** @brief Only constructor.
             * @param name The name of the unexitant option the user tryed to access.
             */
            ConfigBadKeyException(const char* name) noexcept;
            ConfigBadKeyException() = delete;
            virtual ~ConfigBadKeyException();
            /** @brief Return a generic error message. */
            virtual const char* what() const noexcept;
            const char* name() const noexcept;

        private:
            const char* m_name;
    };

    /** @brief Class storing and loading program-scale options. */
    class Config
    {
        public:
            Config();
            Config(const Config&) = delete;
            ~Config();

            /** @brief Parses the command-lines arguments.
             * @return False if an unhandled option is found.
             */
            bool args(int argc, char *argv[]);
            /** @brief Parses a file with a fakefs syntax.
             * @return False if an unhandled option is found.
             */
            bool cfg(const std::string& path);

            /** @brief Define a new option for the program. 
             * @tparam The type of this option. There must be an overload of the function operator<<(std::ostream&, T).
             * @param lname The long name of the option.
             * @param sname The one-letter name of the option. If you don't want one, set it to 0.
             * @param description The description of the option, used when printing the help message.
             * @param value The default value of the option.
             * @return False if couldn't add the option, generally because either lname or sname is already used.
             */
            template <typename T> bool define(const std::string& lname, char sname, const std::string& description, T value);
            /** @brief Prints an help message to an ostream.
             * It uses the description of each value, and does not show the default values.
             */
            void help(std::ostream& os);
            /** @brief Prints the loaded options to an ostream.
             * It is only aimed for debug purpose.
             */
            void dump(std::ostream& os);

            /** @brief Get the value of an option.
             * @tparam The type of the option. The must be an overload of operator>>(std::istream&, T).
             * @param name The name of the option. If the option doesn't exists, a core::ConfigBadKeyException will be thrown.
             */
            template <typename T> T get(const std::string& name);

        private:
            std::unordered_map<char,std::string> m_shorts; /**< @brief A map associating the shorts name to the long ones. */
            FakeFS<std::string> m_fs; /**< @brief The structure storing the options. */
            /** @brief Store data about an option. */
            struct Info {
                char s;        /**< @brief The short name of the option. */
                std::string d; /**< @brief The description of the option. */
            };
            std::unordered_map<std::string,Info> m_descs; /**< @brief A map associating the long names to data about the option. */
    };

    /* Defining template methods */
    template <typename T> bool Config::define(const std::string& lname, char sname, const std::string& description, T value)
    {
        /* sname already used */
        if(sname != 0 && m_shorts.find(sname) != m_shorts.end()) {
            std::ostringstream oss;
            oss << "Short option name '" << sname << "' already used.";
            logger::logm(oss.str(), logger::WARNING);
            return false;
        }

        /* lname already used */
        if(m_fs.existsEntity(lname)) {
            std::ostringstream oss;
            oss << "Long option name \"" << lname << "\" already used.";
            logger::logm(oss.str(), logger::WARNING);
            return false;
        }

        /* Setting description */
        Info info;
        info.s = sname;
        info.d = description;
        m_descs[lname] = info;

        /* Storing the option and its default value */
        std::ostringstream oss;
        oss << value;
        if(!m_fs.createEntity(lname, oss.str())) {
            std::ostringstream os;
            os << "Couldn't add \"" << lname << "\" option.";
            logger::logm(os.str(), logger::WARNING);
            return false;
        }
        else {
            /* Storing the short name */
            m_shorts[sname] = lname;
            return true;
        }
    }
            
    template <typename T> T Config::get(const std::string& name)
    {
        if(!m_fs.existsEntity(name)) {
            std::ostringstream oss;
            oss << "Tryed to get the value of unregistered \"" << name << "\" option.";
            logger::logm(oss.str(), logger::WARNING);
            throw ConfigBadKeyException(name.c_str());
        }
        
        std::istringstream iss( m_fs.getEntityValue(name) );
        T value;
        iss >> value;
        return value;
    }
}

#endif

