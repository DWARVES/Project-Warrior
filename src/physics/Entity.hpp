
#ifndef DEF_PHYSICS_ENTITY
#define DEF_PHYSICS_ENTITY

#include <map>
#include <Box2D/Box2D.h>
#include "geometry/point.hpp"
#include "geometry/line.hpp"
#include "geometry/aabb.hpp"
#include "geometry/circle.hpp"
#include "geometry/polygon.hpp"

namespace physics
{
    class Entity
    {
        public:
            Entity();
            Entity(b2World* world, const geometry::Point& position, const b2BodyType& bodyType, bool fixedRotation = true);

            b2Fixture* createFixture(const std::string& name, const geometry::Line& line, float density, float friction);
            b2Fixture* createFixture(const std::string& name, const geometry::AABB& aabb, float density, float friction, const geometry::Point& position = geometry::Point(0, 0));
            b2Fixture* createFixture(const std::string& name, const geometry::Circle& circle, float density, float friction, const geometry::Point& position = geometry::Point(0, 0));
            b2Fixture* createFixture(const std::string& name, const geometry::Polygon& polygon, float density, float friction);

        protected:
            b2FixtureDef* createBaseFixtureDef(float density, float friction) const;

        protected:
            b2Body* m_body;
            std::map<std::string, b2Fixture*> m_fixtures;
    };
}

#endif
