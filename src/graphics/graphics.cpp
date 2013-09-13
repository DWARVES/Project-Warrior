
#include "graphics/graphics.hpp"
#include "core/logger.hpp"
#include <sstream>
#include <GL/gl.h>

namespace graphics
{
    const float epsilon = 0.000001f;
    const float deg2rad = 0.0174532925199433f;

    Graphics::Graphics()
        : m_win(NULL), m_ctx(0),
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
    void Graphics::setVirtualSize(float w, float h)
    {
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
    void Graphics::blitTexture(const std::string& name, const geometry::Point& pos)
    {
        /* FIXME texture orientation with Y axis inverted */
        if(rctype(name) != TEXT) {
            core::logger::logm(std::string("Tried to blit an unexistant texture : ") + name, core::logger::WARNING);
            return;
        }

        internal::Texture* text = m_fs.getEntityValue(name)->stored.text;
        geometry::Point ori = pos;
        ori.x -= text->hotpoint().x;
        ori.y -= text->hotpoint().y;

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, text->glID());

        glColor4ub(255, 255, 255, 255);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f,0.0f); glVertex2f(ori.x,                        ori.y);
        glTexCoord2f(1.0f,0.0f); glVertex2f(ori.x + (float)text->width(), ori.y);
        glTexCoord2f(1.0f,1.0f); glVertex2f(ori.x + (float)text->width(), ori.y + (float)text->height());
        glTexCoord2f(0.0f,1.0f); glVertex2d(ori.x,                        ori.y + (float)text->height());
        glEnd();
    }

    void Graphics::draw(const geometry::Point& point, const Color& col, float width)
    {
        if(width >= 0.0f)
            glPointSize(width);

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

        glDisable(GL_TEXTURE_2D);
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
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, t->glID());
        glColor4ub(255, 255, 255, 255);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f,   0.0f);    glVertex2f(0.0f,       0.0f);
        glTexCoord2f(repeatX,0.0f);    glVertex2f(aabb.width, 0.0f);
        glTexCoord2f(repeatX,repeatY); glVertex2f(aabb.width, aabb.height);
        glTexCoord2f(0.0f,   repeatY); glVertex2f(0.0f,       aabb.height);
        glEnd();
    }

    void Graphics::draw(const geometry::AABB& aabb, const Color& col)
    {
        glDisable(GL_TEXTURE_2D);
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
        glEnable(GL_TEXTURE_2D);
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
            glTexCoord2f(ntx,nty); glVertex2f(nx,   ny);
            glTexCoord2f(ltx,lty); glVertex2f(lx,   ly);
            glEnd();

            lx = nx;
            ly = ny;
            ltx = ntx;
            lty = nty;
        }
    }

    void Graphics::draw(const geometry::Circle& circle, const Color& col)
    {
        glDisable(GL_TEXTURE_2D);
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
        /* TODO handle concaves polygons */
        if(rctype(text) != TEXT) {
            core::logger::logm(std::string("Tried to use an unexistant texture (circle blitting) : ") + text, core::logger::WARNING);
            return;
        }

        internal::Texture* t = m_fs.getEntityValue(text)->stored.text;
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, t->glID());
        glColor4ub(255, 255, 255, 255);

        float minx, maxx, miny, maxy;
        for(size_t i = 0; i < poly.points.size(); ++i) {
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

        glBegin(GL_POLYGON);
        for(size_t i = 0; i < poly.points.size(); ++i) {
            float tx = (poly.points[i].x - minx) / interx * repeatX;
            float ty = (poly.points[i].y - miny) / intery * repeatY;
            glTexCoord2f(tx, ty);
            glVertex2f(poly.points[i].x, poly.points[i].y);
        }
        glEnd();
    }

    void Graphics::draw(const geometry::Polygon& poly, const Color& col)
    {
        /* TODO handle concaves polygons */
        glDisable(GL_TEXTURE_2D);
        glBegin(GL_POLYGON);
        glColor4ub(col.r, col.g, col.b, col.a);
        for(size_t i = 0; i < poly.points.size(); ++i)
            glVertex2f(poly.points[i].x, poly.points[i].y);
        glEnd();
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

