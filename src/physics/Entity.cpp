
#include "Entity.hpp"
#include "unit_conversions.hpp"

namespace physics
{
    Entity::Entity(b2World* world, const geometry::Point& position, const b2BodyType& bodyType, bool fixedRotation)
    {
        b2BodyDef bodyDef;
        bodyDef.type = bodyType;
        bodyDef.position = b2Vec2(toMeters(position.x), toMeters(position.y));

        m_body = world->CreateBody(&bodyDef);
        m_body->SetFixedRotation(fixedRotation);
    }

    b2Fixture* Entity::createFixture(const std::string& name, const geometry::Point& position, const geometry::AABB& aabb, float density, float friction)
    {
        b2FixtureDef* fixtureDef = createBaseFixtureDef(density, friction);
        
        b2PolygonShape shape;
        shape.SetAsBox(toMeters(aabb.width / 2), toMeters(aabb.height / 2), b2Vec2(toMeters(position.x), toMeters(position.y)), 0);
        fixtureDef->shape = &shape;

        m_fixtures[name] = m_body->CreateFixture(fixtureDef);
        return m_fixtures[name];
    }

    b2FixtureDef* Entity::createBaseFixtureDef(float density, float friction) const
    {
        b2FixtureDef *fixtureDef = new b2FixtureDef;
        fixtureDef->density = density;
        fixtureDef->friction = friction;

        return fixtureDef;
    }
}
