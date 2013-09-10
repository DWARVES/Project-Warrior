
#include "events/save.hpp"
#include "core/logger.hpp"
#include "lua/script.hpp"
#include "lua/saveExposure.hpp"
#include "lua/exception.hpp"
#include <iostream>
#include <exception>

int main()
{
    try {
        /* Initializing */
        core::logger::init();
        core::logger::addOutput(&std::cout);
        events::Save::init();
        events::Save sav;
        sav.createNamespace("ev1");
        sav.createNamespace("ev2");
        lua::Script ev1, ev2;

        /* Exposing save */
        lua::exposure::Save::expose(&ev1);
        lua::exposure::Save::expose(&ev2);

        /* Load scripts */
        if(!ev1.load("ev1.lua"))
            throw lua::program_exception("Couldn't load ev1.lua.");
        if(!ev2.load("ev2.lua"))
            throw lua::program_exception("Couldn't load ev2.lua.");

        /* Process each script */
        sav.enterNamespace("/ev1");
        ev1.callFunction<void>("process", NULL);
        sav.enterNamespace("/ev2");
        ev2.callFunction<void>("process", NULL);

        /* Save to save.fake-fs */
        sav.save("save.fake-fs");

        /* Freeing everything */
        core::logger::free();
        events::Save::quit();
    }
    catch(const std::exception& e) {
        std::cout << "An exception was caught : " << e.what() << std::endl;
    }
    catch(...) {
        std::cout << "An unknown exception was caught." << std::endl;
    }

    return 0;
}

