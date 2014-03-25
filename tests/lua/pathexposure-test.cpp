
#include "gameplay/save.hpp"
#include "core/logger.hpp"
#include "lua/script.hpp"
#include "lua/pathExposure.hpp"
#include "lua/exception.hpp"
#include <iostream>
#include <exception>

int main()
{
    try {
        /* Initializing */
        core::logger::init();
        core::logger::addOutput(&std::cout);
        lua::Script scr;
        lua::exposure::Path::expose(&scr);

        /* Load scripts */
        if(!scr.load("scr.lua"))
            throw lua::program_exception("Couldn't load scr.lua.");

        /* Freeing everything */
        core::logger::free();
    }
    catch(const std::exception& e) {
        std::cout << "An exception was caught : " << e.what() << std::endl;
    }
    catch(...) {
        std::cout << "An unknown exception was caught." << std::endl;
    }

    return 0;
}

