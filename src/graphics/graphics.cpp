
#include "graphics/graphics.hpp"
#include "core/logger.hpp"
#include <sstream>
#include <cstring>

namespace graphics
{
    /** @brief A very small value, used when comparing floats. */
    const float epsilon = 0.000001f;
    /** @brief Used to convert degres in radians. */
    const float deg2rad = 0.0174532925199433f;

    Graphics::Graphics()
        : m_win(NULL), m_ctx(0), m_shads(&m_exts),
        m_virtualW(0.0f), m_virtualH(0.0f), m_appliedW(0.0f), m_appliedH(0.0f), m_bandWidth(0.0f),
        m_bandLR(true), m_virtualR(false), m_yinvert(false),
        m_lineWidth(1.0f)
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
        logWindow(false, false, m_win != NULL, m_win == NULL);
        internal::Movie::init();

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
        SDL_DisplayMode mode;
        if(SDL_GetDesktopDisplayMode(0, &mode) < 0) {
            core::logger::logm("Couldn't get display mode.", core::logger::WARNING);
            return false;
        }

        m_win = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mode.w, mode.h, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
        internal::Movie::init();

        if(!m_win) {
            logWindow(false, true, false, true);
            return false;
        }
        else if(m_win && (windowWidth() < minw
                    ||   windowHeight() < minh)) {
            SDL_DestroyWindow(m_win);
            m_win = NULL;
            logWindow(false, true, false);
            return false;
        }
        else {
            bool ctx = glContext();
            logWindow(false, true, ctx, !ctx);
            if(ctx)
                disableVirtualSize();
            else {
                SDL_DestroyWindow(m_win);
                m_win = NULL;
            }
            return ctx;
        }
    }

    void Graphics::closeWindow()
    {
        internal::Movie::free();
        if(m_win)
        {
            core::logger::logm("Destroying the window.", core::logger::MSG);
            if(m_ctx)
                SDL_GL_DeleteContext(m_ctx);
            m_ctx = NULL;
            SDL_DestroyWindow(m_win);
            m_fs.clear();
        }
        m_win = NULL;
    }

    bool Graphics::windowSize(int width, int height)
    {
        std::string name = SDL_GetWindowTitle(m_win);
        closeWindow();
        return openWindow(name, width, height);
    }
            
    bool Graphics::setFullscreen(bool fs)
    {
        std::string name = SDL_GetWindowTitle(m_win);
        int w, h;
        SDL_GetWindowSize(m_win, &w, &h);
        closeWindow();
        if(fs)
            return openFullscreenWindow(name);
        else
            return openWindow(name, w, h);
    }
            
    std::vector<geometry::AABB> Graphics::windowRes(float minw, float minh) const
    {
        std::vector<geometry::AABB> ret;
        ret.reserve(10);

        /** @todo Handle multiple screens. */
        SDL_DisplayMode mode;
        for(int i = 0; i < SDL_GetNumDisplayModes(0); ++i) {
            if(SDL_GetDisplayMode(0, i, &mode) < 0) {
                std::ostringstream oss;
                oss << "Error retrieving display mode #" << i << " : \"" << SDL_GetError() << "\".";
                core::logger::logm(oss.str(), core::logger::WARNING);
                continue;
            }
            if(mode.w >= minw && mode.h >= minh)
                ret.push_back( geometry::AABB((float)mode.w, (float)mode.h) );
        }
        return ret;
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

    void Graphics::logWindow(bool open, bool full, bool ended, bool sdlerr)
    {
        std::ostringstream oss;
        if(!ended) {
            if(open)
                oss << "Couldn't change to ";
            else
                oss << "Couldn't create ";
        }
        else {
            if(open)
                oss << "Changed to ";
            else
                oss << "Created ";
        }

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
        /* Opening OpenGL */
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        m_ctx = SDL_GL_CreateContext(m_win);

        if(m_ctx == 0) {
            core::logger::logm("Couldn't create the openGL context, needed for all graphical operations.", core::logger::FATAL);
            return false;
        }

        /* Extensions */
        if(!m_exts.init()) {
            core::logger::logm("Couldn't init the openGL extension system, needed for all graphical operations.", core::logger::FATAL);
            return false;
        }

        /* Shaders */
        if(!m_shads.checkAndLoadExtensions()) {
            core::logger::logm("Hardware does not support shaders, needed for all graphical operations.", core::logger::FATAL);
            return false;
        }
        if(!m_shads.load()) {
            core::logger::logm("Couldn't load shaders, needed for all graphical operations.", core::logger::FATAL);
            return false;
        }

        return true;
    }

    /*************************
     *     Virtual size      *
     *************************/
    void Graphics::setVirtualSize(float w, float h)
    {
        if(std::abs(w - m_virtualW) < epsilon
                && std::abs(h - m_virtualH) < epsilon)
            return;
        m_virtualW = w;
        m_virtualH = h;
        computeBands();
    }

    void Graphics::disableVirtualSize()
    {
        setVirtualSize((float)windowWidth(), (float)windowHeight());
    }

    void Graphics::preserveRatio(bool r)
    {
        m_virtualR = r;
        computeBands();
    }

    bool Graphics::preserveRatio() const
    {
        return m_virtualR;
    }

    float Graphics::getVirtualWidth() const
    {
        return m_virtualW;
    }

    float Graphics::getVirtualHeight() const
    {
        return m_virtualH;
    }

    bool Graphics::isEnabled() const
    {
        if(std::abs((float)windowWidth() - getVirtualWidth()) > epsilon
                || std::abs((float)windowHeight() - getVirtualHeight()) > epsilon)
            return true;
        else
            return false;
    }

    void Graphics::computeBands()
    {
        if(!m_virtualR) { /* If the ration mustn't be preserved, do not put black bands */
            m_bandWidth = 0.0f;
            m_bandLR = false;
            m_appliedW = m_virtualW;
            m_appliedH = m_virtualH;
        }
        else {
            float wr = (float)windowWidth() / (float)windowHeight(); /* Compute window ratio */
            float ratio = m_virtualW / m_virtualH; /* Compute actual ratio */

            if(std::abs(wr - ratio) < epsilon) { /* If the ratio is the same, do not put black bands */
                m_virtualR = false;
                computeBands();
                m_virtualR = true;
            }

            else if(ratio > wr) { /* If the virtual ratio is superior as window's, the black bands must be on top and bottom */
                m_bandLR = false;
                m_appliedW = m_virtualW;
                float k = m_virtualW / (float)windowWidth();
                m_bandWidth = k * (float)windowHeight() - m_virtualH;
                m_bandWidth /= 2;
                m_appliedH = k * (float)windowHeight();
            }

            else { /* If the virtual ratio is inferior as window's, the black bands must be on the sides */
                m_bandLR = true;
                m_appliedH = m_virtualH;
                float k = m_virtualH / (float)windowHeight();
                m_bandWidth = k * (float)windowWidth() - m_virtualW;
                m_bandWidth /= 2;
                m_appliedW = k * (float)windowWidth();
            }
        }
        logVirtual();
    }

    void Graphics::invertYAxis(bool inv)
    {
        m_yinvert = inv;
    } 

    bool Graphics::isYAxisInverted() const
    {
        return m_yinvert;
    }

    void Graphics::logVirtual()
    {
        std::ostringstream oss;
        oss << "New virtuals parameters : " << m_virtualW << "x" << m_virtualH << " with ratio [" << (m_virtualR ? "on" : "off") << "]";
        if(m_virtualR) {
            oss << " with a " << m_appliedW << "x" << m_appliedH << " internally used size"
                << " and black bands of width " << m_bandWidth << " placed on " << (m_bandLR ? "sides" : "top/bottom") << ".";
        }
        core::logger::logm(oss.str(), core::logger::MSG);
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
            std::ostringstream oss;
            oss << "Couldn't load picture file : \"" << path << "\"";
            core::logger::logm(oss.str(), core::logger::ERROR);
            return false;
        }

        Entity* ent = new Entity;
        ent->type = TEXT;
        ent->stored.text = text;

        if(!m_fs.createEntity(name, ent)) {
            delete text;
            delete ent;
            std::ostringstream oss;
            oss << "Couldn't create entity for picture file : \"" << path << "\"";
            core::logger::logm(oss.str(), core::logger::ERROR);
            return false;
        }
        else
            return true;
    }

    bool Graphics::loadMovie(const std::string& name, const std::string& path)
    {
        if(m_fs.existsEntity(name))
            return false;

        internal::Movie* mov = new internal::Movie(&m_shads);
        if(!mov->load(path)) {
            delete mov;
            std::ostringstream oss;
            oss << "Couldn't load movie file : \"" << path << "\"";
            core::logger::logm(oss.str(), core::logger::ERROR);
            return false;
        }

        Entity* ent = new Entity;
        ent->type = MOVIE;
        ent->stored.movie = mov;

        if(!m_fs.createEntity(name, ent)) {
            delete mov;
            delete ent;
            std::ostringstream oss;
            oss << "Couldn't create entity for movie file : \"" << path << "\"";
            core::logger::logm(oss.str(), core::logger::ERROR);
            return false;
        }
        else
            return true;
    }

    bool Graphics::loadFont(const std::string& name, const std::string& path)
    {
        if(m_fs.existsEntity(name))
            return false;

        internal::Font* font = new internal::Font(&m_shads);
        if(!font->load(path)) {
            delete font;
            std::ostringstream oss;
            oss << "Couldn't load font file : \"" << path << "\"";
            core::logger::logm(oss.str(), core::logger::ERROR);
            return false;
        }

        Entity* ent = new Entity;
        ent->type = FONT;
        ent->stored.font = font;

        if(!m_fs.createEntity(name, ent)) {
            delete font;
            delete ent;
            std::ostringstream oss;
            oss << "Couldn't create entity for font file : \"" << path << "\"";
            core::logger::logm(oss.str(), core::logger::ERROR);
            return false;
        }
        else
            return true;
    }

    bool Graphics::loadTextureFromText(const std::string& name, const std::string& font, const std::string& txt, const Color& bgc, float pts, bool alpha, unsigned char precision)
    {
        if(m_fs.existsEntity(name))
            return false;
        else if(rctype(font) != FONT) {
            core::logger::logm(std::string("Tryed to use an unvalid font : ") + font, core::logger::WARNING);
            return false;
        }

        internal::Font* f;
        f = m_fs.getEntityValue(font)->stored.font;

        /* Generating the texture buffer */
        geometry::AABB tsize = f->stringSize(txt, pts);
        size_t size = (size_t)tsize.width * (size_t)tsize.height * 4;
        unsigned char* buffer = new unsigned char[size];
        for(size_t i = 0; i < size; ++i)
            buffer[i] = 0;

        /* Drawing */
        glClearColor(bgc.r, bgc.g, bgc.b, bgc.a);
        glClearDepth(1.0f);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, tsize.width, 0, tsize.height, 1, -1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        int vp[4];
        glGetIntegerv(GL_VIEWPORT, vp);
        glViewport(0, 0, (int)tsize.width, (int)tsize.height);
        f->draw(txt, geometry::Point(0.0f, 0.0f), pts, false);

        /* Getting the texture rendered */
        glReadPixels(0, 0, (int)tsize.width, (int)tsize.height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        /* Applying transparency */
        if(alpha) {
            for(size_t i = 0; i < (size/4); ++i) {
                if(std::abs((short)buffer[i*4] - (short)bgc.r) <= (short)precision
                        && std::abs((short)buffer[i*4 + 1] - (short)bgc.g) <= (short)precision
                        && std::abs((short)buffer[i*4 + 2] - (short)bgc.b) <= (short)precision) {
                    buffer[i*4 + 3] = 0;
                }
            }
        }

        /* Generating a texture to store the pixels */
        GLuint text = 0;
        glGenTextures(1, &text);
        glBindTexture(GL_TEXTURE_2D, text);
        glTexImage2D(GL_TEXTURE_2D, 0, 4, (int)tsize.width, (int)tsize.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        /* Storing and freeing */
        glViewport(vp[0], vp[1], vp[2], vp[3]);
        internal::Texture* t = new internal::Texture;
        t->loadgl(text, (int)tsize.width, (int)tsize.height);
        delete[] buffer;

        Entity* ent = new Entity;
        ent->type = TEXT;
        ent->stored.text = t;
        if(!m_fs.createEntity(name, ent)) {
            delete t;
            delete ent;
            std::ostringstream oss;
            oss << "Couldn't create entity for text texture : \"" << txt << "\"";
            core::logger::logm(oss.str(), core::logger::ERROR);
            return false;
        }
        else
            return true;
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
        return m_fs.link(name, target, true);
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

    /*************************
     *   Fonts management    *
     *************************/
    geometry::AABB Graphics::stringSize(const std::string& font, const std::string& str, float size) const
    {
        if(rctype(font) != FONT || str.empty())
            return geometry::AABB(0.0f, 0.0f);
        internal::Font* f = m_fs.getEntityValue(font)->stored.font;
        return f->stringSize(str, size);
    }

    float Graphics::stringWidth(const std::string& font, const std::string& str, float size) const
    {
        return stringSize(font, str, size).width;
    }

    /*************************
     *   Movies management   *
     *************************/
    float Graphics::getMovieSpeed(const std::string& name) const
    {
        if(rctype(name) != MOVIE) {
            core::logger::logm("Tryed to access to non-loaded video : " + name, core::logger::WARNING);
            return 0.0f;
        }
        else
            return m_fs.getEntityValue(name)->stored.movie->speed();
    }

    float Graphics::setMovieSpeed(const std::string& name, float nspeed) const
    {
        if(rctype(name) != MOVIE) {
            core::logger::logm("Tryed to access to non-loaded video : " + name, core::logger::WARNING);
            return 0.0f;
        }
        else {
            internal::Movie* mov = m_fs.getEntityValue(name)->stored.movie;
            mov->speed(nspeed);
            return mov->speed();
        }
    }

    void Graphics::rewindMovie(const std::string& name)
    {
        if(rctype(name) != MOVIE) {
            core::logger::logm("Tryed to access to non-loaded video : " + name, core::logger::WARNING);
            return;
        }
        else
            m_fs.getEntityValue(name)->stored.movie->replay();
    }

    /*************************
     *    Transformations    *
     *************************/
    void Graphics::rotate(float angle)
    {
        glRotatef(angle, 0, 0, 1);
    }

    void Graphics::scale(float x, float y)
    {
        glScalef(x, y, 1.0f);
    }

    void Graphics::move(float x, float y)
    {
        glTranslatef(x, y, 0.0f);
    }

    void Graphics::push()
    {
        glPushMatrix();
    }

    bool Graphics::pop()
    {
        glPopMatrix();
        return glGetError() != GL_STACK_UNDERFLOW;
    }

    void Graphics::identity()
    {
        glLoadIdentity();
        if(m_virtualR) {
            if(m_bandLR)
                move(m_bandWidth, 0.0f);
            else
                move(0.0f, m_bandWidth);
        }
    }

    /*************************
     *       Drawing         *
     *************************/
    void Graphics::blitTexture(const std::string& name, const geometry::Point& pos, bool flip)
    {
        if(rctype(name) != TEXT) {
            core::logger::logm(std::string("Tried to blit an unexistant texture : ") + name, core::logger::WARNING);
            return;
        }

        internal::Texture* text = m_fs.getEntityValue(name)->stored.text;
        geometry::Point ori = pos;
        ori.x -= text->hotpoint().x;
        ori.y -= text->hotpoint().y;

        m_shads.text(true);
        glBindTexture(GL_TEXTURE_2D, text->glID());

        float xfill = 1.0f, xempty = 0.0f;
        if(flip) {
            xfill = 0.0f;
            xempty = 1.0f;
        }

        glColor4ub(255, 255, 255, 255);
        glBegin(GL_QUADS);
        if(m_yinvert) {
            glTexCoord2f(xempty,1.0f); glVertex2f(ori.x,                        ori.y);
            glTexCoord2f(xfill, 1.0f); glVertex2f(ori.x + (float)text->width(), ori.y);
            glTexCoord2f(xfill, 0.0f); glVertex2f(ori.x + (float)text->width(), ori.y + (float)text->height());
            glTexCoord2f(xempty,0.0f); glVertex2d(ori.x,                        ori.y + (float)text->height());
        }
        else {
            glTexCoord2f(xempty,0.0f); glVertex2f(ori.x,                        ori.y);
            glTexCoord2f(xfill, 0.0f); glVertex2f(ori.x + (float)text->width(), ori.y);
            glTexCoord2f(xfill, 1.0f); glVertex2f(ori.x + (float)text->width(), ori.y + (float)text->height());
            glTexCoord2f(xempty,1.0f); glVertex2d(ori.x,                        ori.y + (float)text->height());
        }
        glEnd();
    }

    void Graphics::draw(const geometry::Point& point, const Color& col, float width)
    {
        if(width >= 0.0f)
            glPointSize(width);
        m_shads.text(false);

        glBegin(GL_POINTS);
        glColor4ub(col.r, col.g, col.b, col.a);
        glVertex2f(point.x, point.y);
        glEnd();

        glPointSize(m_lineWidth);
    }

    void Graphics::draw(const geometry::Line& line, const Color& col, float width)
    {
        if(width >= 0)
            glLineWidth(width);

        m_shads.text(false);
        glBegin(GL_LINES);
        glColor4ub(col.r, col.g, col.b, col.a);
        glVertex2f(line.p1.x, line.p1.y);
        glVertex2f(line.p2.x, line.p2.y);
        glEnd();

        glLineWidth(width);
    }

    void Graphics::draw(const geometry::AABB& aabb, const std::string& text, float repeatX, float repeatY)
    {
        if(rctype(text) != TEXT) {
            core::logger::logm(std::string("Tried to use an unexistant texture (AABB blitting) : ") + text, core::logger::WARNING);
            return;
        }

        internal::Texture* t = m_fs.getEntityValue(text)->stored.text;
        m_shads.text(true);
        glBindTexture(GL_TEXTURE_2D, t->glID());
        glColor4ub(255, 255, 255, 255);

        glBegin(GL_QUADS);
        if(m_yinvert) {
            glTexCoord2f(0.0f,   repeatY); glVertex2f(0.0f,       0.0f);
            glTexCoord2f(repeatX,repeatY); glVertex2f(aabb.width, 0.0f);
            glTexCoord2f(repeatX,0.0f);    glVertex2f(aabb.width, aabb.height);
            glTexCoord2f(0.0f,   0.0f);    glVertex2f(0.0f,       aabb.height);
        }
        else {
            glTexCoord2f(0.0f,   0.0f);    glVertex2f(0.0f,       0.0f);
            glTexCoord2f(repeatX,0.0f);    glVertex2f(aabb.width, 0.0f);
            glTexCoord2f(repeatX,repeatY); glVertex2f(aabb.width, aabb.height);
            glTexCoord2f(0.0f,   repeatY); glVertex2f(0.0f,       aabb.height);
        }
        glEnd();
    }

    void Graphics::draw(const geometry::AABB& aabb, const Color& col)
    {
        m_shads.text(false);
        glBegin(GL_QUADS);
        glColor4ub(col.r, col.g, col.b, col.a);
        glVertex2f(0.0f,       0.0f);
        glVertex2f(aabb.width, 0.0f);
        glVertex2f(aabb.width, aabb.height);
        glVertex2f(0.0f,       aabb.height);
        glEnd();
    }

    void Graphics::draw(const geometry::Circle& circle, const std::string& text, float repeatX, float repeatY)
    {
        if(rctype(text) != TEXT) {
            core::logger::logm(std::string("Tried to use an unexistant texture (circle blitting) : ") + text, core::logger::WARNING);
            return;
        }

        internal::Texture* t = m_fs.getEntityValue(text)->stored.text;
        m_shads.text(true);
        glBindTexture(GL_TEXTURE_2D, t->glID());
        glColor4ub(255, 255, 255, 255);

        float lx = std::cos(0.0f) * circle.radius;
        float ly = std::sin(0.0f) * circle.radius;
        float ltx = (std::cos(0.0f) + 1) / 2 * repeatX;
        float lty = (std::sin(0.0f) + 1) / 2 * repeatY;
        float mx = repeatX / 2;
        float my = repeatY / 2;
        for(int i = 1; i <= 360; ++i) {
            float angle = (float)i * deg2rad;
            float ca = std::cos(angle);
            float sa = std::sin(angle);
            float nx = ca * circle.radius;
            float ny = sa * circle.radius;
            float ntx = (ca + 1) / 2 * repeatX;
            float nty = (sa + 1) / 2 * repeatY;

            glBegin(GL_POLYGON);
            glTexCoord2f(mx, my ); glVertex2f(0.0f, 0.0f);
            if(m_yinvert) {
                glTexCoord2f(ntx,-nty+2*mx); glVertex2f(nx,   ny);
                glTexCoord2f(ltx,-lty+2*my); glVertex2f(lx,   ly);
            }
            else {
                glTexCoord2f(ntx,nty); glVertex2f(nx,   ny);
                glTexCoord2f(ltx,lty); glVertex2f(lx,   ly);
            }
            glEnd();

            lx = nx;
            ly = ny;
            ltx = ntx;
            lty = nty;
        }
    }

    void Graphics::draw(const geometry::Circle& circle, const Color& col)
    {
        m_shads.text(false);
        glColor4ub(col.r, col.g, col.b, col.a);
        float lx = std::cos(0.0f) * circle.radius;
        float ly = std::sin(0.0f) * circle.radius;

        for(int i = 1; i <= 360; ++i) {
            float angle = (float)i * deg2rad;
            float nx = std::cos(angle) * circle.radius;
            float ny = std::sin(angle) * circle.radius;

            glBegin(GL_POLYGON);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(nx, ny);
            glVertex2f(lx, ly);
            glEnd();

            lx = nx;
            ly = ny;
        }
    }

    void Graphics::draw(const geometry::Polygon& poly, const std::string& text, float repeatX, float repeatY)
    {
        if(rctype(text) != TEXT) {
            core::logger::logm(std::string("Tried to use an unexistant texture (polygon blitting) : ") + text, core::logger::WARNING);
            return;
        }

        if(poly.points.size() == 0)
            return;

        internal::Texture* t = m_fs.getEntityValue(text)->stored.text;
        m_shads.text(true);
        glBindTexture(GL_TEXTURE_2D, t->glID());
        glColor4ub(255, 255, 255, 255);

        float minx = poly.points[0].x;
        float miny = poly.points[0].y;
        float maxx = poly.points[0].x;
        float maxy = poly.points[0].y;
        for(size_t i = 1; i < poly.points.size(); ++i) {
            if(poly.points[i].x < minx)
                minx = poly.points[i].x;
            else if(poly.points[i].x > maxx)
                maxx = poly.points[i].x;
            if(poly.points[i].y < miny)
                miny = poly.points[i].y;
            else if(poly.points[i].y > maxy)
                maxy = poly.points[i].y;
        }
        float interx = maxx - minx;
        float intery = maxy - miny;

        std::vector<geometry::Polygon> conv = poly.convexify();
        for(geometry::Polygon p : conv) {
            glBegin(GL_POLYGON);
            for(size_t i = 0; i < p.points.size(); ++i) {
                float tx = (p.points[i].x - minx) / interx * repeatX;
                float ty = (p.points[i].y - miny) / intery * repeatY;
                if(m_yinvert)
                    glTexCoord2f(tx, -ty);
                else
                    glTexCoord2f(tx, ty);
                glVertex2f(p.points[i].x, p.points[i].y);
            }
            glEnd();
        }
    }

    void Graphics::draw(const geometry::Polygon& poly, const Color& col)
    {
        m_shads.text(false);
        std::vector<geometry::Polygon> conv = poly.convexify();
        for(geometry::Polygon p : conv) {
            glBegin(GL_POLYGON);
            glColor4ub(col.r, col.g, col.b, col.a);
            for(size_t i = 0; i < poly.points.size(); ++i)
                glVertex2f(p.points[i].x, p.points[i].y);
            glEnd();
        }
    }

    void Graphics::draw(const std::string& str, const std::string& font, float pts)
    {
        if(rctype(font) != FONT) {
            core::logger::logm(std::string("Tried to use an unexistant font (text drawing) : ") + font, core::logger::WARNING);
            return;
        }

        internal::Font* f = m_fs.getEntityValue(font)->stored.font;
        f->draw(str, geometry::Point(0.0f, 0.0f), pts, true, m_yinvert);
    }

    bool Graphics::play(const std::string& movie, const geometry::AABB& rect, bool ratio)
    {
        if(rctype(movie) != MOVIE) {
            core::logger::logm(std::string("Tried to play an unexistant movie : ") + movie, core::logger::WARNING);
            return false;
        }

        internal::Movie* m = m_fs.getEntityValue(movie)->stored.movie;
        bool ret = m->updateFrame();
        m->displayFrame(rect, ratio, m_yinvert);
        return ret;
    }

    float Graphics::defaultWidth(float nval)
    {
        glPointSize(nval);
        glLineWidth(nval);
        return (m_lineWidth = nval);
    }

    float Graphics::defaultWidth() const
    {
        return m_lineWidth;
    }

    void Graphics::beginDraw()
    {
        glClearColor(0, 0, 0, 0);
        glClearDepth(1.0f);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if(m_yinvert)
            glOrtho(0, m_appliedW, 0, m_appliedH, 1, -1);
        else
            glOrtho(0, m_appliedW, m_appliedH, 0, 1, -1);

        glMatrixMode(GL_MODELVIEW);
        identity();
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        m_shads.enable(true);
    }

    void Graphics::endDraw()
    {
        if(m_virtualR) {
            geometry::AABB band;
            identity();
            if(m_bandLR) {
                band.set(m_bandWidth, m_appliedH);
                move(-m_bandWidth, 0.0f);
            }
            else {
                band.set(m_appliedW, m_bandWidth);
                move(0.0f, -m_bandWidth);
            }
            Color c;
            draw(band, c);
            if(m_bandLR)
                move(m_bandWidth + m_virtualW, 0.0f);
            else
                move(0.0f, m_bandWidth + m_virtualH);
            draw(band, c);
        }

        glFlush();
        SDL_GL_SwapWindow(m_win);
    }

}

