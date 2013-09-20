
#ifndef DEF_PHYSICS_CHARACTER
#define DEF_PHYSICS_CHARACTER

#include "Entity.hpp"

namespace physics
{
    class Character : public Entity
    {
        public:
            Character(b2World* world, const geometry::Point& position, float gravityScale);
    };
}

#endif
