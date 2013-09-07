
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

            Entity* getEntity(const std::string& name) const;

            Entity* createEntity(const std::string& name, const geometry::Point& position, const b2BodyType& bodyType, bool fixedRotation = true);

            void applyForceToCenter(const std::string& entityName, const b2Vec2& force);
            void applyForceToPoint(const std::string& entityName, const b2Vec2& force, const geometry::Point& position);

        protected:
            b2World* m_world;
            std::map<std::string, Entity*> m_entities;
    };
}

#endif
