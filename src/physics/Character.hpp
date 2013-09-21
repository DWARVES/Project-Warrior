
#ifndef DEF_PHYSICS_CHARACTER
#define DEF_PHYSICS_CHARACTER

#include "Entity.hpp"

namespace physics
{
    class Character : public Entity
    {
        public:
            class Weight
            {
                public:
                    constexpr static float 
                        Lightweight = 0.9f,
                        Medium = 1.0f,
                        Heavy = 1.1f;
            };

        public:
            Character(const std::string& name, b2World* world, const geometry::Point& position, const geometry::AABB& size, float weight);
    };
}

#endif
