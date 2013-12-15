
#include "i18n.hpp"

#ifdef HAVE_LIBINTL_H
#  include <locale.h>
#  include <sstream>
#  include "core/logger.hpp"
#endif

namespace core
{
    bool initLocale()
    {
#ifdef HAVE_LIBINTL_H
        char* loc = setlocale(LC_ALL, "");
        if(!loc) {
            logger::logm("Couldn't load the locale.", logger::ERROR);
            return false;
        }
        else {
            std::ostringstream oss;
            oss << "Locale loaded : \"" << loc << "\".";
            logger::logm(oss.str(), logger::MSG);
        }

        /** @todo system independant path. */
        loc = bindtextdomain("warrior", "/usr/share/locale");
        if(!loc) {
            logger::logm("Couldn't bind the locale domain.", logger::ERROR);
            return false;
        }
        else {
            std::ostringstream oss;
            oss << "Locale path setted to : \"" << loc << "\".";
            logger::logm(oss.str(), logger::MSG);
        }

        loc = textdomain("warrior");
        if(!loc) {
            logger::logm("Couldn't get the message domain.", logger::ERROR);
            return false;
        }
        else {
            std::ostringstream oss;
            oss << "Message domain setted to : \"" << loc << "\".";
            logger::logm(oss.str(), logger::MSG);
        }
        return true;
#else
        /* If no support for translation, no need to load the locale. */
        logger::logm("Compiled witheout locale support.", logger::MSG);
        return true;
#endif
    }
}


