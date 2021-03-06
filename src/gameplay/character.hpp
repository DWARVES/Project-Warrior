
#ifndef DEF_GAMEPLAY_CHARACTER
#define DEF_GAMEPLAY_CHARACTER

#include <string>
#include <list>
#include "geometry/aabb.hpp"
#include "lua/script.hpp"
#include "physics/World.hpp"
#include "physics/Character.hpp"

namespace gameplay
{
    /** @brief Reprensents a playable character.
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

            /** @brief Load the character main script and rcs for a specified color.
             * @param nb The id of the character in [0;3].
             */
            bool load(Color c, int nb);
            /** @brief The controls that can be sent to the character. */
            enum Control {
                Walk,   /**< @brief The walk control. */
                Run,    /**< @brief The run control. */
                Attack, /**< @brief The basic attack control. */
                Spell,  /**< @brief The magic attack control. */
                Smash,  /**< @brief The smash control. */
                Dodge,  /**< @brief The dodge control. */
                Catch,  /**< @brief The catch control. */
            };
            /** @brief The directions of the controls. */
            enum Direction {
                Left,      /**< @brief The control is left oriented. */
                Right,     /**< @brief The control is right oriented. */
                Up,        /**< @brief The control is top oriented. */
                Down,      /**< @brief The control is bottom oriented. */
                Fixed,     /**< @brief The control has no orientation. */
                TurnLeft,  /**< @brief Going from right to left. */
                TurnRight, /**< @brief Going from left to right. */
            };
            /** @brief Send an action to the character. Must be called on every frame.
             * @param control The control or action the character must do.
             * @param dir     The direction of the control.
             */
            void action(Control control, Direction dir);
            /** @brief Warp the character. */
            void warp(const geometry::Point& p);
            /** @brief Enable/disable a maximum size when drawing. */
            void enableMSize(bool en, const geometry::AABB& ms = geometry::AABB(0.0f,0.0f));
            /** @brief Sets the msize to the physic size, with a fact.
             * @param hgh If true, the size limitation if only on the height.
             */
            void physicMSize(float fact = 1.0f, bool hgh = false);
            /** @brief Set the default direction and the one to flip : false = left normal. */
            void setFlip(bool f);
            /** @brief Draw the character at the position setted by its physics (in the stage repere). */
            void draw();
            /** @brief Indicates if the character is flipped when drawn. */
            bool flipped() const;
            /** @brief Draw the character appearing.
             * @param percent The percentage of the animation [0-100].
             * @param msize The maximum size.
             */
            void appear(float percent, const geometry::AABB& msize);
            /** @brief The animation if the character won, with a maximum size. */
            void lost(const geometry::AABB& msize);
            /** @brief The animation if the character lost, with a maximum size. */
            void won(const geometry::AABB& msize);

            /** @brief Create the character in the physics world. */
            bool world(physics::World* w);
            /** @brief Get the physics::World used. */
            physics::World* world() const;
            /** @brief Get the physic entity. */
            physics::Entity* entity() const;
            /** @brief Get the coordinates of the character. */
            geometry::Point getPos() const;
            /** @brief Defines the position to appear. Must be called before world. */
            void appearancePos(const geometry::Point& pos);
            /** @brief Defines the physic size of the character. */
            void phSize(const geometry::AABB& size);
            /** @brief Get the physic size of the character. */
            geometry::AABB phSize() const;
            /** @brief Defines the physic weight of the character. */
            void phWeight(float w);
            /** @brief Defines the maximum mana of the character. */
            void setManaMax(unsigned int m);
            /** @brief Set the speed of mana recovering in mana units per ms. */
            void setManaRecov(float m);
            /** @brief Indicates if the character is onGround. */
            bool onGround() const;
            /** @brief Use mana from the character, return false if there were not enough mana. */
            bool requireMana(unsigned int mn);
            /** @brief Add mana to the actual level of mana of the character. */
            void addMana(unsigned int mn);
            /** @brief Get the mana left. */
            unsigned int mana() const;
            /** @brief Get the maximum mana of the character. */
            unsigned int manaMax() const;
            /** @brief Indicates the percent of mana left. */
            float manaProgress() const;
            /** @brief Get the mana recovering speed. */
            float getManaRecov() const;

            /** @brief Allocate another character with the same path. */
            Character* clone() const;

