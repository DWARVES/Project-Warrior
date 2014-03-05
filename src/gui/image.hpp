
#ifndef DEF_GUI_IMAGE
#define DEF_GUI_IMAGE

#include "widget.hpp"
#include "graphics/graphics.hpp"
#include <string>

namespace gui
{
    /** @brief A widget used to display a picture. */
    class Image : public Widget
    {
        public:
            Image() = delete;
            Image(graphics::Graphics* gfx);
            /** @brief Create and load the picture.
             * @param path Path to the picture.
             * @param real If true, path will be loaded from hardisk, if not, it will be used directly from gfx.
             */
            Image(graphics::Graphics* gfx, const std::string& path, bool real = true);
            ~Image();

            /** @brief Load the picture.
             * @param path Path to the picture.
             * @param real If true, path will be loaded from hardisk, if not, it will be used directly from gfx.
             */
            void set(const std::string& path, bool real = true);
            /** @brief Enable/disable ratio respect. */
            bool ratio(bool r);
            /** @brief Indicates if the ratio respect is set. */
            bool ratio() const;

            virtual float width(float w);
            virtual float height(float h);
            virtual float width() const;
            virtual float height() const;

            virtual void draw();

        private:
            std::string m_name;             /**< @brief The name of the entity of the picture. */
            std::string m_namespace;        /**< @brief The graphics namespace of the picture. */
            bool m_ratio;                   /**< @brief Respect the ratio of the picture of not. */
            geometry::AABB m_rect;          /**< @brief The width and height used when drawing. */
            geometry::Point m_pos;          /**< @brief The position if the drawn picture. */
            static unsigned int m_count;    /**< @brief Count all allocated images. */

            /** @brief Update m_rect and m_pos. */
            void updatePosRect();
            /** @brief Allocate a namespace for this image. */
            std::string createNamespace();
    };
}

#endif

