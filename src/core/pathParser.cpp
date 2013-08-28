
#include "core/pathParser.hpp"
#include "config.h"
#include <cstring>

#ifdef HAVE_SYSSTAT_H
    #include <sys/types.h>
    #include <sys/stat.h>
#else
    #error "sys/stat.h is needed."
#endif

#ifdef HAVE_DIRENT_H
    #include <dirent.h>
#else
    #error "dirent.h is needed."
#endif

namespace core
{
    namespace path
    {
        std::string cleanSep(const std::string& p)
		{
            std::string np; /* New path */
            bool sep = false; /* Is the previous letter a '/' */

            for(size_t i = 0; i < p.size(); ++i) {
                if(p[i] == '/') {
                    if(!sep) {
                        sep = true;
                        np.push_back(p[i]);
                    }
                }
                else {
                    sep = false;
                    np.push_back(p[i]);
                }
            }

            return np;
		}

        std::vector<std::string> parts(const std::string& p)
		{
            std::vector<std::string> vparts;
            std::string apart; /* A part of the path */

            for(size_t i = 0; i < p.size(); ++i) {
                if(p[i] == '/') {
                    if(!apart.empty()) {
                        vparts.push_back(apart);
                        apart.clear();
                    }
                }
                else {
                    apart.push_back(p[i]);
                }
            }

            if(p.back() != '/' && !apart.empty()) {
                vparts.push_back(apart);
            }

            return vparts;
		}

        std::string extension(const std::string& p)
		{
            std::string ext;
            bool ended = false; /* Found a '.' ? */
            std::string::const_reverse_iterator it = p.crbegin();

            /* We skips the finals '/' */
            while(it != p.crend() && *it == '/') {
                ++it;
            }

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
            while(it != p.crend() && *it == '/') {
                ++it;
            }
            
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

        bool absolute(const std::string& p)
		{
            size_t idx = 0;

            while(idx < p.size() && p[idx] == ' ') {
                ++idx;
            }

            if(idx == p.size() || p[idx] != '/') {
                return false;
            }
            else {
                return true;
            }
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

