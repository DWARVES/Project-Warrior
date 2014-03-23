
#include "Platform.hpp"

namespace physics
{
    Platform::Platform(const std::string& name, World* world, const geometry::Point& position) : Entity(name, world, position, b2_staticBody, Entity::Type::Platform)
    {}

    Platform::Platform(const std::string& name, World* world, const geometry::Point& position, const geometry::AABB& rect, float friction) : Entity(name, world, position, b2_staticBody, Entity::Type::Platform)
    {
        createFixture("body", rect, 1, friction);
    }
}
