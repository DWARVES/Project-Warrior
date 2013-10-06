
#ifndef DEF_PHYSICS_PLATFORM
#define DEF_PHYSICS_PLATFORM

#include "Entity.hpp"

namespace physics
{
    class Platform : public Entity
    {
        public:
            Platform(const std::string& name, b2World* world, const geometry::Point& position); // Create an empty platform (without fixture)
            Platform(const std::string& name, b2World* world, const geometry::Point& position, const geometry::AABB& rect, float friction = 1); // Create a basic platform with AABB fixture
    };
}

#endif
