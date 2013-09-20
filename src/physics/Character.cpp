
#include "Character.hpp"

namespace physics
{
    Character::Character(b2World* world, const geometry::Point& position, const geometry::AABB& aabb, float gravityScale) : Entity(world, position, b2_dynamicBody, Entity::Type::Character, ~(Entity::Type::Platform), gravityScale, true)
    {}
}
