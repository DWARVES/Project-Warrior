
#ifndef DEF_PHYSICS_WORLD
#define DEF_PHYSICS_WORLD

#include <map>
#include <Box2D/Box2D.h>
#include "geometry/point.hpp"
#include "Entity.hpp"

namespace physics
{
    class World
    {
        public:
            World(const b2Vec2& gravity);

            Entity* getEntity(const std::string& name) const; // Return a pointer to the entity named "name" in the world entities map

            Entity* createEntity(const std::string& name, const geometry::Point& position, const b2BodyType& bodyType, bool fixedRotation = true); // Adds an entity to the world, returning a pointer the user can use to access it (can be done later with getEntity())

        protected:
            b2World* m_world; // The world used in Box2D for simulation, containing all the bodies and fixtures (that we grouped in the Entity class)
            std::map<std::string, Entity*> m_entities; // A map containing all the entities of the world, allowing to access them with a specific name given by the user when created

    };
}

#endif
