
#ifndef DEF_GAMEPLAY_STAGE
#define DEF_GAMEPLAY_STAGE

#include <string>
#include "geometry/aabb.hpp"

namespace gameplay
{
    /** @brief Represents a stage/map in the game.
     * @todo End it, it is just a beginning of implementation.
     */
    class Stage
    {
        public:
            Stage() = delete;
            Stage(const Stage&) = delete;
            Stage(const std::string& path);
            ~Stage();

            /** @brief Load the preview of the stage. */
            bool preload();
            /** @brief Indicates if the stage has been validated. */
            bool validated() const;
            /** @brief Returns the name of the stage. */
            std::string name() const;
            /** @brief Draw the preview of the stage in an AABB. */
            void draw(const geometry::AABB& rect) const;

        private:
            static int m_count;      /**< @brief Count of all stages. */
            std::string m_path;      /**< @brief Path to the directory of the stage. */
            std::string m_namespace; /**< @brief The namespace used by the stage. */
            std::string m_name;      /**< @brief The name of the stage. */
            bool m_valid;            /**< @brief Has the stage been validated. */

            /** @brief Create and return the namespace used. */
            std::string getNamespace();
    };
}

#endif

