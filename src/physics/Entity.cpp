
#include "Entity.hpp"
#include "unit_conversions.hpp"

namespace physics
{
    Entity::Entity()
    {}

    Entity::Entity(b2World* world, const geometry::Point& position, const b2BodyType& bodyType, bool fixedRotation)
    {
        b2BodyDef bodyDef;
        bodyDef.type = bodyType;
        bodyDef.position = b2Vec2(toMeters(position.x), toMeters(position.y));

        m_body = world->CreateBody(&bodyDef);
        m_body->SetFixedRotation(fixedRotation);
    }

    b2Fixture* Entity::getFixture(const std::string& name) const
    {
        return m_fixtures.at(name);
    }

    // createFixtures overloaded functions

    b2Fixture* Entity::createFixture(const std::string& name, const geometry::Line& line, float density, float friction)
    {
        b2FixtureDef* fixtureDef = createBaseFixtureDef(density, friction);
        
        b2EdgeShape shape;
        shape.Set(b2Vec2(toMeters(line.p1.x), toMeters(line.p1.y)), b2Vec2(toMeters(line.p2.x), toMeters(line.p2.y)));
        fixtureDef->shape = &shape;

        m_fixtures[name] = m_body->CreateFixture(fixtureDef);
        return m_fixtures.at(name);
    }

    b2Fixture* Entity::createFixture(const std::string& name, const geometry::AABB& aabb, float density, float friction, const geometry::Point& position)
    {
        b2FixtureDef* fixtureDef = createBaseFixtureDef(density, friction);
        
        b2PolygonShape shape;
        shape.SetAsBox(toMeters(aabb.width / 2), toMeters(aabb.height / 2), b2Vec2(toMeters(position.x), toMeters(position.y)), 0);
        fixtureDef->shape = &shape;

        m_fixtures[name] = m_body->CreateFixture(fixtureDef);
        return m_fixtures.at(name);
    }

    b2Fixture* Entity::createFixture(const std::string& name, const geometry::Circle& circle, float density, float friction, const geometry::Point& position)
    {
        b2FixtureDef* fixtureDef = createBaseFixtureDef(density, friction);
        
        b2CircleShape shape;
        shape.m_p.Set(toMeters(position.x), toMeters(position.y));
        shape.m_radius = toMeters(circle.radius);
        fixtureDef->shape = &shape;

        m_fixtures[name] = m_body->CreateFixture(fixtureDef);
        return m_fixtures.at(name);
    }

    b2Fixture* Entity::createFixture(const std::string& name, const geometry::Polygon& polygon, float density, float friction)
    {
        b2FixtureDef* fixtureDef = createBaseFixtureDef(density, friction);
        
        b2PolygonShape shape;
        unsigned int ptnb = (unsigned int)polygon.points.size();
        b2Vec2 *points = new b2Vec2[ptnb];
        for(unsigned int i = 0 ; i < ptnb ; ++i)
            points[i].Set(toMeters(polygon.points[i].x), toMeters(polygon.points[i].y));
        shape.Set(points, ptnb);
        fixtureDef->shape = &shape;

        m_fixtures[name] = m_body->CreateFixture(fixtureDef);
        return m_fixtures.at(name);
    }

    // createBaseFixtureDef, used in createFixture overloaded functions to avoid code repetitions

    b2FixtureDef* Entity::createBaseFixtureDef(float density, float friction) const
    {
        b2FixtureDef *fixtureDef = new b2FixtureDef;
        fixtureDef->density = density;
        fixtureDef->friction = friction;

        return fixtureDef;
    }

    // Apply a force to the entity m_body's center

    void Entity::applyForceToCenter(const b2Vec2& force)
    {
        m_body->ApplyForce(force, m_body->GetWorldCenter());
    }

    // Apply a force to the entity's m_body at a point of the world

    void Entity::applyForceToPoint(const b2Vec2& force, const geometry::Point& point)
    {
        m_body->ApplyForce(force, b2Vec2(point.x, point.y));
    }
}
