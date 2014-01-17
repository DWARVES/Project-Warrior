
#ifndef DEF_GAMEPLAY_CHARACTER
#define DEF_GAMEPLAY_CHARACTER

#include <string>
#include "geometry/aabb.hpp"
#include "lua/script.hpp"

namespace gameplay
{
    /** @brief Reprensents a playable character.
     * @todo Physic integration.
     * @todo Audio integration.
     */
    class Character
    {
        public:
            /** @brief Initialize the character. Doesn't load anything.
             * @param path The path to the directory of this caracter.
             */
            Character(const std::string& path);
            Character() = delete;
            Character(const Character&) = delete;
            ~Character();

            /** @brief The colors possibles for a character. */
            enum Color : int {
                Red   = 0, /**< @brief The red theme of the character. */
                Blue  = 1, /**< @brief The blue theme of the character. */
                Green = 2, /**< @brief The green theme of the character. */
                White = 3, /**< @brief The white theme of the character. */
                Black = 4, /**< @brief The black theme of the character. */
                None  = 5, /**< @brief None means the default color of the character. */
            };

            /** @brief Load the preview, the name and the description. */
            bool preload();
            /** @brief Check if the character has been validated. */
            bool validated() const;
            /** @brief Returns the name of the character. */
            std::string name() const;
            /** @brief Returns the description of the character. */
            std::string desc() const;
            /** @brief Print the preview picture.
             * @param msize The max size of the picture.
             */
            void preview(const geometry::AABB& msize) const;
            /** @brief Print the big preview picture.
             * @param color The color of the character.
             * @param msize the max size of the picture.
             */
            void bigPreview(Color color, const geometry::AABB& msize);

            /** @brief Load the character main script and rcs for a specified color. */
            bool load(Color c);
            /** @brief The controls that can be sent to the character. */
            enum Control {
                Walk,   /**< @brief The walk control. */
                Run,    /**< @brief The run control. */
                Attack, /**< @brief The basic attack control. */
                Spell,  /**< @brief The magic attack control. */
                Smash,  /**< @brief The smash control. */
                Dodge,  /**< @brief The static dodge control. */
                DDodge, /**< @brief The dash dodge control. */
                Catch,  /**< @brief The catch control. */
            };
            /** @brief The directions of the controls. */
            enum Direction {
                Left,   /**< @brief The control is left oriented. */
                Right,  /**< @brief The control is right oriented. */
                Up,     /**< @brief The control is top oriented. */
                Down,   /**< @brief The control is bottom oriented. */
                Fixed,  /**< @brief The control has no orientation. */
            };
            /** @brief Send an action to the character.
             * @param control The control or action the character must do.
             * @param dir     The direction of the control.
             */
            void action(Control control, Direction dir);
            /** @brief Draw the character at the position setted by its physics (in the stage repere). */
            void draw();

        private:
            std::string m_namespace;  /**< @brief The name of the namespace used by this character in gfx and audio. */
            std::string m_path;       /**< @brief The path to the directory of the character. */
            static size_t m_count;    /**< @brief The number of created characters. */

            lua::Script m_preview;    /**< @brief The preview.lua script. */
            std::string m_name;       /**< @brief The name of the character. */
            std::string m_desc;       /**< @brief The description of the character. */
            bool m_valid;             /**< @brief Has the character been validated. */

            lua::Script m_perso;      /**< @brief The perso.lua script. */
            /** @brief Describe the possible actions of the character. */
            enum class Action {
                WalkLeft,             /**< @brief Walk to the left. */
                WalkRight,            /**< @brief Walk to the right. */
                RunLeft,              /**< @brief Run to the left. */
                RunRight,             /**< @brief Run to the right. */
                Stop,                 /**< @brief Stop after running. */
                Jump,                 /**< @brief Jump. */
                JumpAir,              /**< @brief Do a double jump. */
                Down,                 /**< @brief Get down from midair. */
                FastDown,             /**< @brief Get down from midair fast. */
                Land,                 /**< @brief After going down, land on ground. */
                Stand,                /**< @brief Stay witheout moving. */
                Shield,               /**< @brief Protect itself with the shield. */
                StaticDodge,          /**< @brief A static dodge on the ground. */
                FlyingStaticDodge,    /**< @brief A static dodge in the air. */
                DashDodgeLeft,        /**< @brief Dodge by dashing to left on the ground. */
                DashDodgeRight,       /**< @brief Dodge by dashing to right on the ground. */
                FlyingDashDodgeLeft,  /**< @brief Dodge by dashing to left in the air. */
                FlyingDashDodgeRight, /**< @brief Dodge by dashing to right in the air. */
                Attack,               /**< @brief A regular non directional attack. */
                AttackLeft,           /**< @brief A regular attack to the left. */
                AttackRight,          /**< @brief A regular attack to the right. */
                AttackUp,             /**< @brief A regular attack to top. */
                AttackDown,           /**< @brief A regular attack to bottom. */
                Spell,                /**< @brief A non directional magic attack. */
                SpellLeft,            /**< @brief A magic attack to the left. */
                SpellRight,           /**< @brief A magic attack to the right. */
                SpellDown,            /**< @brief A magic attack to bottom. */
                SpellUp,              /**< @brief A magic attack to top. */
                SmashLeft,            /**< @brief A smash to the left. */
                SmashRight,           /**< @brief A smash to the right. */
                SmashUp,              /**< @brief A smash to top. */
                SmashDown,            /**< @brief A smash to bottom. */
                CatchLeft,            /**< @brief Catch something on the left. */
                CatchRight,           /**< @brief Catch something on the right. */
                None,                 /**< @brief No action. */
            };
            Action m_actual;          /**< @brief The action the character is actually doing. */
            Action m_next;            /**< @brief The action the character will do once m_actual is finished. */
            unsigned long m_begin;    /**< @brief The timestamp of the actual action. */

            /* Internal methods. */
            /** @brief Draw the "preview" texture with a maximum size. */
            void drawPrev(const geometry::AABB& msize) const;
            /** @brief Check the existence of a function in a script, and log a warning on the contrary.
             * @param script The script.
             * @param nm     The name of the script for logging.
             * @param func   The function which existence will be tested.
             * @return False if the function wasn't found.
             */
            bool checkFunc(lua::Script& script, const std::string& nm, const std::string& func);
            /** @brief Call the right lua function depending on the actual action. */
            void actuateByLua();
    };
}

#endif

