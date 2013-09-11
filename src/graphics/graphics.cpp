
#include "graphics/graphics.hpp"
#include "core/logger.hpp"
#include <sstream>

namespace graphics
{
    Graphics::Graphics()
        : m_win(NULL)
    {}

    Graphics::~Graphics()
    {
        closeWindow();
    }

    bool Graphics::openWindow(const std::string& name, int w, int h)
    {
        m_win = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_OPENGL);
        logWindow(false, m_win != NULL, m_win == NULL);
        return m_win != NULL;
    }

    bool Graphics::openFullscreenWindow(const std::string& name, int minw, int minh)
    {
        m_win = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);

        if(!m_win) {
            logWindow(false, false, true);
            return false;
        }
        else if(m_win && (windowWidth() < minw
                ||   windowHeight() < minh)) {
            SDL_DestroyWindow(m_win);
            m_win = NULL;
            logWindow(false, false);
            return false;
        }
        else {
            logWindow(false, true);
            return true;
        }
    }

    void Graphics::closeWindow()
    {
        if(m_win)
        {
            core::logger::logm("Destroying the window.", core::logger::MSG);
            SDL_DestroyWindow(m_win);
        }
        m_win = NULL;
    }

    int Graphics::windowWidth() const
    {
        if(!m_win)
            return 0;
        int w;
        SDL_GetWindowSize(m_win, &w, NULL);
        return w;
    }

    int Graphics::windowHeight() const
    {
        if(!m_win)
            return 0;
        int h;
        SDL_GetWindowSize(m_win, NULL, &h);
        return h;
    }

    int Graphics::windowDepth() const
    {
        if(!m_win)
            return 0;
        else
            return SDL_GetWindowPixelFormat(m_win);
    }

    bool Graphics::isWindowOpen() const
    {
        return m_win != NULL;
    }
            
    void Graphics::logWindow(bool full, bool ended, bool sdlerr)
    {
        std::ostringstream oss;
        if(!ended)
            oss << "Couldn't create ";
        else
            oss << "Created ";

        if(full)
            oss << "fullscreen ";

        if(!ended)
            oss << "window.";
        else
            oss << "window of " << windowWidth() << "x" << windowHeight() << " size";

        if(!ended && sdlerr)
            oss << " : \"" << SDL_GetError() << "\".";
        else
            oss << ".";

        if(!ended)
            core::logger::logm(oss.str(), core::logger::WARNING);
        else
            core::logger::logm(oss.str(), core::logger::MSG);
    }
}
