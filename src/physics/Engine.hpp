
#ifndef DEF_PHYSICS_ENGINE
#define DEF_PHYSICS_ENGINE

#include <map>
#include <Box2D/Box2D.h>
#include "geometry/point.hpp"
#include "Entity.hpp"

namespace physics
{
    class Engine
    {
        public:
            Engine(const b2Vec2& gravity);

            Entity* getEntity(const std::string& name) const; // Return a pointer to the entity named "name" in the engine entities map

            Entity* createEntity(const std::string& name, const geometry::Point& position, const b2BodyType& bodyType, bool fixedRotation = true); // Adds an entity to the engine's world, returning a pointer the user can use to access it (can be done later with getEntity())

            // Functions used to apply linear and angular forces and impulses to the entity named "entityName" in the engine entities map ; they just pass parameter(s) to the corresponding Entity function
            void applyForce(const std::string& entityName, const b2Vec2& force);
            void applyForce(const std::string& entityName, const b2Vec2& force, const geometry::Point& point);
            void applyLinearImpulse(const std::string& entityName, const b2Vec2& force);
            void applyLinearImpulse(const std::string& entityName, const b2Vec2& force, const geometry::Point& point);
            void applyTorque(const std::string& entityName, float force);
            void applyAngularImpulse(const std::string& entityName, float force);

        protected:
            b2World* m_world; // The world used in Box2D for simulation, containing all the bodies and fixtures (that we grouped in the Entity class)
            std::map<std::string, Entity*> m_entities; // A map containing all the entities of the world, allowing to access them with a specific name given by the user when created

    };
}

#endif
