
#include "World.hpp"
#include "core/logger.hpp"

namespace physics
{
    World::World(const b2Vec2& gravity)
    {
        m_world = new b2World(gravity);
    }

    Entity* World::getEntity(const std::string& name) const
    {
        if(!m_entities.count(name))
        {
            core::logger::logm("Tried to get unexisting entity \"" + name + "\" : returned NULL.", core::logger::WARNING);
            return NULL;
        }

        return m_entities.at(name);
    }

    Entity* World::createEntity(const std::string& name, const geometry::Point& position, const b2BodyType& bodyType, bool fixedRotation)
    {
        if(m_entities.count(name))
        {
            core::logger::logm("Tried to override the existing entity \"" + name + "\" : cancelled operation and returned NULL.", core::logger::WARNING);
            return NULL;
        }

        m_entities[name] = new Entity(name, m_world, position, bodyType, fixedRotation);
        core::logger::logm("The entity \"" + name + "\" has been created.", core::logger::MSG);

        return m_entities.at(name);
    }
}
