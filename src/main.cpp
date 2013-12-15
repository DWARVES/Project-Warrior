
#include <iostream>
#include <exception>
#include <sstream>
#include "global.hpp"
#include "core/logger.hpp"
#include "core/i18n.hpp"

/** @brief An exception used to report a fatal error while initializing the game. */
class init_exception : public std::exception
{
    public:
        init_exception(const char* msg)
            : m_msg(msg)
        {}

        const char* what() const noexcept {
            return m_msg;
        }

    private:
        const char* m_msg;
};

/** @brief Load the locale. */
void loadLocale();
/** @brief Parses the command line argument and config file. */
void loadConfig(int argc, char *argv[]);
/** @brief Load the SDL and init the graphics. */
void initGraphics();
/** @brief Free the global variables. */
void freeEverything();

int main(int argc, char *argv[])
{
    int retcode = 0;

    /* Init the logger. */
    core::logger::init();
    core::logger::addOutput(&std::cout);

    /* Init everything. */
    try {
        loadLocale();
        loadConfig(argc, argv);
        initGraphics();
    }
    catch(const std::exception& e) {
        std::ostringstream oss;
        oss << "Error during the initialization : \"" << e.what() << "\".";
        core::logger::logm(oss.str(), core::logger::FATAL);
        retcode = 1;
    }

    freeEverything();
    core::logger::free();
    return retcode;
}

void loadConfig(int argc, char *argv[])
{
    global::cfg = new core::Config;

    /* Defines the options. */
    /* Global options */
    global::cfg->define("help", 0, _i("Display an help message and quit."), false);
    global::cfg->define("config", 'c', _i("The path to the config file."), "/etc/warrior.cfg");
    /* Graphics options */
    global::cfg->define("fullscreen", 'F', _i("A boolean indicating if the game must run in fullscreen mode. You can't precise the size : the desktop size will automaticly be used."), true);
    global::cfg->define("resw", 'W', _i("The width of the window in pixels."), 1024);
    global::cfg->define("resh", 'H', _i("The height of the window in pixels."), 768);
    global::cfg->define("guitheme", 'T', _i("The path to the gui theme."), "/usr/share/warrior/guirc");
    global::cfg->define("name", 0, _i("The name of the window."), "Project Warror");

    /* Parses the command line. */
    if(!global::cfg->args(argc, argv))
        throw init_exception("Couldn't parse the command line arguments.");

    /* Parses the config file. */
    std::string path = global::cfg->get<std::string>("config");
    if(!global::cfg->cfg(path)) {
        std::ostringstream oss;
        oss << "Couldn't parse \"" << path << "\" config file.";
        core::logger::logm(oss.str(), core::logger::WARNING);
    }
}

void initGraphics()
{
    /* Init the SDL. */
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
        throw init_exception(SDL_GetError());

    /* Open the window. */
    global::gfx = new graphics::Graphics;
    if(global::cfg->get<bool>("fullscreen")) {
        if(!global::gfx->openFullscreenWindow(global::cfg->get<std::string>("name"), 800, 600))
            throw init_exception("Couldn't open the window.");
    }
    else {
        if(!global::gfx->openWindow(global::cfg->get<std::string>("name"), global::cfg->get<int>("resw"),
                    global::cfg->get<int>("resh")))
            throw init_exception("Couldn't open the window.");
    }
}

void loadLocale()
{
    if(!core::initLocale())
        throw init_exception("Couldn't init the locale.");
}

void freeEverything()
{
    if(global::cfg)
        delete global::cfg;
    if(global::gfx)
        delete global::gfx;
}

