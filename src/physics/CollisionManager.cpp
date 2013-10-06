
#include "CollisionManager.hpp"

namespace physics
{
    void CollisionManager::BeginContact(b2Contact* contact)
    {
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        Entity* entityA = getEntityFromFixture(fixtureA);
        Entity* entityB = getEntityFromFixture(fixtureB);

        if(entityA == NULL || entityB == NULL)
        {
            return;
        }

        // Platform collision management

        // Check if one of the fixtures is the platform
        b2Body* platformBody = NULL;
        b2Body* otherBody = NULL;
        if(entityA->getType() == Entity::Type::Platform) {
            platformBody = fixtureA->GetBody();
            otherBody = fixtureB->GetBody();
        }
        if(entityB->getType() == Entity::Type::Platform) {
            platformBody = fixtureB->GetBody();
            otherBody = fixtureA->GetBody();
        }
        if(!platformBody)
            return;

        int numPoints = contact->GetManifold()->pointCount;
        b2WorldManifold worldManifold;
        contact->GetWorldManifold(&worldManifold);

        // Check if contact points are moving downward
        for(int i = 0; i < numPoints; i++) {
            b2Vec2 pointVel = otherBody->GetLinearVelocityFromWorldPoint(worldManifold.points[i]);
            if(pointVel.y < 0)
                return; // Point is moving down, leave contact solid and exit
        }

        // No points are moving downward, contact should not be solid
        contact->SetEnabled(false);
    }

    void CollisionManager::EndContact(b2Contact* contact)
    {
        // Platform collision management

        // Reset the default state of the contact in case it comes back for more
        contact->SetEnabled(true);
    }

    Entity* CollisionManager::getEntityFromFixture(b2Fixture* fixture) const
    {
        void* bodyUserData = fixture->GetBody()->GetUserData();
        if(bodyUserData) {
            return static_cast<Entity*>(bodyUserData);
        }
        else
            return NULL;
    }
}
