
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
                Normal = ~(Entity::Type::Platform),
                Ghost = ~(Entity::Type::Platform | Entity::Type::Character),
                Spectre = ~(Entity::Type::Platform | Entity::Type::Obstacle),

            } CollideType;

        public:
            Attack(const std::string& name, b2World* world, const b2BodyType& bodyType, uint16 collideWith = CollideType::Normal, float gravityScale = 1);
    };
}

#endif
