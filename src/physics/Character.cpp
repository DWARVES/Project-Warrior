
#include "Character.hpp"

namespace physics
{
    Character::Character(const std::string& name, b2World* world, const geometry::Point& position, const geometry::AABB& size, float weight) : Entity(name, world, position, b2_dynamicBody, Entity::Type::Character, ~(Entity::Type::Platform), 1, true)
    {
        createFixture("body", size, weight, 1);
    }

    void Character::jump(float vel)
    {
        setYLinearVelocity(vel * (1 - (getFixture("body")->GetDensity() - 1)));
    }
}
