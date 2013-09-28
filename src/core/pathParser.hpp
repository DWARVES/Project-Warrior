
#ifndef DEF_CORE_PATHPARSER
#define DEF_CORE_PATHPARSER

#include <string>
#include <vector>

namespace core
{
    /* All the functions works only for unix-like path */
    namespace path
    {
        /* Remove doubles '/' */
        std::string cleanSep(const std::string& p);

        /* Lists parts : '/home/fred/file' -> ('home' 'fred' 'file') */
        std::vector<std::string> parts(const std::string& p);

        /* Return extensions : 'file.txt' -> 'txt' */
        std::string extension(const std::string& p);

        /* Return head : '/home/fred/file.txt' -> 'file.txt'
         * If ext = false, remove the extension (if any)
         */
        std::string head(const std::string& p, bool ext = true);

        /* Return body : '/home/fred/file.txt' -> '/home/fred'
         * If tdir, will remove the head only if it's not a directory
         */
        std::string body(const std::string& p, bool tdir = false);

        /* Is the path absolute, could have been named 'hasRoot' */
        bool absolute(const std::string& p);

        /* Type of the path */
        enum class Type {
            Reg,        /* Normal file */
            Dir,        /* Directory */
            Socket,     /* Socket */
            Unknown,    /* Other */
            Unexistant  /* The file does not exists */
        };
        Type type(const std::string& p);

        /* List a directory contents
         * It does NOT include '.' and '..'
         */
        std::vector<std::string> dirContents(const std::string& dir);
    }
}

#endif

