
#ifndef DEF_PHYSICS_OBSTACLE
#define DEF_PHYSICS_OBSTACLE

#include "Entity.hpp"

namespace physics
{
    /** @brief Entity based class facilitating creation of obstacles */
    class Obstacle : public Entity
    {
        public:
            /** @brief Creates an empty platform (without fixture) */
            Obstacle(const std::string& name, World* world, const geometry::Point& position);
            /** @brief Creates a basic obstacle with AABB fixture */
            Obstacle(const std::string& name, World* world, const geometry::Point& position, const geometry::AABB& rect, float friction = 1);
    };
}

#endif
