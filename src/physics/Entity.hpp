
#ifndef DEF_PHYSICS_ENTITY
#define DEF_PHYSICS_ENTITY

#include <map>
#include <Box2D/Box2D.h>
#include "geometry/point.hpp"
#include "geometry/aabb.hpp"

namespace Physics
{
    class Entity
    {
        public:
            Entity(b2World* world, const Geometry::Point& position, const b2BodyType& bodyType, bool fixedRotation = true);

            b2Fixture* createFixture(const std::string& name, const Geometry::Point& position, const Geometry::AABB& aabb, float density, float friction);

        protected:
            b2FixtureDef* createBaseFixtureDef(float density, float friction) const;

        protected:
            b2Body* m_body;
            std::map<std::string, b2Fixture*> m_fixtures;
    };
}

#endif
