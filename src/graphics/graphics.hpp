
#ifndef DEF_GRAPHICS_GRAPHICS
#define DEF_GRAPHICS_GRAPHICS

#include "core/fakefs.hpp"

#include "geometry/point.hpp"
#include "geometry/line.hpp"
#include "geometry/circle.hpp"
#include "geometry/aabb.hpp"
#include "geometry/polygon.hpp"

#include "graphics/texture.hpp"
#include "graphics/movie.hpp"
#include "graphics/font.hpp"
#include "graphics/color.hpp"

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
            /* Opens a window of size w*h and of depth d */
            bool openWindow(unsigned int w, unsigned int h, unsigned int d);
            /* Opens a window with the fullscreen size and default depth
             * The min* arguments precise the minimal value for window size and depth : will fail if return false if can't create a window of at least these values
             */
            bool openFullscreenWindow(unsigned int minw = 0, unsigned int minh = 0, unsigned int mind = 0);
            /* Closes the window (necessary if you wants to open a new window) */
            void closeWindow();
            /* Get Information about the window */
            unsigned int windowWindth() const;
            unsigned int windowHeight() const;
            unsigned int windowDepth() const;
            bool isWindowOpen() const;

            /*************************
             *     Virtual size      *
             *************************/
            /* The virtual size is a size used to place and draw elements */
            void setVirtualSize(unsigned int w, unsigned int h);
            /* It is the same as setVirtualSize(windowWidth(), windowHeight()) */
            void disableVirtualSize();
            /* If r, it will add black bands on the side of the screen to preserve the ratio */
            void preserveRatio(bool r);
            /* Get information about virtual size */
            bool preserveRatio() const;
            unsigned int getVirtualWidth() const;
            unsigned int getVirtualHeight() const;
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
            /* Will create a texture in which the txt text is rendered with font */
            bool loadTextureFromText(const std::string& name, const std::string& font, const std::string& txt);
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
            unsigned int getTextureWidth(const std::string& name) const;
            unsigned int getTextureHeight(const std::string& name) const;
            bool setTextureHotpoint(const std::string& name, int x, int y);

            /*************************
             *    Transformations    *
             *************************/
            /* The angle is in degres, along z axis */
            void rotate(float angle);
            void scale(float x, float y);
            void move(float x, float y);

            /*************************
             *       Drawing         *
             *************************/
            /* The hotpoint is only used for blitTexture */
            void blitTexture(const std::string& name, const geometry::Point& pos);
            /* If width < 0, will use the default width */
            void draw(const geometry::Point& point, const Color& col, int width = -1);
            void draw(const geometry::Line& line, const std::string& text, float repeatX = 1.0, float repeatY = 1.0, int width = -1);
            void draw(const geometry::Line& line, const Color& col, int width = -1);
            void draw(const geometry::AABB& aabb, const std::string& text, float repeatX = 1.0, float repeatY = 1.0);
            void draw(const geometry::AABB& aabb, const Color& col);
            void draw(const geometry::Circle& circle, const std::string& text, float repeatX = 1.0, float repeatY = 1.0);
            void draw(const geometry::Circle& circle, const Color& col);
            void draw(const geometry::Polygon& poly, const std::string& text, float repeatX = 1.0, float repeatY = 1.0);
            void draw(const geometry::Polygon& poly, const Color& col);
            /* Set and get default width */
            unsigned int defaultWidth(unsigned int nval);
            unsigned int defaultWidth() const;

        private:
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
    };
}

#endif