            /** @brief Inflict damages to a character. */
            void inflictDamages(int dm, Character* from = NULL);
            /** @brief Get the damages of the character. */
            int getDamages() const;
            /** @brief Add points to the character. */
            void addPoints(int pts);
            /** @brief Get the amount of points of the character. */
            int getPoints() const;
            /** @brief Indicates a character he is dead : reset its coordinates of damages, after a laps of time. */
            void die();
            /** @brief Indicates if a character is dead. */
            bool dead();

            /******* CC *******/
            /** @brief Inflict a stun of ms millis to the character. */
            void stun(unsigned int ms, Character* from = NULL);
            /** @brief Indicates the progress of the stun from 0 to 1 (not stunned). */
            float stunProgress() const;
            /** @brief Inflict an impact on the character. */
            void impact(float x, float y, Character* from = NULL, bool fixe = false);

            /* Methods exposed to lua. */
            bool createAttack(const geometry::AABB& rect, bool physic, const std::string& mvx, const std::string& mvy, const std::string& drw, const std::string& contact, bool gravity = false);

        private:
            std::string m_namespace;  /**< @brief The name of the namespace used by this character in gfx, audio and physics. */
            std::string m_path;       /**< @brief The path to the directory of the character. */
            static size_t m_count;    /**< @brief The number of created characters. */

            lua::Script m_preview;    /**< @brief The preview.lua script. */
            std::string m_name;       /**< @brief The name of the character. */
            std::string m_desc;       /**< @brief The description of the character. */
            bool m_valid;             /**< @brief Has the character been validated. */

            lua::Script m_perso;      /**< @brief The perso.lua script. */
            /** @brief Describe the possible actions of the character. */
            enum class ActionID : unsigned int {
                Walk,                 /**< @brief Walk. */
                Run,                  /**< @brief Run. */
                Stop,                 /**< @brief Stop after running. */
                Jump,                 /**< @brief Jump. */
                JumpAir,              /**< @brief Do a double jump. */
                Down,                 /**< @brief Get down from midair. */
                FastDown,             /**< @brief Get down from midair fast. */
                Up,                   /**< @brief Going up without jumping. */
                Land,                 /**< @brief After going down, land on ground. */
                Stand,                /**< @brief Stay witheout moving. */
                Shield,               /**< @brief Protect itself with the shield. */
                StaticDodge,          /**< @brief A static dodge on the ground. */
                FlyingStaticDodge,    /**< @brief A static dodge in the air. */
                DashDodge,            /**< @brief Dodge by dashing to left on the ground. */
                FlyingDashDodge,      /**< @brief Dodge by dashing to left in the air. */
                Attack,               /**< @brief A regular non directional attack. */
                AttackSide,           /**< @brief A regular attack to the side. */
                AttackUp,             /**< @brief A regular attack to top. */
                AttackDown,           /**< @brief A regular attack to bottom. */
                AttackAir,            /**< @brief A regular attack on the air. */
                AttackAirFront,       /**< @brief A regular attack on the air to the front. */
                AttackAirBack,        /**< @brief A regular attack on the air to the back. */
                AttackAirUp,          /**< @brief A regular attack on the air to the top. */
                AttackAirDown,        /**< @brief A regular attack on the air to the bottom. */
                Spell,                /**< @brief A non directional magic attack. */
                SpellSide,            /**< @brief A magic attack to the side. */
                SpellDown,            /**< @brief A magic attack to bottom. */
                SpellUp,              /**< @brief A magic attack to top. */
                SmashSide,            /**< @brief A smash to the side. */
                SmashUp,              /**< @brief A smash to top. */
                SmashDown,            /**< @brief A smash to bottom. */
                Catch,                /**< @brief Catch something. */
                Stunned,              /**< @brief When the character is stunned. */
                Won,                  /**< @brief The victory animation. */
                Lost,                 /**< @brief The defeat animation. */
                None,                 /**< @brief No action. */
            };
            float m_stir;             /**< @brief Indicates to actuateByPhysics the value of a lateral force to apply. */
            /** @brief Describes an action. */
            struct Action {
                ActionID id;          /**< @brief The ID of the action. */
                bool flip;            /**< @brief If it is a side action, must it be flipped horizontally. */
            };
            Action m_actual;          /**< @brief The action the character is actually doing. */
            Action m_next;            /**< @brief The action the character will do once m_actual is finished. */
            bool m_doubleJump;        /**< @brief Has used its double jump. */
            unsigned long m_begin;    /**< @brief The timestamp of the actual action. */
            bool m_useMsize;          /**< @brief Must the action be drawn with a maximum size. */
            geometry::AABB m_msize;   /**< @brief The maximum size used when drawing. */
            bool m_flip;              /**< @brief Must the picture be flipped when drawing to the left. */

