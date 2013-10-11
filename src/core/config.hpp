
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
    class ConfigBadKeyException : public std::exception
    {
        public:
            ConfigBadKeyException(const char* name) noexcept;
            ConfigBadKeyException() = delete;
            virtual ~ConfigBadKeyException();
            virtual const char* what() const noexcept;
            const char* name() const noexcept;

        private:
            const char* m_name;
    };

    class Config
    {
        public:
            Config();
            Config(const Config&) = delete;
            ~Config();

            /* Parses the arguments, returns false if error
             * Returns false if an unhandled option is found
             */
            bool args(int argc, char *argv[]);
            /* Parses a file, returns false if error
             * Returns false if an unhandled option is found
             */
            bool cfg(const std::string& path);

            /* Returns false if either lname or sname is already used
             * If you don't want a sname, put it to 0
             * value is the default value, converted to string with ostringstream
             * For non-standart types, overload operator<<(std::ostream&, Y)
             */
            template <typename T> bool define(const std::string& lname, char sname, const std::string& description, T value);
            /* Prints help to the ostream */
            void help(std::ostream& os);
            /* Prints the loaded options to the ostream */
            void dump(std::ostream& os);

            /* Get the value of an option, converted with istringstream
             * For non standart types, overload operator>>(std::istream&, T)
             * If name is not the name of an option, it will throw a ConfigBadKeyException
             */
            template <typename T> T get(const std::string& name);

        private:
            std::unordered_map<char,std::string> m_shorts;
            FakeFS<std::string> m_fs;
            /* For descriptions */
            struct Info {
                char s;
                std::string d;
            };
            std::unordered_map<std::string,Info> m_descs;
    };

    /* Defining template mathods */
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

