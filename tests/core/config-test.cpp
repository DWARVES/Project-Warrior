
#include <iostream>
#include "core/config.hpp"
#include "core/logger.hpp"

int main(int argc, char *argv[])
{
    core::logger::init();
    core::logger::addOutput(&std::cout);

    core::Config cfg;
    cfg.define("hello", 'W', "Display an hello message.", false);
    cfg.define("cfg", 'c', "Indicate a config file to use.", "rc");
    cfg.define("nocfg", 'N', "Prevent the program from loading a config file.", false);
    cfg.define("help", 'H', "Display help.", false);
    cfg.define("color", 'C', "Background color.", "0xFFFFFF");
    cfg.define("fps", 'f', "FPS of the game.", 60);
    cfg.define("width", 'w', "Width of the window.", 800);
    cfg.define("height", 'h', "Height of the window.", 600);
    cfg.define("ratio", 'r', "Ratio used ingame.", 1.3333f);
    cfg.define("fullscreen", 'F', "Play fullscreen.", true);

    if(!cfg.args(argc, argv))
        return 1;
    if(!cfg.get<bool>("nocfg")) {
        if(!cfg.cfg(cfg.get<std::string>("cfg")))
            return 1;
    }

    if(cfg.get<bool>("hello")) {
        std::cout << ">>>> DUMP <<<<" << std::endl;
        cfg.dump(std::cout);
    }
    if(cfg.get<bool>("help")) {
        std::cout << "\n>>>> HELP <<<<" << std::endl;
        cfg.help(std::cout);
    }

    core::logger::free();
    return 0;
}

