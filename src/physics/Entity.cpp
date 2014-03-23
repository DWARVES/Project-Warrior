
#include "Entity.hpp"
#include "unit_conversions.hpp"
#include "core/logger.hpp"

namespace physics
{
    Entity::Entity()
    {}

    Entity::Entity(const std::string& name, b2World* world, const geometry::Point& position, const b2BodyType& bodyType, uint16 type, uint16 collideWith, float gravityScale, bool fixedRotation) : m_name(name), m_type(type), m_collideWith(collideWith)
    {
        b2BodyDef bodyDef;
        bodyDef.type = bodyType;
        bodyDef.position = b2Vec2(toMeters(position.x), toMeters(position.y));
        bodyDef.gravityScale = gravityScale;

        m_body = world->CreateBody(&bodyDef);
        m_body->SetFixedRotation(fixedRotation);
        m_body->SetUserData(this);
    }

    b2Body* Entity::getBody() const
    {
        return m_body;
    }

    b2Fixture* Entity::getFixture(const std::string& name) const
    {
        if(!m_fixtures.existsEntity(name))
        {
            core::logger::logm("Tried to get unexisting fixture \"" + name + "\" from entity \"" + m_name + "\" : returned nullptr.", core::logger::WARNING);
            return nullptr;
        }

        return m_fixtures.getEntityValue(name);
    }

    std::string Entity::getName() const
    {
        return m_name;
    }
    
    uint16 Entity::getType() const
    {
        return m_type;
    }

    geometry::Point Entity::getPosition() const
    {
        return geometry::Point(m_body->GetPosition().x, m_body->GetPosition().y);
    }

    float Entity::getXLinearVelocity()
    {
        return m_body->GetLinearVelocity().x;
    }

    float Entity::getYLinearVelocity()
    {
        return m_body->GetLinearVelocity().y;
    }

    float Entity::getAngularVelocity()
    {
        return m_body->GetAngularVelocity();
    }

    void Entity::setXLinearVelocity(float velX)
    {
        m_body->SetLinearVelocity(b2Vec2(velX, m_body->GetLinearVelocity().y));
    }

    void Entity::setYLinearVelocity(float velY)
    {
        m_body->SetLinearVelocity(b2Vec2(m_body->GetLinearVelocity().x, velY));
    }

    void Entity::setLinearVelocity(float velX, float velY)
    {
        m_body->SetLinearVelocity(b2Vec2(velX, velY));
    }

    void Entity::setAngularVelocity(float omega)
    {
        m_body->SetAngularVelocity(omega);
    }

    void Entity::setFixedRotation(bool fixedRotation)
    {
        m_body->SetFixedRotation(fixedRotation);
    }

    b2Fixture* Entity::createFixture(const std::string& name, const geometry::Line& line, float density, float friction, uint16 type, uint16 collideWith, bool sensor)
    {
        b2FixtureDef* fixtureDef = createBaseFixtureDef(name, density, friction, type, collideWith, sensor);
        if(fixtureDef == nullptr)
            return nullptr;
        
        b2EdgeShape shape;
        shape.Set(b2Vec2(toMeters(line.p1.x), toMeters(line.p1.y)), b2Vec2(toMeters(line.p2.x), toMeters(line.p2.y)));
        fixtureDef->shape = &shape;

        m_fixtures.createEntity(name, m_body->CreateFixture(fixtureDef));

        return m_fixtures.getEntityValue(name);
    }

    b2Fixture* Entity::createFixture(const std::string& name, const geometry::AABB& aabb, float density, float friction, uint16 type, uint16 collideWith, const geometry::Point& position, bool sensor)
    {
        b2FixtureDef* fixtureDef = createBaseFixtureDef(name, density, friction, type, collideWith, sensor);
        if(fixtureDef == nullptr)
            return nullptr;
        
        b2PolygonShape shape;
        shape.SetAsBox(toMeters(aabb.width / 2), toMeters(aabb.height / 2), b2Vec2(toMeters(position.x), toMeters(position.y)), 0);
        fixtureDef->shape = &shape;

        m_fixtures.createEntity(name, m_body->CreateFixture(fixtureDef));

        return m_fixtures.getEntityValue(name);
    }

