
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

/** @brief Contains all classes and methods related to graphical output. */
namespace graphics
{
    /** @brief Manages drawing and window management. Hides low-level graphical stuff to the user. */
    class Graphics
    {
        public:
            Graphics();
            ~Graphics();
            
            /*************************
             *  Window manipulation  *
             *************************/
            /** @name Window managing.
             * @{
             */
            /** @brief Opens the window of size (w*h). */
            bool openWindow(const std::string& name, int w, int h);
            /** @brief Opens the window with the fullscreen size and default depth.
             *
             * The min* arguments precise the minimal value for window size : will fail if return false if can't create a window of at least these values.
             */
            bool openFullscreenWindow(const std::string& name, int minw = 0, int minh = 0);
            /** @brief Closes the window. */
            void closeWindow();
            /** @brief Resizes the window, only valid in windowed mode.
             * @note All the loaded ressources will be lost.
             */
            bool windowSize(int width, int height);
            /** @brief Set the window fullscreen/windowed.
             * @note All the loaded ressources will be lost.
             */
            bool setFullscreen(bool fs);
            /** @brief Get all the possible resolutions above the given one. */
            std::vector<geometry::AABB> windowRes(float minw = 0.0f, float minh = 0.0f) const;
            /** @brief Returns the window height. */
            int windowWidth() const;
            /** @brief Returns the window width. */
            int windowHeight() const;
            /** @brief Returns the window depth. */
            int windowDepth() const;
            /** @brief Indicates if the window is open. */
            bool isWindowOpen() const;
            /** @} */

            /*************************
             *     Virtual size      *
             *************************/
            /** @name Virtual size handling.
             * @brief The virtual size is the size which is used when drawing, and it's automaticly scalled to fit in the window.
             * @{
             */
            /** @brief Set the virtual size. */
            void setVirtualSize(float w, float h);
            /** @brief Disable thhe virtual size.
             *
             * It is the same as setVirtualSize(windowWidth(), windowHeight()).
             */
            void disableVirtualSize();
            /** @brief Enables/disable the preservation of the ratio.
             *
             * If enabled, black bands will be added if necessary.
             */
            void preserveRatio(bool r);
            /** @brief By default, the Y axs is from top to bottom. This method allows you to invert it. */
            void invertYAxis(bool inv);
            /** @brief Indicates if the ratio is preserved. */
            bool preserveRatio() const;
            /** @brief Returns the virtual width. */
            float getVirtualWidth() const;
            /** @brief Returns the virtual height. */
            float getVirtualHeight() const;
            /** @brief Indicates if the y axis is inverted. */
            bool isYAxisInverted() const;
            /** @brief Indicates if the virtual size is different from window's. */
            bool isEnabled() const;
            /** @} */

            /*************************
             * Namespaces management *
             *************************/
            /** @name Namespace management.
             * @brief All the ressources are stored in namespaces, to prevent name conflict.
             * Namespace use is recursive : you can create a namespace in another one, and so on.
             * @{
             */
            bool enterNamespace(const std::string& name);
            bool createNamespace(const std::string& name);
            void deleteNamespace(const std::string& name);
            std::string actualNamespace() const;
            bool existsNamespace(const std::string& name) const;
            /** @} */

            /*************************
             *  Ressources loading   *
             *************************/
            /** @brief The types possible for a ressource. */
            enum RcType {
                TEXT,  /**< @brief The ressource is a texture. */
                MOVIE, /**< @brief The ressource is a movie. */
                FONT,  /**< @brief The ressource is a font. */
                NONE   /**< @brief The ressource doesn't exists or is invalid. */
            };

