
#include "graphics/font.hpp"
#include <algorithm>

namespace graphics
{
    namespace internal
    {
        Font::Font(Shaders* shads)
            : m_yspacing(0), m_xspacing(0), m_text(NULL), m_shads(shads)
        {}

        Font::~Font()
        {
            if(m_text)
                delete m_text;
        }
               
        bool Font::load(const std::string& path, const std::string& letters)
        {
            m_text = new Texture;
            SDL_Surface* surf = m_text->preload(path);
            if(surf == NULL) {
                delete m_text;
                m_text = NULL;
                return false;
            }

            /* Find number of rows and columns */
            Uint32 sepColor = pixel(surf, 0, 0);
            unsigned int columns = 0;
            for(int i = 1; i < surf->w; ++i) {
                if(pixel(surf, i, 1) == sepColor)
                    ++columns;
            }

            unsigned int rows = 0;
            for(int i = 1; i < surf->h; ++i) {
                if(pixel(surf, 1, i) == sepColor)
                    ++rows;
            }
            
            if(columns == 0 || rows == 0
                    || columns * rows < letters.size()) {
                SDL_FreeSurface(surf);
                delete m_text;
                m_text = NULL;
                return false;
            }

            /* Load caracters */
            Uint32 bg = pixel(surf, 1, 1);
            int wd = surf->w / columns;
            int hd = surf->h / rows;
            for(size_t i = 0; i < letters.size(); ++i) {
                Letter l;
                l.lt.x = float( ((int)i % columns) * wd + 1 );
                l.lt.y = float( ((int)i / columns) * hd + 1 );
                l.rb.x = l.lt.x + (float)wd - 1.0f;
                l.rb.y = l.lt.y + (float)hd - 2.0f;
                fitToChar(&l, surf, bg);
                m_letters[ letters[i] ] = l;
            }

            /* Deleting separators */
            for(int x = 0; x < surf->w; ++x) {
                for(int y = 0; y < surf->h; ++y) {
                    if(pixel(surf, x, y) == sepColor)
                        pixel(surf, x, y, bg);
                }
            }

            /* Saving */
            m_xspacing = (float)wd;
            m_yspacing = (float)hd;
            bool ret = m_text->loadsdl(surf);
            if(!ret) {
                delete m_text;
                m_text = NULL;
            }
            SDL_FreeSurface(surf);
            return ret;
        }

        void Font::draw(const std::string& str, const geometry::Point& pos, float size, bool smooth, bool invert)
        {
            m_shads->text(true);
            glBindTexture(GL_TEXTURE_2D, m_text->glID());
            glColor4ub(255, 255, 255, 255);

            if(smooth) {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            }
            else {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            }

            geometry::Point actPos = pos;
            float fact = 1.0f;
            if(size < 0.0f)
                size = m_yspacing;
            else
                fact = size / m_yspacing;

            if(invert) {
                unsigned int nbret = 0;
                for(size_t i = 0; i < str.size(); ++i) {
                    if(str[i] == '\n')
                        ++nbret;
                }
                actPos.y += (float)nbret * size;
            }

            for(size_t i = 0; i < str.size(); ++i) {
                if(str[i] == '\n') {
                    actPos.x = pos.x;
                    if(invert)
                        actPos.y -= size;
                    else
                        actPos.y += size;
                }
                else if(!hasLetter(str[i])) { /* If the letter is not found, draw a space */
                    actPos.x += m_xspacing * fact;
                }
                else { /* Draw the letter */
                    Letter l = m_letters[str[i]];
                    glBegin(GL_QUADS);
                    if(invert) {
                        glTexCoord2f(l.lt.x, l.rb.y); glVertex2f(actPos.x,              actPos.y);
                        glTexCoord2f(l.rb.x, l.rb.y); glVertex2f(actPos.x + l.w * fact, actPos.y);
                        glTexCoord2f(l.rb.x, l.lt.y); glVertex2f(actPos.x + l.w * fact, actPos.y + l.h * fact);
                        glTexCoord2f(l.lt.x, l.lt.y); glVertex2f(actPos.x,              actPos.y + l.h * fact);
                    }
                    else {
                        glTexCoord2f(l.lt.x, l.lt.y); glVertex2f(actPos.x,              actPos.y);
                        glTexCoord2f(l.rb.x, l.lt.y); glVertex2f(actPos.x + l.w * fact, actPos.y);
                        glTexCoord2f(l.rb.x, l.rb.y); glVertex2f(actPos.x + l.w * fact, actPos.y + l.h * fact);
                        glTexCoord2f(l.lt.x, l.rb.y); glVertex2f(actPos.x,              actPos.y + l.h * fact);
                    }
                    glEnd();
                    actPos.x += (float)l.w * fact;
                }
            }
        }

        geometry::AABB Font::stringSize(const std::string& str, float size) const
        {
            std::vector<float> widths(1,0);
            size_t act = 0;
            float width = 0;
            float fact = 1.0f;
            if(size < 0.0f)
                size = m_yspacing;
            else
                fact = size / m_yspacing;
            float height = size;

            for(size_t i = 0; i < str.size(); ++i) {
                if(str[i] == '\n') {
                    width = std::max(width, widths[act]);
                    widths.push_back(0);
                    ++act;
                    height += size;
                }
                else if(hasLetter(str[i]))
                    widths[act] += (m_letters.find(str[i])->second.w * fact);
                else
                    widths[act] += (m_xspacing * fact); /* Non existant characters are replaced by spaces */
            }

            width = std::max(width, widths[act]);
            return geometry::AABB(width, height);
        }

        float Font::widthLetter(char l) const
        {
            if(!hasLetter(l))
                return 0;
            else
                return m_letters.find(l)->second.w;
        }

        bool Font::hasLetter(char l) const
        {
            return m_letters.find(l) != m_letters.end();
        }

        bool Font::isLoaded() const
        {
            return m_text != NULL;
        }

        Uint32 Font::pixel(SDL_Surface* s, int x, int y)
        {
            Uint32* pixels = (Uint32*)s->pixels;
            return pixels[ y * s->w + x ];
        }

        void Font::pixel(SDL_Surface* s, int x, int y, Uint32 pix)
        {
            Uint32* pixels = (Uint32*)s->pixels;
            pixels[ y * s->w + x ] = pix;
        }
                
        void Font::fitToChar(Letter* l, SDL_Surface* surf, Uint32 bg)
        {
            geometry::Point lt = l->lt;
            geometry::Point rb = l->rb;
            lt.x = l->rb.x;
            rb.x = l->lt.x;

            for(float x = l->lt.x; x < l->rb.x; ++x) {
                for(float y = l->lt.y; y < l->rb.y; ++y) {
                    if(pixel(surf, (int)x, (int)y) != bg) {
                        lt.x = std::min(x, lt.x);
                        rb.x = std::max(x, rb.x);
                    }
                }
            }

            rb.x += 1.0f;
            l->w = rb.x - lt.x;
            l->h = rb.y - lt.y;
            lt.x /= (float)surf->w;
            lt.y /= (float)surf->h;
            rb.x /= (float)surf->w;
            rb.y /= (float)surf->h;
            l->lt = lt;
            l->rb = rb;
        }
    }
}


