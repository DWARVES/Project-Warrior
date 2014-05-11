
#ifndef DEF_PHYSICS_CHARACTER
#define DEF_PHYSICS_CHARACTER

#include "Entity.hpp"

namespace physics
{
    /** @brief Entity based class facilitating creation of characters */
    class Character : public Entity
    {
        public:
            /** @brief Standard float values used to define a character's weight */
            class Weight
            {
                public:
                    constexpr static float 
                        Lightweight = 0.85f,
                        Medium = 1.0f,
                        Heavy = 1.15f;
            };

        public:
            /** @brief Constructor defining some attributes and physical parameters ; should not be used directly to instanciate a b2Body in a b2World, but only through World::createCharacter method */
            Character(const std::string& name, World* world, const geometry::Point& position, const geometry::AABB& rect, float weight);
            virtual ~Character();

            /** @brief Make jump the character at a speed based on vel and his weight */
            void jump(float vel);
            /** @brief Indicates if the character is on the ground. */
            bool onGround() const;

            /** @brief Setting the id of the character. */
            void setID(int id);
            /** @brief Getting the id of the character. */
            int getID() const;

        private:
            std::vector<Entity*> m_underfoot; /**< @brief A list of all entities under the player foot. */
            World* m_world;
            int m_id; /**< @brief The ID of the character. */

            /** @brief the callback used by Character to keep track of the ground. */
            static void character_foot_callback(Entity* ch, Entity* ground, bool start, void*);
    };
}

#endif
