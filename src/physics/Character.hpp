
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
            Character(const std::string& name, b2World* world, const geometry::Point& position, const geometry::AABB& rect, float weight);

            /** @brief Make jump the character at a speed based on vel and his weight */
            void jump(float vel);
    };
}

#endif
