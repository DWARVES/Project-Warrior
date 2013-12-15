
#ifndef DEF_CORE_I18N
#define DEF_CORE_I18N

#include "config.h"

#ifdef HAVE_LIBINTL_H
#  include <libintl.h>
#  define _i(str) gettext(str)
#else
#  define _i(str) (str)
#endif

namespace core
{
    /** @brief Loads the system locale. */
    bool initLocale();
}

#endif

