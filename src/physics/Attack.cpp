
#include "Attack.hpp"

namespace physics
{
    Attack::Attack(const std::string& name, b2World* world, const geometry::Point& position, const b2BodyType& bodyType, uint16 collideWith, float gravityScale, bool fixedRotation) : Entity(name, world, position, bodyType, Entity::Type::Attack, collideWith, gravityScale, fixedRotation)
    {}
}
