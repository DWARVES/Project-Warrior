
#include "World.hpp"

namespace physics
{
    World::World(const b2Vec2& gravity)
    {
        m_world = new b2World(gravity);
    }

    Entity* World::getEntity(const std::string& name) const
    {
        return m_entities.at(name);
    }

    Entity* World::createEntity(const std::string& name, const geometry::Point& position, const b2BodyType& bodyType, bool fixedRotation)
    {
        m_entities[name] = new Entity(m_world, position, bodyType, fixedRotation);
        return m_entities.at(name);
    }
}
