
#include "Engine.hpp"

namespace physics
{
    Engine::Engine(const b2Vec2& gravity)
    {
        m_world = new b2World(gravity);
    }

    Entity* Engine::getEntity(const std::string& name) const
    {
        return m_entities[name];
    }

    Entity* Engine::createEntity(const std::string& name, const geometry::Point& position, const b2BodyType& bodyType, bool fixedRotation)
    {
        m_entities[name] = new Entity(m_world, position, bodyType, fixedRotation);
        getEntity[name];
    }
}