            /** @brief Link an actionID to the corresponding lua function. */
            static const char* const m_luaCalls[(unsigned int)ActionID::None];

            /* Mana. */
            unsigned int m_mana;      /**< @brief The mana left of the character. */
            unsigned int m_manamax;   /**< @brief The maximum mana of the character. */
            float m_manaRecov;        /**< @brief Mana recovering speed. */
            Uint32 m_lastReload;      /**< @brief The timestamp of the previous mana reload. */

            /* Physics integration. */
            physics::World* m_world;  /**< @brief The physics world. */
            physics::Character* m_ch; /**< @brief The physics entity of the character. */
            geometry::AABB m_phsize;  /**< @brief The size of the entity in the physic world. */
            float m_phweight;         /**< @brief The weight of the entity on the physic world. */
            geometry::Point m_phpos;  /**< @brief The position of appearance of the character in the physics world. */

            /* Points and damages. */
            int m_damages;            /**< @brief The amount of damages of the character. */
            int m_points;             /**< @brief The points of the character. */
            Uint32 m_death;           /**< @brief Last time of death of the character. */
            bool m_dead;              /**< @brief Indicates if the character is dead. */
            Character* m_lastAtt;     /**< @brief The last character who attacked this one. */
            Uint32 m_lastAttTime;     /**< @brief The timestamp of the last attack to this character. */

            /* CCs */
            bool m_stuned;            /**< @brief Indicates if the character is stuned. */
            Uint32 m_stun;            /**< @brief Indicates the beggining of the stun. */
            Uint32 m_stunTime;        /**< @brief Indicates the duration of the stun. */
            
            /* Attacks */
            /** @brief Structure used to store attacks. */
            struct AttackSt {
                std::string name;     /**< @brief The physic name of the entity/fixture. */
                std::string moveX;    /**< @brief The lua callback moving the attack (returns its move along X axis). It takes as argument the number os ms since the creating of the attack. */
                std::string moveY;    /**< @brief The lua callback moving the attack (returns its move along Y axis). It takes as argument the number os ms since the creating of the attack. */
                std::string draw;     /**< @brief The lua callback drawing the attack. It takes as argument the number os ms since the creating of the attack. */
                std::string contact;  /**< @brief The lua callback called when a character touches the attack. It takes as argument the number os ms since the creating of the attack. */
                bool physic;          /**< @brief Indicates if it is a magic attack or a physic attack. */
                geometry::AABB rect;  /**< @brief The collison rect. */
                Uint32 begin;         /**< @brief The local timestamp of the creation of the attack. */
                Character* ch;        /**< @brief Pointer to the character calling. */
                bool ended;           /**< @brief Indicates if the attack must be deleted. */
                bool flip;            /**< @brief Must the attacks be flipped. */
            };
            /**< @brief The list of all the attacks occuring. */
            std::list<AttackSt> m_attacks;  
            int m_attackCount;        /**< @brief Count the number of attacks created. */

            /* Internal methods. */
            /** @brief Draw the nm texture with a maximum size, can flip it horizontally. */
            void drawPrev(const std::string& nm, const geometry::AABB& msize, bool flip = false, bool hp = false) const;
            /** @brief Check the existence of a function in a script, and log a warning on the contrary.
             * @param script The script.
             * @param nm     The name of the script for logging.
             * @param func   The function which existence will be tested.
             * @return False if the function wasn't found.
             */
            bool checkFunc(lua::Script& script, const std::string& nm, const std::string& func);
            /** @brief Call the right lua function depending on the actual action. */
            void actuateByLua();
            /** @brief Apply the physic action depending on the control. */
            void actuateByPhysic(Action actual, Action previous);
            /** @brief Call the lua callbacks about the attacks. */
            void attackCallbacks();
            /** @brief The callback for the collision with attacks. */
            static void attackcallback(physics::Entity*, physics::Entity* chara, bool bg, void* data);
            /** @brief Remove an attack callback. */
            static void removeAttack(const AttackSt& st);
    };
}

#endif

