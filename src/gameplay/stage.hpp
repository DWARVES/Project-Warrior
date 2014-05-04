
#ifndef DEF_GAMEPLAY_STAGE
#define DEF_GAMEPLAY_STAGE

#include <string>
#include "controler.hpp"
#include "events/events.hpp"
#include "physics/World.hpp"
#include "physics/Entity.hpp"
#include "geometry/aabb.hpp"
#include "lua/script.hpp"
#include "lua/stageExposure.hpp"

namespace gameplay
{
    /** @brief Represents a stage/map in the game.
     * @todo End it, it is just a beginning of implementation.
     */
    class Stage
    {
        friend lua::exposure::Stage;

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
            void setWorldCenter(const geometry::Point& c);
            void setMaxSize(const geometry::AABB& ms);
            void setDeathRect(const geometry::AABB& dr);
            void setMinSize(const geometry::AABB& ms);
            float getWindowWidth() const;
            float getWindowHeight() const;
            void setAppearPos(int id, const geometry::Point& p);
            bool addPlatform(const std::string& nm, const geometry::Point& center, const geometry::AABB& rect, float friction = 1.0f);
            bool addObstacle(const std::string& nm, const geometry::Point& center, const geometry::AABB& rect, float friction = 1.0f);
            void removeEntity(const std::string& nm);
            /** @brief Set the callbacks for an entity.
             * @param nm The entity to set the callbacks to.
             * @param begincontact The callback called when a character start touching the entity.
             * @param endcontact The callback when a character stop touching the entity.
             * @param incontact Called every frame for each character touching the entity.
             *
             * The callbacks must be lua functions which accepts one argument : the id of the character.
             * If the function name is empty or doesn't exists in the lua script, it will be ignored.
             */
            bool setEntityCallbacks(const std::string& nm, const std::string& begincontact, const std::string& endcontact, const std::string& incontact);
            void unsetEntityCallbacks(const std::string& nm);

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
            bool m_drawbg;                   /**< @brief Indicates if the lua script has a drawBG function. */
            bool m_drawstaticfg;             /**< @brief Indicates if the lua script has a drawStaticFG function. */
            bool m_drawstaticbg;             /**< @brief Indicates if the lua script has a drawStaticBG function. */
            bool m_drawfg;                   /**< @brief Indicates if the lua script has a drawFG function. */

            geometry::Point m_center;        /**< @brief The coordinates of the center of m_maxSize and m_deathRect. */
            geometry::AABB m_maxSize;        /**< @brief Characters outside this AABB won't be shown. */
            geometry::AABB m_deathRect;      /**< @brief Characters outside this AABB will die. */
            geometry::AABB m_minSize;        /**< @brief The AABB to use when only one character is shown. */
            geometry::AABB m_windowRect;     /**< @brief The window size in physic units. */

            /* Managing appearance. */
            bool m_justLoaded;               /**< @brief Is set to true by load and to false by update. */
            Uint32 m_beggining;              /**< @brief The time of the beggining of the game. */
            geometry::Point m_appearPos[4];  /**< @brief The pos of appearance of each character, setted by the script. */
            bool m_started;                  /**< @brief Setted to true when the physics are launched, false before. */
            
            /* Lua callbacks. */
            /** @brief A structure to store the callbacks for an entity. */
            struct EntityCallbacks {
                physics::Entity* ent;        /**< @brief The physic entity. */
                std::string begin;           /**< @brief The callback for the beggining of a contact. */
                std::string end;             /**< @brief The callback for the end of a contact. */
                std::string in;              /**< @brief Called every frame with the characters in contact. */
                bool charas[4];              /**< @brief Indicates which characters are in contact. */
                Stage* st;                   /**< @brief The stage, for use from the callback function. */
            };
            /** @brief A map from the entity names to their callbacks. */
            std::map<std::string, EntityCallbacks> m_callbacks;
            /** @brief The callback function passed to physics. */
            static void phcallback(physics::Entity* ground, physics::Entity* chara, bool bg, void* data);

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

