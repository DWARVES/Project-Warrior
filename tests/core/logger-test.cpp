
#include <iostream>
#include <fstream>
#include "core/logger.hpp"

int main()
{
    core::logger::init();
    core::logger::addOutput(&std::cout);
    std::ofstream* ofs = new std::ofstream("log");
    core::logger::addOutput(ofs, true);

    core::logger::minLevel(core::logger::MSG);
    core::logger::log("Begginig with minimal level : MSG", core::logger::MSG);
    core::logger::pushBlock();
    core::logger::log("But be careful with memory management.", core::logger::WARNING);
    core::logger::pushBlock();
    core::logger::log("Invalid write of size 32.", core::logger::ERROR);
    core::logger::popBlock();
    core::logger::log("Couldn't free most of the used memory.", core::logger::SERIOUS);
    core::logger::popBlock();
    core::logger::log("Segfault.", core::logger::FATAL);

    core::logger::minLevel(core::logger::SERIOUS);
    core::logger::log("Begginig with minimal level : MSG", core::logger::MSG);
    core::logger::pushBlock();
    core::logger::log("But be careful with memory management.", core::logger::WARNING);
    core::logger::pushBlock();
    core::logger::log("Invalid write of size 32.", core::logger::ERROR);
    core::logger::popBlock();
    core::logger::log("Couldn't free most of the used memory.", core::logger::SERIOUS);
    core::logger::popBlock();
    core::logger::log("Segfault.", core::logger::FATAL);

    core::logger::free();
    return 0;
}

