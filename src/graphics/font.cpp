
#include "graphics/font.hpp"
#include "core/logger.hpp"
#include "core/utf8.hpp"
#include <algorithm>
#include <sstream>
#include <fstream>
#include <cstring>

namespace graphics
{
    namespace internal
    {
        Font::Font(Shaders* shads)
            : m_yspacing(0.0f), m_xspacing(0.0f), m_letterSP(0.0f), m_text(NULL), m_shads(shads)
        {}

        Font::~Font()
        {
            if(m_text)
                delete m_text;
        }
               
        bool Font::load(const std::string& path)
        {
            /* Open the file */
            std::ifstream ifs(path);
            if(!ifs) {
                core::logger::logm("Couldn't open " + path, core::logger::WARNING);
                return false;
            }

            /* Get file content */
            ifs.seekg(0, ifs.end);
            size_t length = ifs.tellg();
            if(length <= 10) {
                core::logger::logm("Invalid file : " + path, core::logger::WARNING);
                return false;
            }
            ifs.seekg(0, ifs.beg);
            char* buffer = new char [length];
            ifs.read(buffer, length);
            if(!ifs) {
                delete buffer;
                core::logger::logm("Couldn't read " + path, core::logger::WARNING);
                return false;
            }

            /* Get the letters */
            unsigned int lettersSize;
            std::memcpy(&lettersSize, buffer, sizeof(lettersSize));
            if(lettersSize + 4 > length) {
                core::logger::logm("Invalid file : " + path, core::logger::WARNING);
                delete buffer;
                return false;
            }

            char* buffer_lets = new char [lettersSize+1];
            std::memcpy(buffer_lets, buffer + 4, sizeof(char) * lettersSize);
            buffer_lets[lettersSize] = '\0';
            std::string letters(buffer_lets);
            delete buffer_lets;

            /* Load the texture */
            SDL_RWops* imgrw = SDL_RWFromMem(buffer + 4u + lettersSize, (int)(length - 4 - lettersSize));
            if(imgrw == NULL) {
                delete buffer;
                core::logger::logm("Couldn't open rwops from memory.", core::logger::WARNING);
                return false;
            }

            m_text = new Texture(m_shads->exts());
            SDL_Surface* surf = m_text->preload(imgrw);
            SDL_RWclose(imgrw);
            if(surf == NULL) {
                delete m_text;
                m_text = NULL;
                delete buffer;
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
            
            core::UTF8String utf(letters);
            size_t lets = utf.size();
            if(columns == 0 || rows == 0
                    || columns * rows < lets) {
                std::ostringstream oss;
                oss << "Invalid number of columns and rows (" << columns << "x" << rows << ") in font \"" << path << "\" with #" << lets << " letters.";
                core::logger::logm(oss.str(), core::logger::WARNING);
                SDL_FreeSurface(surf);
                delete m_text;
                m_text = NULL;
                delete buffer;
                return false;
            }

            /* Load caracters */
            Uint32 bg = pixel(surf, 1, 1);
            int wd = surf->w / columns;
            int hd = surf->h / rows;
            for(size_t i = 0; i < lets; ++i) {
                Letter l;
                l.lt.x = float( ((int)i % columns) * wd + 1 );
                l.lt.y = float( ((int)i / columns) * hd + 1 );
                l.rb.x = l.lt.x + (float)wd - 1.0f;
                l.rb.y = l.lt.y + (float)hd - 2.0f;
                fitToChar(&l, surf, bg);
                m_letters[ utf[i] ] = l;
            }

            /* Deleting separators */
            for(int x = 0; x < surf->w; ++x) {
                for(int y = 0; y < surf->h; ++y) {
                    if(pixel(surf, x, y) == sepColor)
                        pixel(surf, x, y, bg);
                }
            }

            /* Saving */
            m_xspacing = (float)wd/2.0f;
            m_letterSP = m_xspacing/10.0f;
            m_yspacing = (float)hd;
            bool ret = m_text->loadsdl(surf);
            if(!ret) {
                delete m_text;
                m_text = NULL;
            }
            SDL_FreeSurface(surf);
            delete buffer;
            return ret;
        }

        void Font::draw(const std::string& str, const geometry::Point& pos, float size, bool smooth, bool invert)
        {
            core::UTF8String utf(str);
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
                for(size_t i = 0; i < utf.size(); ++i) {
                    if(utf[i] == 10) /* 10 is new line */
                        ++nbret;
                }
                actPos.y += (float)nbret * size;
            }

            for(size_t i = 0; i < utf.size(); ++i) {
                if(utf[i] == 10) { /* 10 is new line */
                    actPos.x = pos.x;
                    if(invert)
                        actPos.y -= size;
                    else
                        actPos.y += size;
                }
                else if(!hasLetter(utf[i])) { /* If the letter is not found, draw a space */
                    actPos.x += m_xspacing * fact;
                }
                else { /* Draw the letter */
                    Letter l = m_letters[utf[i]];
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
                    actPos.x += (float)l.w * fact + m_letterSP * fact;
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
            core::UTF8String utf(str);

            for(size_t i = 0; i < utf.size(); ++i) {
                if(utf[i] == '\n') {
                    width = std::max(width, widths[act]);
                    widths.push_back(0);
                    ++act;
                    height += size;
                }
                else if(hasLetter(utf[i]))
                    widths[act] += (m_letters.find(utf[i])->second.w * fact + m_letterSP * fact);
                else
                    widths[act] += (m_xspacing * fact); /* Non existant characters are replaced by spaces */
            }

            width = std::max(width, widths[act]);
            return geometry::AABB(width, height);
        }

        float Font::widthLetter(unsigned int l) const
        {
            if(!hasLetter(l))
                return 0;
            else
                return m_letters.find(l)->second.w;
        }

        bool Font::hasLetter(unsigned int l) const
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


