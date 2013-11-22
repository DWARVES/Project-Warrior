
#ifndef DEF_CORE_PATHPARSER
#define DEF_CORE_PATHPARSER

#include <string>
#include <vector>

namespace core
{
    /** @brief Contains path manipulation functions.
     * /!\\ : All the functions only works for unix-like path.
     */
    namespace path
    {
        /** @brief Removes doubles slashs in a path. */
        std::string cleanSep(const std::string& p);

        /** @brief Cut a path into parts. 
         * Example : '/home/fred/file' -> ('home' 'fred' 'file').
         * Double slashs must have been removed for a better output.
         */
        std::vector<std::string> parts(const std::string& p);

        /** @brief Returns the extension of a path : 'file.txt' -> 'txt'. */
        std::string extension(const std::string& p);

        /** @brief Returns the head of a path : '/home/fred/file.txt' -> 'file.txt'.
         * @param ext If false, remove the extension (if any).
         */
        std::string head(const std::string& p, bool ext = true);

        /** @brief Returns the body of a path : '/home/fred/file.txt' -> '/home/fred'.
         * @param tdir If true, will remove the head only if it's not a directory.
         */
        std::string body(const std::string& p, bool tdir = false);

        /** @brief Check if the path is absolute. */
        bool absolute(const std::string& p);

        /** @brief Type of the path. */
        enum class Type {
            Reg,        /**< @brief Normal file */
            Dir,        /**< @brief Directory */
            Socket,     /**< @brief Socket */
            Unknown,    /**< @brief Other */
            Unexistant  /**< @brief The path does not exists */
        };
        /** @brief Indicate the type of the path.
         * @todo Only works under Linux for now, find a more portable implementation.
         */
        Type type(const std::string& p);

        /** @brief List a directory contents.
         * It does NOT include '.' and '..'.
         */
        std::vector<std::string> dirContents(const std::string& dir);
    }
}

#endif