            /** @name Ressources management.
             * @brief All ressources are renferenced by a name, used to acces them.
             * @{
             */
            /** @brief Load a texture from a file. */
            bool loadTexture(const std::string& name, const std::string& path);
            /** @brief Load a movie from a file. */
            bool loadMovie(const std::string& name, const std::string& path);
            /** @brief Load a font from a file. */
            bool loadFont(const std::string& name, const std::string& path);
            /** @brief Load a texture from a rendered text.
             * @param font The font used to render.
             * @param txt The txt to render.
             * @param bgc The color of the background.
             * @param pts The size of the font. If negative, the default size will be used.
             * @param alpha If true, the pixel of color bgc will be transparent.
             * @param precision Precise the deviation allowed from bgc to still have alpha appliqued. Needs alpha applied.
             */
            bool loadTextureFromText(const std::string& name, const std::string& font, const std::string& txt, const Color& bgc, float pts = -1.0f, bool alpha = false, unsigned char precision = 5);
            /** @brief Free a ressource. */
            void free(const std::string& name);
            /** @brief Returns the type of a texture or NONE if it doesn't exists. */
            RcType rctype(const std::string& name) const;
            /** @brief Creates a link to the ressource which path is target. If name exists, it will be erased. */
            bool link(const std::string& name, const std::string& target);
            /** @} */

            /*************************
             *  Textures management  *
             *************************/
            /** @name Textures management.
             * @{
             */
            /** @brief Returns the width in pixel of a texture. */
            int getTextureWidth(const std::string& name) const;
            /** @brief Returns the height in pixel of a texture. */
            int getTextureHeight(const std::string& name) const;
            /** @brief Set the hotpoint of a texture. */
            bool setTextureHotpoint(const std::string& name, int x, int y);
            /** @brief Get the hotpoint of a texture. */
            geometry::Point getTextureHotPoint(const std::string& name) const;
            /** @} */

            /*************************
             *   Fonts management    *
             *************************/
            /** @name Fonts management.
             * @{
             */
            /** @brief Returns the width of a text rendered. */
            float stringWidth(const std::string& font, const std::string& str, float size = -1.0f) const;
            /** @brief Returns the size of a text rendered. */
            geometry::AABB stringSize(const std::string& font, const std::string& str, float size = -1.0f) const;
            /** @} */
            
            /*************************
             *   Movies management   *
             *************************/
            /** @name Movies management.
             * @{
             */
            /** @brief Get the speed factor of a movie (see internal::Movie::speed for more details). */
            float getMovieSpeed(const std::string& name) const;
            /** @brief Set the speed factor of a movie (see internal::Movie::speed for more details). */
            float setMovieSpeed(const std::string& name, float nspeed) const;
            /** @brief Allows the movie to be played again from the beggining. */
            void rewindMovie(const std::string& name);
            /** @} */

            /*************************
             *    Transformations    *
             *************************/
            /** @name Repere management.
             * @brief These methods allow the user to change the repere used.
             */
            /** @brief Rotate the repere, the angle is in degres. */
            void rotate(float angle);
            /** @brief Scale the repere. */
            void scale(float x, float y);
            /** @brief Translate the repere. */
            void move(float x, float y);
            /** @brief Put the repere in its initial state. */
            void identity();
            /** @brief Store the actual state of the repere. */
            void push();
            /** @brief Restore the previous state of the repere.
             * @return False if nothing is to be restored.
             */
            bool pop();
            /** @} */