    b2Fixture* Entity::createFixture(const std::string& name, const geometry::Circle& circle, float density, float friction, uint16 type, uint16 collideWith, const geometry::Point& position, bool sensor)
    {
        b2FixtureDef* fixtureDef = createBaseFixtureDef(name, density, friction, type, collideWith, sensor);
        if(fixtureDef == nullptr)
            return nullptr;
        
        b2CircleShape shape;
        shape.m_p.Set(toMeters(position.x), toMeters(position.y));
        shape.m_radius = toMeters(circle.radius);
        fixtureDef->shape = &shape;

        m_fixtures.createEntity(name, m_body->CreateFixture(fixtureDef));

        return m_fixtures.getEntityValue(name);
    }

    b2Fixture* Entity::createFixture(const std::string& name, const geometry::Polygon& polygon, float density, float friction, uint16 type, uint16 collideWith, bool sensor)
    {
        b2FixtureDef* fixtureDef = createBaseFixtureDef(name, density, friction, type, collideWith, sensor);
        if(fixtureDef == nullptr)
            return nullptr;
        
        b2PolygonShape shape;
        unsigned int ptnb = (unsigned int)polygon.points.size();
        b2Vec2 *points = new b2Vec2[ptnb];
        for(unsigned int i = 0 ; i < ptnb ; ++i)
            points[i].Set(toMeters(polygon.points[i].x), toMeters(polygon.points[i].y));
        shape.Set(points, ptnb);
        fixtureDef->shape = &shape;

        m_fixtures.createEntity(name, m_body->CreateFixture(fixtureDef));

        return m_fixtures.getEntityValue(name);
    }

    b2FixtureDef* Entity::createBaseFixtureDef(const std::string& name, float density, float friction, uint16 type, uint16 collideWith, bool sensor) const
    {
        if(m_fixtures.existsEntity(name))
        {
            core::logger::logm("Tried to override existing fixture \"" + name + "\" in entity \"" + m_name + "\" : cancelled operation and returned nullptr.", core::logger::WARNING);
            return nullptr;
        }

        b2FixtureDef *fixtureDef = new b2FixtureDef;

        fixtureDef->density = density;
        fixtureDef->friction = friction;
        fixtureDef->isSensor = sensor;

        if(type != Type::ThisType)
            fixtureDef->filter.categoryBits = type;
        else
            fixtureDef->filter.categoryBits = m_type;
        if(collideWith != Type::ThisCollideWith)
            fixtureDef->filter.maskBits = collideWith;
        else
            fixtureDef->filter.maskBits = m_collideWith;

        return fixtureDef;
    }

    void Entity::destroyFixture(const std::string& name)
    {
        if(m_fixtures.existsEntity(name)) {
            m_body->DestroyFixture(m_fixtures.getEntityValue(name));
            m_fixtures.deleteEntity(name);
            core::logger::logm("The fixture \"" + name + "\" has been destroyed in entity \"" + m_name + "\".", core::logger::MSG);
        }
        else
            core::logger::logm("Tried to destroy unexisting fixture \"" + name + "\" in entity \"" + m_name + "\" : cancelled operation.", core::logger::WARNING);
    }

    void Entity::applyForce(float forceX, float forceY)
    {
        m_body->ApplyForce(b2Vec2(forceX, forceY), m_body->GetWorldCenter());
    }

    void Entity::applyForce(float forceX, float forceY, const geometry::Point& point)
    {
        m_body->ApplyForce(b2Vec2(forceX, forceY), b2Vec2(point.x, point.y));
    }

    void Entity::applyLinearImpulse(float forceX, float forceY)
    {
        m_body->ApplyLinearImpulse(b2Vec2(forceX, forceY), m_body->GetWorldCenter());
    }

    void Entity::applyLinearImpulse(float forceX, float forceY, const geometry::Point& point)
    {
        m_body->ApplyLinearImpulse(b2Vec2(forceX, forceY), b2Vec2(point.x, point.y));
    }

    void Entity::applyTorque(float force)
    {
        m_body->ApplyTorque(force);
    }

    void Entity::applyAngularImpulse(float force)
    {
        m_body->ApplyAngularImpulse(force);
    }
}
