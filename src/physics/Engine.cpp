
#include "Engine.hpp"

namespace physics
{
    Engine::Engine(const b2Vec2& gravity)
    {
        m_world = new b2World(gravity);
    }

    Entity* Engine::getEntity(const std::string& name) const
    {
        return m_entities.at(name);
    }

    Entity* Engine::createEntity(const std::string& name, const geometry::Point& position, const b2BodyType& bodyType, bool fixedRotation)
    {
        m_entities[name] = new Entity(m_world, position, bodyType, fixedRotation);
        return m_entities.at(name);
    }

    void Engine::applyForce(const std::string& entityName, const b2Vec2& force)
    {
        m_entities.at(entityName)->applyForce(force);
    }

    void Engine::applyForce(const std::string& entityName, const b2Vec2& force, const geometry::Point& point)
    {
        m_entities.at(entityName)->applyForce(force, point);
    }

    void Engine::applyLinearImpulse(const std::string& entityName, const b2Vec2& force)
    {
        m_entities.at(entityName)->applyLinearImpulse(force);
    }

    void Engine::applyLinearImpulse(const std::string& entityName, const b2Vec2& force, const geometry::Point& point)
    {
        m_entities.at(entityName)->applyLinearImpulse(force, point);
    }

    void Engine::applyTorque(const std::string& entityName, float force)
    {
        m_entities.at(entityName)->applyTorque(force);
    }

    void Engine::applyAngularImpulse(const std::string& entityName, float force)
    {
        m_entities.at(entityName)->applyAngularImpulse(force);
    }
}
