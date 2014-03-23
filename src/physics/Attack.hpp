
#ifndef DEF_PHYSICS_ATTACK
#define DEF_PHYSICS_ATTACK

#include "Entity.hpp"

namespace physics
{
    /** @brief Entity based class facilitating creation of attacks */
    class Attack : public Entity
    {
        public:
            /** @brief Standard collision masks for attacks, including Normal, Ghost and Spectre */
            typedef enum
            {
                Normal = Entity::Type::All,
                Ghost = ~(Entity::Type::Character),
                Spectre = ~(Entity::Type::Obstacle | Entity::Type::Platform)

            } CollideType;

        public:
            /** @brief Constructor defining some attributes and physical parameters ; should not be used directly to instanciate a b2Body in a b2World, but only through World::createAttack method */
            Attack(const std::string& name, World* world, const geometry::Point& position, const b2BodyType& bodyType, uint16 collideWith = CollideType::Normal, float gravityScale = 1, bool fixedRotation = true);
    };
}

#endif
