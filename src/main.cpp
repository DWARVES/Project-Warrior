
#include <iostream>
#include <exception>
#include <sstream>
#include <fstream>
#include "global.hpp"
#include "core/logger.hpp"
#include "core/i18n.hpp"
#include "menus/mainmenu.hpp"


/** @brief The entry point of the program. */
int main(int argc, char *argv[])
{
    int retcode = 0;
    srand((unsigned int)time(NULL));

    /* Init the logger. */
    std::ofstream ofs("warrior.log");
    core::logger::init();
    core::logger::addOutput(&std::cout);
    if(ofs)
        core::logger::addOutput(&ofs);
    core::logger::logm(">>> Execution started <<<", core::logger::MSG);

    /* Init everything. */
    core::logger::pushBlock();
    core::logger::logm("Initialization.", core::logger::MSG);
    try {
        global::loadLocale();
        global::loadConfig(argc, argv);
        global::initGraphics();
        global::loadEvents();
        global::loadControlers();
    }
    catch(const std::exception& e) {
        std::ostringstream oss;
        oss << "Error during the initialization : \"" << e.what() << "\".";
        core::logger::logm(oss.str(), core::logger::FATAL);
        retcode = 1;
    }
    core::logger::popBlock();

    /* The main loop. */
    /** @todo Separate main loop from main. */
    if(retcode == 0) {
        try {
            MainMenu menu;
            if(!menu.prepare())
                throw global::init_exception("Couldn't prepare the main menu.");
            global::evs->openJoysticks();
            global::evs->enableInput(false);

            while(menu.update())
            {
                global::evs->update();
                if(global::evs->quit() || global::evs->closed())
                    break;

            }
        }
        catch(const std::exception& e) {
            std::ostringstream oss;
            oss << "An exception was thrown during the execution : \"" << e.what() << "\".";
            core::logger::logm(oss.str(), core::logger::FATAL);
            retcode = 1;
        }
        catch(...) {
            core::logger::logm("An unknown exception was thrown during the execution.", core::logger::FATAL);
            retcode = 1;
        }
    }

    /* Free everything. */
    global::freeEverything();
    core::logger::logm(">>> End of the execution <<<", core::logger::MSG);
    core::logger::free();
    SDL_Quit();
    return retcode;
}

