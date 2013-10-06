
#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "Box2D/Box2D.h"
#include "Entity.hpp"

namespace physics
{
    class CollisionManager : public b2ContactListener
    {
        void BeginContact(b2Contact* contact);
        void EndContact(b2Contact* contact);

        Entity* getEntityFromFixture(b2Fixture* fixture) const;
    };
}

#endif
