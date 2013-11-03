
#ifndef DEF_GRAPHICS_GRAPHICS
#define DEF_GRAPHICS_GRAPHICS

#include "core/fakefs.hpp"

#include "geometry/point.hpp"
#include "geometry/line.hpp"
#include "geometry/circle.hpp"
#include "geometry/aabb.hpp"
#include "geometry/polygon.hpp"

#include "graphics/shaders.hpp"
#include "graphics/texture.hpp"
#include "graphics/movie.hpp"
#include "graphics/font.hpp"
#include "graphics/exts.hpp"
#include "graphics/color.hpp"

#include <SDL.h>
#include <GL/gl.h>
#include <GL/glext.h>

namespace graphics
{
    class Graphics
    {
        public:
            Graphics();
            ~Graphics();
            
            /*************************
             *  Window manipulation  *
             *************************/
            /* Opens a window of size w*h */
            bool openWindow(const std::string& name, int w, int h);
            /* Opens a window with the fullscreen size and default depth
             * The min* arguments precise the minimal value for window size : will fail if return false if can't create a window of at least these values
             */
            bool openFullscreenWindow(const std::string& name, int minw = 0, int minh = 0);
            /* Closes the window (necessary if you wants to open a new window) */
            void closeWindow();
            /* Get Information about the window */
            int windowWidth() const;
            int windowHeight() const;
            int windowDepth() const;
            bool isWindowOpen() const;

            /*************************
             *     Virtual size      *
             *************************/
            /* The virtual size is a size used to place and draw elements */
            void setVirtualSize(float w, float h);
            /* It is the same as setVirtualSize(windowWidth(), windowHeight()) */
            void disableVirtualSize();
            /* If r, it will add black bands on the side of the screen to preserve the ratio */
            void preserveRatio(bool r);
            /* By default, the Y axs is from top to bottom. This method allows you to invert it */
            void invertYAxis(bool inv);
            /* Get information about virtual size */
            bool preserveRatio() const;
            float getVirtualWidth() const;
            float getVirtualHeight() const;
            bool isYAxisInverted() const;
            bool isEnabled() const; /* Is the virtual size different from the window size */

            /*************************
             * Namespaces management *
             *************************/
            bool enterNamespace(const std::string& name);
            bool createNamespace(const std::string& name);
            void deleteNamespace(const std::string& name);
            std::string actualNamespace() const;

            /*************************
             *  Ressources loading   *
             *************************/
            bool loadTexture(const std::string& name, const std::string& path);
            bool loadMovie(const std::string& name, const std::string& path);
            bool loadFont(const std::string& name, const std::string& path);
            /* Will create a texture in which the txt text is rendered with font
             * bgc is the color of the background, because trasparency is not handled
             * If alpha is set, the bgc will be transparent
             * precision precise the deviation allowed from bgc to still have alpha appliqued
             * pts is the heights in units of the rendered font, if it's negative, default height will be used
             */
            bool loadTextureFromText(const std::string& name, const std::string& font, const std::string& txt, const Color& bgc, float pts = -1.0f, bool alpha = false, unsigned char precision = 5);
            /* Will free the ressource wathever its type is */
            void free(const std::string& name);
            /* Return the type of a ressource or NONE if name doesn't exists */
            enum RcType {TEXT, MOVIE, FONT, NONE};
            RcType rctype(const std::string& name) const;
            /* Will create a link named name to target */
            bool link(const std::string& name, const std::string& target);

            /*************************
             *  Textures management  *
             *************************/
            int getTextureWidth(const std::string& name) const;
            int getTextureHeight(const std::string& name) const;
            bool setTextureHotpoint(const std::string& name, int x, int y);

            /*************************
             *   Fonts management    *
             *************************/
            float stringWidth(const std::string& font, const std::string& str, float size = -1.0f) const;
            geometry::AABB stringSize(const std::string& font, const std::string& str, float size = -1.0f) const;
            
            /*************************
             *   Movies management   *
             *************************/
            /* Will return 0.0f if name does not design a video */
            float getMovieSpeed(const std::string& name) const;
            float setMovieSpeed(const std::string& name, float nspeed) const;
            void rewindMovie(const std::string& name);

            /*************************
             *    Transformations    *
             *************************/
            /* The angle is in degres, along z axis */
            void rotate(float angle);
            void scale(float x, float y);
            void move(float x, float y);
            void identity();
            void push();
            bool pop();

            /*************************
             *       Drawing         *
             *************************/
            /* The hotpoint is only used for blitTexture */
            void blitTexture(const std::string& name, const geometry::Point& pos);
            /* If width < 0, will use the default width */
            void draw(const geometry::Point& point, const Color& col, float width = -1.0f);
            void draw(const geometry::Line& line, const Color& col, float width = -1.0f);
            void draw(const geometry::AABB& aabb, const std::string& text, float repeatX = 1.0f, float repeatY = 1.0f);
            void draw(const geometry::AABB& aabb, const Color& col);
            void draw(const geometry::Circle& circle, const std::string& text, float repeatX = 1.0f, float repeatY = 1.0f);
            void draw(const geometry::Circle& circle, const Color& col);
            void draw(const geometry::Polygon& poly, const std::string& text, float repeatX = 1.0f, float repeatY = 1.0f);
            void draw(const geometry::Polygon& poly, const Color& col);
            void draw(const std::string& str, const std::string& font, float pts = -1.0f); /* Write a text with the selected font */
            /* Return false when the video ended */
            bool play(const std::string& movie, const geometry::AABB& rect, bool ratio = true);
            /* Set and get default width */
            float defaultWidth(float nval);
            float defaultWidth() const;
            /* Must be called at the begin and the end of each draw cycle */
            void beginDraw();
            void endDraw();

        private:
            SDL_Window* m_win;
            SDL_GLContext m_ctx;
            internal::Extensions m_exts;
            internal::Shaders m_shads;
            /* Virtual size */
            float m_virtualW, m_virtualH;
            float m_appliedW, m_appliedH;
            float m_bandWidth;
            bool m_bandLR; /* Must the black bands be on left and right or up and down */
            bool m_virtualR;
            bool m_yinvert;
            /* Drawing */
            float m_lineWidth;

            /**************************
             *   Fake-FS structure    *
             **************************/
            struct Entity /* Used in the fake-fs */
            {
                union Stored { /* The value really stored */
                    internal::Texture* text;
                    internal::Movie* movie;
                    internal::Font* font;
                };
                Stored stored;
                RcType type; /* It indicates the type of the value stored */
            };

            class EntityLiberator
            {
                public:
                    void operator()(Entity* tofree) const;
            };

            core::FakeFS<Entity*, EntityLiberator> m_fs;

            /**************************
             *   Internal functions   *
             **************************/
            /* Windows */
            void logWindow(bool full, bool ended, bool sdlerr = false);
            bool glContext();
            /* Virtual size */
            void computeBands();
            void logVirtual();
    };
}

#endif