            /*************************
             *       Drawing         *
             *************************/
            /** @name Drawing functions.
             * @{
             */
            /** @brief Draw a texture, taking in account its size and its hotPoint.
             * @param flip If flip, the texture is flipped horizontaly.
             */
            void blitTexture(const std::string& name, const geometry::Point& pos, bool flip = false);
            /** @brief Draw a point with specified color and width (=radius). */
            void draw(const geometry::Point& point, const Color& col, float width = -1.0f);
            /** @brief Draw a line with specified color and width (=radius). */
            void draw(const geometry::Line& line, const Color& col, float width = -1.0f);
            /** @brief Draw an AABB with a texture, possibly repeated. */
            void draw(const geometry::AABB& aabb, const std::string& text, float repeatX = 1.0f, float repeatY = 1.0f);
            /** @brief Draw an AABB with the specified color. */
            void draw(const geometry::AABB& aabb, const Color& col);
            /** @brief Draw a circle with a texture, possibly repeated. */
            void draw(const geometry::Circle& circle, const std::string& text, float repeatX = 1.0f, float repeatY = 1.0f);
            /** @brief Draw a circle with the specified color. */
            void draw(const geometry::Circle& circle, const Color& col);
            /** @brief Draw a polygon with a texture, possibly repeated. */
            void draw(const geometry::Polygon& poly, const std::string& text, float repeatX = 1.0f, float repeatY = 1.0f);
            /** @brief Draw a polygon with the specified color. */
            void draw(const geometry::Polygon& poly, const Color& col);
            /** @brief Draw a text width the specified font and size. */
            void draw(const std::string& str, const std::string& font, float pts = -1.0f);
            /** @brief Display a playing movie, or start it playing.
             * @return False when the end of the movie was reached : to continue playing, you must call rewindMovie.
             */
            bool play(const std::string& movie, const geometry::AABB& rect, bool ratio = true);
            /** @brief Set the default width used when drawing points and lines. */
            float defaultWidth(float nval);
            /** @brief Get the default width. */
            float defaultWidth() const;
            /** @brief Initialize a drawing : must be called once before all draws call. */
            void beginDraw();
            /** @brief Send the drawn to the screen : must be called once at the end of the drawing. */
            void endDraw();
            /** @} */

        private:
            SDL_Window* m_win;           /**< @brief The SDL instance of the window. */
            SDL_GLContext m_ctx;         /**< @brief The OpenGL context. */
            internal::Extensions m_exts; /**< @brief Used to manage OpenGL extensions. */
            internal::Shaders m_shads;   /**< @brief Used to manage shaders. */
            /* Virtual size */
            float m_virtualW;            /**< @brief Width of the virtual size. */
            float m_virtualH;            /**< @brief Height of the virtual size. */
            float m_appliedW;            /**< @brief The applied width of the virtual size : include borders when ratio is preserved. */
            float m_appliedH;            /**< @brief The applied height of the virtual size : include borders when ratio is preserved. */
            float m_bandWidth;           /**< @brief The width of the black bands put when preserving ratio. */
            bool m_bandLR;               /**< @brief Must the black bands be on left and right or on top and bottom. */
            bool m_virtualR;             /**< @brief Must the ratio be preserved. */
            bool m_yinvert;              /**< @brief Is the y axis inverted. */
            /* Drawing */
            float m_lineWidth;           /**< @brief The width of lines used when drawing. */

            /**************************
             *   Fake-FS structure    *
             **************************/
            /** @brief The internal repressentation of a structure. */
            struct Entity
            {
                /** @brief An union of the possible types for a ressource. */
                union Stored {
                    internal::Texture* text; /**< @brief Used if the ressource is a texture. */
                    internal::Movie* movie;  /**< @brief Used if the ressource is a movie. */
                    internal::Font* font;    /**< @brief Used if the ressource is a font. */
                };
                Stored stored; /**< @brief The value stored of the ressource. */
                RcType type;   /**< @brief The type of the value stored. */
            };

            /** @brief Functor used to free an Entity. */
            class EntityLiberator
            {
                public:
                    void operator()(Entity* tofree) const;
            };

            /** @brief All the ressources stored in an abr. */
            core::FakeFS<Entity*, EntityLiberator> m_fs;

            /**************************
             *   Internal functions   *
             **************************/
            /** @brief Log the state of the window.
             * @param open Is this the opening or a change of the window.
             * @param full Indicates if the window is fullscreen.
             * @param ended Indicates if the window could have been loaded.
             * @param sdlerr Indicates if it should log the actual sdl error.
             */
            void logWindow(bool open, bool full, bool ended, bool sdlerr = false);
            /** @brief Creates the openGL context and load the extensions and shaders. */
            bool glContext();
            /** @brief Depending on all the parameters of virtual size, compute the size and position of the black band used to preserve ratio. */
            void computeBands();
            /** @brief Log the actual virtual size state. */
            void logVirtual();
    };
}

#endif

