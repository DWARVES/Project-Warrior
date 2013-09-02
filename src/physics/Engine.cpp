
#include "Engine.hpp"

namespace Physics
{
    Engine::Engine(const b2Vec2& gravity)
    {
        m_world = new b2World(gravity);
    }

    Entity* Engine::createEntity(const std::string& name, const Geometry::Point& position, const b2BodyType& bodyType, bool fixedRotation)
    {
        m_entities[name] = new Entity(m_world, position, bodyType, fixedRotation);
        return m_entities[name];
    }
}
