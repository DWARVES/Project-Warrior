
#include "Obstacle.hpp"

namespace physics
{
    Obstacle::Obstacle(const std::string& name, World* world, const geometry::Point& position) : Entity(name, world, position, b2_staticBody, Entity::Type::Obstacle)
    {}

    Obstacle::Obstacle(const std::string& name, World* world, const geometry::Point& position, const geometry::AABB& rect, float friction) : Entity(name, world, position, b2_staticBody, Entity::Type::Obstacle)
    {
        createFixture("body", rect, 1, friction);
    }
}
