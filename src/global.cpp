
#include "global.hpp"
#include "core/i18n.hpp"
#include "gameplay/controler.hpp"
#include "lua/graphicsExposure.hpp"

namespace global
{
    graphics::Graphics* gfx = NULL;
    core::Config* cfg = NULL;
    events::Events* evs = NULL;
    audio::Audio* audio = NULL;
    gui::Gui* gui = NULL;
    gui::Theme* theme = NULL;
            
    init_exception::init_exception(const char* msg) noexcept
        : m_msg(msg)
        {}

    const char* init_exception::what() const noexcept
    {
        return m_msg;
    }

    void loadGui()
    {
        /* Loading the gui. */
        global::gui = new gui::Gui(global::gfx);

        /* Loading the gui theme. */
        global::theme = new gui::Theme(global::gfx, global::cfg->get<std::string>("guitheme"));
        if(!global::theme->load())
            throw init_exception("Couldn't load the gui theme.");
    }

    void loadAudio()
    {
        global::audio = new audio::Audio;
        if(!global::audio->init(global::cfg->get<int>("frequence")))
            throw init_exception("Couldn't initialize the audio.");
        global::audio->musicVolume((unsigned char)global::cfg->get<unsigned int>("music"));
        global::audio->soundsVolume((unsigned char)global::cfg->get<unsigned int>("sounds"));
    }

    void freeEverything()
    {
        if(global::cfg)
            delete global::cfg;
        if(global::gfx)
            delete global::gfx;
        if(global::evs)
            delete global::evs;
        if(global::audio)
            delete global::audio;
        if(global::gui)
            delete global::gui;
        if(global::theme)
            delete global::theme;
        gameplay::Controler::close();
    }

    void loadConfig(int argc, char *argv[])
    {
        global::cfg = new core::Config;

        /* Defines the options. */
        /* Global options */
        global::cfg->define("help",    0,  _i("Display an help message and quit."), false);
        global::cfg->define("config", 'c', _i("The path to the config file."), "warrior.cfg");
        global::cfg->define("rcs",    'r', _i("The path to the ressources directory."), "/usr/share/warrior/rcs");
        /* Graphics options */
        global::cfg->define("fullscreen", 'F', _i("A boolean indicating if the game must run in fullscreen mode. You can't precise the size : the desktop size will automaticly be used."), false);
        global::cfg->define("resw",       'W', _i("The width of the window in pixels."), 1024);
        global::cfg->define("resh",       'H', _i("The height of the window in pixels."), 768);
        global::cfg->define("guitheme",   'T', _i("The path to the gui theme."), "/usr/share/warrior/guirc");
        global::cfg->define("name",        0,  _i("The name of the window."), "Project Warror");
        global::cfg->define("phdebug",     0,  _i("Enable debug draw in the physic engine."), false);
        /* Audio options */
        global::cfg->define("frequence", 0, _i("The frequence of the audio output."), 44100);
        global::cfg->define("sounds",    0, _i("The volume of the sounds, between 0 and 255."), 255);
        global::cfg->define("music",     0, _i("The volume of the music, between 0 and 255."), 200);

        /* Parses the command line. */
        if(!global::cfg->args(argc, argv))
            throw init_exception("Couldn't parse the command line arguments.");

        /* Print the help. */
        if(global::cfg->get<bool>("help")) {
            global::cfg->help(std::cout);
            throw init_exception("Ending after printing help.");
        }

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

        lua::exposure::Graphics::setGraphicsInstance(global::gfx);
    }

    void loadEvents()
    {
        evs = new events::Events;
    }

    void loadLocale()
    {
        if(!core::initLocale())
            throw init_exception("Couldn't init the locale.");
    }
    
    void loadControlers()
    {
        std::string path = cfg->get<std::string>("rcs") + "/controls";
        if(gameplay::Controler::load(path)) {
            std::ostringstream oss;
            oss << "Loaded controlers config file \"" << path << "\".";
            core::logger::logm(oss.str(), core::logger::MSG);
        }
        else {
            std::ostringstream oss;
            oss << "Couldn't load controlers config file \"" << path << "\".";
            core::logger::logm(oss.str(), core::logger::WARNING);
        }
    }

}


