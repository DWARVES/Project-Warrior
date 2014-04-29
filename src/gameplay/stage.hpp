
#ifndef DEF_GAMEPLAY_STAGE
#define DEF_GAMEPLAY_STAGE

#include <string>
#include "controler.hpp"
#include "events/events.hpp"
#include "physics/World.hpp"
#include "geometry/aabb.hpp"
#include "lua/script.hpp"

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

            /** @brief Load the stage for its use.
             * The valid ctrls will be deleted when the stage is free'd.
             */
            bool load(gameplay::Controler* ctrls[4]);
            /** @brief Update the stage. */
            void update(const events::Events& ev);
            /** @brief Draw the stage. */
            void draw();

            
        private: /* The methods to expose to lua. */
            /* TODO */

        private:
            static int m_count;              /**< @brief Count of all stages. */
            std::string m_path;              /**< @brief Path to the directory of the stage. */
            std::string m_namespace;         /**< @brief The namespace used by the stage. */
            std::string m_name;              /**< @brief The name of the stage. */
            bool m_valid;                    /**< @brief Has the stage been validated. */

            physics::World m_world;          /**< @brief The physic world used. */
            gameplay::Controler* m_ctrls[4]; /**< @brief The controlers used. */
            int m_nbPlayers;                 /**< @brief The number of players [1-4]. */
            lua::Script m_script;            /**< @brief The lua script. */

            geometry::Point m_center;        /**< @brief The coordinates of the center of m_maxSize and m_deathRect. */
            geometry::AABB m_maxSize;        /**< @brief Characters outside this AABB won't be shown. */
            geometry::AABB m_deathRect;      /**< @brief Characters outside this AABB will die. */
            geometry::AABB m_minSize;        /**< @brief The AABB to use when only one character is shown. */
            geometry::AABB m_windowRect;     /**< @brief The window size in physic units. */

            /* Managing appearance. */
            bool m_justLoaded;               /**< @brief Is set to true by load and to false by update. */
            Uint32 m_beggining;              /**< @brief The time of the beggining of the game. */
            geometry::AABB m_appearSize;     /**< @brief The size of appearance, setted by the script. */
            geometry::Point m_appearPos[4];  /**< @brief The pos of appearance of each character, setted by the script. */
            geometry::AABB m_appearView;     /**< @brief The AABB to view during appearance. */

            /** @brief Create and return the namespace used. */
            std::string getNamespace();
            /** @brief Center the view on the character shown. */
            void centerView();
            /** @brief Make a rect fit/englobe another one with ratio respect.
             * @param res   The AABB to resize.
             * @param fit   The AABB to englobe/fit in.
             * @param large Must englobe (true) or fit in (false).
             */
            std::pair<geometry::AABB,geometry::Point> ratioResize(const geometry::AABB& res, const geometry::AABB& fit, bool large = false) const;
            /** @brief Check if a coordinate is in a rect. */
            bool isIn(const geometry::Point& p, const geometry::AABB& rect, const geometry::Point& center) const;
    };
}

#endif

