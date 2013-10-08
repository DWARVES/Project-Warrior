
#ifndef DEF_PHYSICS_OBSTACLE
#define DEF_PHYSICS_OBSTACLE

#include "Entity.hpp"

namespace physics
{
    class Obstacle : public Entity
    {
        public:
            Obstacle(const std::string& name, b2World* world, const geometry::Point& position); // Create an empty obstacle (without fixture)
            Obstacle(const std::string& name, b2World* world, const geometry::Point& position, const geometry::AABB& rect, float friction = 1); // Create a basic obstacle with AABB fixture
    };
}

#endif
