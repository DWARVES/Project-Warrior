
#ifndef DEF_CORE_CONFIG
#define DEF_CORE_CONFIG

#include "core/fakefs.hpp"
#include <string>
#include <iostream>
#include <unordered_map>

namespace core
{
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
             */
            template <typename T> T get(const std::string& name);

        private:
            std::unordered_map<char,std::string> m_shorts;
            FakeFS<std::string> m_fs;
            /* For descriptions */
            struct Info {
                char* s;
                std::string d;
            };
            std::unordered_map<std::string,Info> m_descs;
    };
}

#endif

