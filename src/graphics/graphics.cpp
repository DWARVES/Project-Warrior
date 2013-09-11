
#include "graphics/graphics.hpp"
#include "core/logger.hpp"
#include <sstream>

namespace graphics
{
    Graphics::Graphics()
        : m_win(NULL), m_ctx(0), m_virtualW(0), m_virtualH(0), m_virtualR(false)
    {}

    Graphics::~Graphics()
    {
        closeWindow();
    }

    /*************************
     *  Window manipulation  *
     *************************/

    bool Graphics::openWindow(const std::string& name, int w, int h)
    {
        m_win = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_OPENGL);
        logWindow(false, m_win != NULL, m_win == NULL);

        if(m_win != NULL) {
            bool ctx = glContext();
            if(ctx)
                disableVirtualSize();
            else
                SDL_DestroyWindow(m_win);
            return ctx;
        }
        else
            return false;
    }

    bool Graphics::openFullscreenWindow(const std::string& name, int minw, int minh)
    {
        m_win = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);

        if(!m_win) {
            logWindow(true, false, true);
            return false;
        }
        else if(m_win && (windowWidth() < minw
                    ||   windowHeight() < minh)) {
            SDL_DestroyWindow(m_win);
            m_win = NULL;
            logWindow(true, false);
            return false;
        }
        else {
            bool ctx = glContext();
            logWindow(true, ctx, !ctx);
            if(ctx)
                disableVirtualSize();
            else
                SDL_DestroyWindow(m_win);
            return ctx;
        }
    }

    void Graphics::closeWindow()
    {
        if(m_win)
        {
            core::logger::logm("Destroying the window.", core::logger::MSG);
            if(m_ctx)
                SDL_GL_DeleteContext(m_ctx);
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

    bool Graphics::glContext()
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        m_ctx = SDL_GL_CreateContext(m_win);
        return m_ctx != 0;
    }

    /*************************
     *     Virtual size      *
     *************************/
    void Graphics::setVirtualSize(int w, int h)
    {
        m_virtualW = w;
        m_virtualH = h;

        /* Logging */
        std::ostringstream oss;
        oss << "Virtual graphic size setted to " << w << "x" << h << ".";
        core::logger::logm(oss.str(), core::logger::MSG);
    }

    void Graphics::disableVirtualSize()
    {
        setVirtualSize(windowWidth(), windowHeight());
    }

    void Graphics::preserveRatio(bool r)
    {
        m_virtualR = r;
        /* Logging */
        std::ostringstream oss;
        oss << "Ratio preserved : [" << (r ? "on" : "off") << "].";
        core::logger::logm(oss.str(), core::logger::MSG);
    }

    bool Graphics::preserveRatio() const
    {
        return m_virtualR;
    }

    int Graphics::getVirtualWidth() const
    {
        return m_virtualW;
    }

    int Graphics::getVirtualHeight() const
    {
        return m_virtualH;
    }

    bool Graphics::isEnabled() const
    {
        if(windowWidth() != getVirtualWidth()
                || windowHeight() != getVirtualHeight())
            return false;
        else
            return true;
    }

    /*************************
     * Namespaces management *
     *************************/
    bool Graphics::enterNamespace(const std::string& name)
    {
        return m_fs.enterNamespace(name);
    }

    bool Graphics::createNamespace(const std::string& name)
    {
        return m_fs.createNamespace(name);
    }

    void Graphics::deleteNamespace(const std::string& name)
    {
        m_fs.deleteNamespace(name);
    }

    std::string Graphics::actualNamespace() const
    {
        return m_fs.actualNamespace();
    }

    void Graphics::EntityLiberator::operator()(Entity* tofree) const
    {
        if(!tofree)
            return;

        switch(tofree->type) {
            case TEXT:
                if(tofree->stored.text)  delete tofree->stored.text;
                break;
            case MOVIE:
                if(tofree->stored.movie) delete tofree->stored.movie;
                break;
            case FONT:
                if(tofree->stored.font)  delete tofree->stored.font;
                break;
            case NONE:
            default:
                /* Invalid entity, shouldn't happen */
                break;
        }
        delete tofree;
    }

    /*************************
     *  Ressources loading   *
     *************************/

    bool Graphics::loadTexture(const std::string& name, const std::string& path)
    {
        if(m_fs.existsEntity(name))
            return false;

        internal::Texture* text = new internal::Texture;
        if(!text->load(path)) {
            delete text;
            return false;
        }

        Entity* ent = new Entity;
        ent->type = TEXT;
        ent->stored.text = text;
        return true;
    }

    bool Graphics::loadMovie(const std::string& name, const std::string& path)
    {
        if(m_fs.existsEntity(name))
            return false;

        internal::Movie* mov = new internal::Movie;
        if(!mov->load(path)) {
            delete mov;
            return false;
        }

        Entity* ent = new Entity;
        ent->type = MOVIE;
        ent->stored.movie = mov;
        return true;
    }

    bool Graphics::loadFont(const std::string& name, const std::string& path)
    {
        if(m_fs.existsEntity(name))
            return false;

        internal::Font* font = new internal::Font;
        if(!font->load(path)) {
            delete font;
            return false;
        }

        Entity* ent = new Entity;
        ent->type = FONT;
        ent->stored.font = font;
        return true;
    }

    bool Graphics::loadTextureFromText(const std::string& name, const std::string& font, const std::string& txt)
    {
        /* TODO */
        return !name.empty() && !font.empty() && !txt.empty();
    }

    void Graphics::free(const std::string& name)
    {
        m_fs.deleteEntity(name);
    }

    Graphics::RcType Graphics::rctype(const std::string& name) const
    {
        if(!m_fs.existsEntity(name))
            return NONE;
        Entity* ent = m_fs.getEntityValue(name);
        return ent->type;
    }

    bool Graphics::link(const std::string& name, const std::string& target)
    {
        return m_fs.link(name, target);
    }

    /*************************
     *  Textures management  *
     *************************/
    int Graphics::getTextureWidth(const std::string& name) const
    {
        if(rctype(name) != TEXT)
            return 0;
        else
            return m_fs.getEntityValue(name)->stored.text->width();
    }

    int Graphics::getTextureHeight(const std::string& name) const
    {
        if(rctype(name) != TEXT)
            return 0;
        else
            return m_fs.getEntityValue(name)->stored.text->height();
    }

    bool Graphics::setTextureHotpoint(const std::string& name, int x, int y)
    {
        if(rctype(name) != TEXT)
            return false;
        Entity* ent = m_fs.getEntityValue(name);
        ent->stored.text->hotpoint(geometry::Point((float)x,(float)y));
        return true;
    }
}
