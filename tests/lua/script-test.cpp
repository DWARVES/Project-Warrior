
#include "lua/script.hpp"
#include "core/logger.hpp"
#include <iostream>

int main()
{
    core::logger::init();
    core::logger::addOutput(&std::cout);
    core::logger::logm("Begin of the program", core::logger::MSG);

    try {
        /* Creating and loading */
        core::logger::logm("Creating lua script object.", core::logger::MSG);
        lua::Script scr;

        core::logger::logm("Loading lua script.", core::logger::MSG);
        if(!scr.load("script.lua")) {
            core::logger::logm("Couldn't load s.lua.", core::logger::ERROR);
            throw lua::exception("Script couldn't be loaded.");
        }

        /* Variables methods */
        std::cout << "Type of \"var\" variable : " << scr.typeVariable("var") << std::endl;;
        std::cout << "Type of \"num\" variable : " << scr.typeVariable("num") << std::endl;;
        std::cout << "Value of \"var\" variable : " << scr.getVariable<std::string>("var") << std::endl;
        std::cout << "Value of \"num\" variable : " << scr.getVariable<bool>("num") << std::endl;
        scr.setVariable("num", "Hello world !");
        std::cout << "Value of \"num\" variable : " << scr.getVariable<std::string>("num") << std::endl;
        scr.setVariable("num", 3.14);
        std::cout << "Value of \"num\" variable : " << scr.getVariable<double>("num") << std::endl;

        /* Function methods */
        core::logger::logm("Testing and calling functions.", core::logger::MSG);
        if(!scr.existsFunction("un")) {
            std::cout << "Function un doesn't exists." << std::endl;
        }
        if(scr.existsFunction("echo")) {
            /* Must use explicit template types for conversion from const char* to std::string */
            scr.callFunction<int, std::string, std::string, int>("echo", false, "Hello world", "Second line", 27);
        }
        if(scr.existsFunction("add")) {
            std::cout << "Ret value for add : " << scr.callFunction<float>("add", true, 1.f, 2.5f, 50.f) << std::endl;
        }
        scr.callFunction<int>("unexists", false); /* Try call of unexistent function */
    }
    catch(const lua::exception& e) {
        core::logger::logm(std::string("Lua exception catched : ") + e.what(), core::logger::FATAL);
        return 1;
    }
    catch(const std::exception& e) {
        core::logger::logm(std::string("Standart exception catched : ") + e.what(), core::logger::FATAL);
        return 1;
    }
    catch(...) {
        core::logger::logm("Unknown exception catched.", core::logger::FATAL);
        return 1;
    }

    core::logger::free();
    return 0;
}

