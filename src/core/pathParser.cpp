
#include "core/pathParser.hpp"
#include "config.h"
#include <cstring>
#include <algorithm>
#include <sstream>

#ifdef HAVE_SYSSTAT_H
#  include <sys/types.h>
#  include <sys/stat.h>
#else
#  error "sys/stat.h is needed."
#endif

#ifdef HAVE_DIRENT_H
#  include <dirent.h>
#else
#  error "dirent.h is needed."
#endif

namespace core
{
    namespace path
    {
        std::string cleanSep(const std::string& p)
        {
            std::string np = p; /* New path */
            auto it = std::unique_copy(p.begin(), p.end(), np.begin(),
                    [] (char l, char n) { return (l == n) && (l == '/'); } );
            np.resize( std::distance(np.begin(), it) );
            return np;
        }

        std::vector<std::string> parts(const std::string& p)
        {
            std::vector<std::string> vparts;
            std::istringstream iss(p);
            std::string part;
            while(std::getline(iss,part,'/')) {
                if(!part.empty())
                    vparts.push_back(part);
            }

            return vparts;
        }

        std::string extension(const std::string& p)
        {
            std::string ext;
            bool ended = false; /* Found a '.' ? */
            std::string::const_reverse_iterator it = p.crbegin();

            /* We skips the finals '/' */
            it = std::find_if_not(it, p.crend(), [] (char c) { return c == '/'; });

            /* We find the extension */
            for(; it != p.crend(); ++it) {
                if(*it == '.') {
                    ended = true;
                    break;
                }
                else if(*it == '/') {
                    break;
                }
                else {
                    ext.insert(0, 1, *it);
                }
            }

            if(!ended) {
                ext.clear();
            }

            return ext;
        }

        std::string head(const std::string& p, bool ext)
        {
            std::string hd; /* The head */
            std::string::const_reverse_iterator it = p.crbegin();

            /* We skips the finals '/' */
            it = std::find_if_not(it, p.crend(), [] (char c) { return c == '/'; });

            /* We get the head */
            for(; it != p.crend(); ++it) {
                if(*it == '/') {
                    break;
                }
                else if(!ext && *it == '.') {
                    hd.clear();
                }
                else {
                    hd.insert(0, 1, *it);
                }
            }

            return hd;
        }
        
        std::string body(const std::string& p, bool tdir)
        {
            /* If it's a directory and tdir is set, no transformations needed */
            if(tdir && type(p) == Type::Dir)
                return p;

            std::string body;
            bool inbody = false; /* Is parsing the body ? */
            std::string::const_reverse_iterator it = p.crbegin();

            /* We skips the finals '/' */
            it = std::find_if_not(it, p.crend(), [] (char c) { return c == '/'; });

            /* We get the body */
            for(; it != p.crend(); ++it) {
                /* The body is before the last '/' */
                if(!inbody && *it == '/')
                    inbody = true;
                else if(inbody)
                    body.insert(0, 1, *it);
            }

            return body;

        }

        bool absolute(const std::string& p)
        {
            size_t idx = 0;

            while(idx < p.size() && p[idx] == ' ')
                ++idx;

            if(idx == p.size() || p[idx] != '/')
                return false;
            else
                return true;
        }

        Type type(const std::string& p)
        {
#ifdef HAVE_SYSSTAT_H
            struct stat buffer;

            if(stat(p.c_str(), &buffer) < 0) {
                if(errno == ENOENT || errno == ENOTDIR)
                    return Type::Unexistant;
                else
                    return Type::Unknown;
            }

            if(S_ISREG(buffer.st_mode))
                return Type::Reg;
            else if(S_ISDIR(buffer.st_mode))
                return Type::Dir;
            else if(S_ISSOCK(buffer.st_mode))
                return Type::Socket;
            else
                return Type::Unknown;
#endif//HAVE_SYSSTAT_H
        }

        std::vector<std::string> dirContents(const std::string& dir)
        {
#ifdef HAVE_DIRENT_H
            std::vector<std::string> contents;

            DIR* d = opendir(dir.c_str());
            if(d == NULL)
                return contents;
            struct dirent* ent = NULL;

            while((ent = readdir(d)) != NULL) {
                if(strcmp(ent->d_name, ".") != 0
                        && strcmp(ent->d_name, "..") != 0)
                    contents.push_back(ent->d_name);
            }

            closedir(d);
            return contents;
#endif//HAVE_DIRENT_H
        }
    }
}

