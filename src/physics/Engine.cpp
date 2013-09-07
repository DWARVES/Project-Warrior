
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

    void Engine::applyForceToCenter(const std::string& entityName, const b2Vec2& force)
    {
        m_entities.at(entityName)->applyForceToCenter(force);
    }

    void Engine::applyForceToPoint(const std::string& entityName, const b2Vec2& force, const geometry::Point& point)
    {
        m_entities.at(entityName)->applyForceToPoint(force, point);
    }
}
