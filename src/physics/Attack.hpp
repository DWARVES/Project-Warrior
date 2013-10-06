
#ifndef DEF_PHYSICS_ATTACK
#define DEF_PHYSICS_ATTACK

#include "Entity.hpp"

namespace physics
{
    class Attack : public Entity
    {
        public:
            typedef enum
            {
                Normal = Entity::Type::All,
                Ghost = ~(Entity::Type::Character),
                Spectre = ~(Entity::Type::Obstacle)

            } CollideType;

        public:
            Attack(const std::string& name, b2World* world, const geometry::Point& position, const b2BodyType& bodyType, uint16 collideWith = CollideType::Normal, float gravityScale = 1, bool fixedRotation = true);
    };
}

#endif
