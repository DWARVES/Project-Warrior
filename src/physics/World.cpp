
#include "World.hpp"
#include <iostream>

namespace physics
{
    World::World(const b2Vec2& gravity)
    {
        m_world = new b2World(gravity);
        m_world->SetContactListener(new CollisionManager());
        m_world->SetDestructionListener(this);

        core::logger::init();
        core::logger::addOutput(&std::cout);
    }

    World::World(b2World* world) : m_world(world)
    {
        m_world->SetContactListener(new CollisionManager());
        m_world->SetDestructionListener(this);

        core::logger::init();
        core::logger::addOutput(&std::cout);
    }

    World::~World()
    {
        core::logger::free();
    }

    Entity* World::getEntity(const std::string& name) const
    {
        if(!existsEntity(name)) {
            core::logger::logm("Tried to get unexisting entity \"" + name + "\" : returned nullptr.", core::logger::WARNING);
            return nullptr;
        }

        return m_entities.at(name).get();
    }

    b2Joint* World::getJoint(const std::string& name) const
    {
        if(!existsJoint(name)) {
            core::logger::logm("Tried to get unexisting joint \"" + name + "\" : returned nullptr.", core::logger::WARNING);
            return nullptr;
        }

        return m_joints.at(name).get();
    }

    b2Vec2 World::getGravity() const
    {
        return m_world->GetGravity();
    }

    void World::setGravity(float x, float y)
    {
        m_world->SetGravity(b2Vec2(x, y));
    }

    bool World::existsEntity(const std::string& name) const
    {
        if(m_entities.count(name))
            return true;
        else
            return false;
    }

    bool World::existsJoint(const std::string& name) const
    {
        if(m_joints.count(name))
            return true;
        else
            return false;
    }

    Entity* World::createEntity(const std::string& name, const geometry::Point& position, const b2BodyType& bodyType, uint16 type, uint16 collideWith, float gravityScale, bool fixedRotation)
    {
        if(!existsEntity(name)) {
            Entity *entity = new Entity(name, m_world, position, bodyType, type, collideWith, gravityScale, fixedRotation);
            m_entities[name] = std::unique_ptr<Entity>(entity);
            core::logger::logm("The entity \"" + name + "\" has been created.", core::logger::MSG);
            return entity;
        }
        else {
            core::logger::logm("Tried to override the existing entity \"" + name + "\" : cancelled operation and returned nullptr.", core::logger::WARNING);
            return nullptr;
        }
    }

    Character* World::createCharacter(const std::string& name, const geometry::Point& position, const geometry::AABB& rect, float weight)
    {
        if(!existsEntity(name)) {
            Character *character = new Character(name, m_world, position, rect, weight);
            m_entities[name] = std::unique_ptr<Character>(character);
            core::logger::logm("The character \"" + name + "\" has been created.", core::logger::MSG);
            return character;
        }
        else {
            core::logger::logm("Tried to override the existing character \"" + name + "\" : cancelled operation and returned nullptr.", core::logger::WARNING);
            return nullptr;
        }
    }

    Attack* World::createAttack(const std::string& name, const geometry::Point& position, const b2BodyType& bodyType, uint16 collideWith, float gravityScale, bool fixedRotation)
    {
        if(!existsEntity(name)) {
            Attack *attack = new Attack(name, m_world, position, bodyType, collideWith, gravityScale, fixedRotation);
            m_entities[name] = std::unique_ptr<Attack>(attack);
            core::logger::logm("The attack \"" + name + "\" has been created.", core::logger::MSG);
            return attack;
        }
        else {
            core::logger::logm("Tried to override the existing attack \"" + name + "\" : cancelled operation and returned nullptr.", core::logger::WARNING);
            return nullptr;
        }
    }

    Platform* World::createPlatform(const std::string& name, const geometry::Point& position)
    {
        if(!existsEntity(name)) {
            Platform *platform = new Platform(name, m_world, position);
            m_entities[name] = std::unique_ptr<Platform>(platform);
            core::logger::logm("The platform \"" + name + "\" has been created.", core::logger::MSG);
            return platform;
        }
        else {
            core::logger::logm("Tried to override the existing platform \"" + name + "\" : cancelled operation and returned nullptr.", core::logger::WARNING);
            return nullptr;
        }
    }
    Platform* World::createPlatform(const std::string& name, const geometry::Point& position, const geometry::AABB& rect, float friction)
    {
        if(!existsEntity(name)) {
            Platform *platform = new Platform(name, m_world, position, rect, friction);
            m_entities[name] = std::unique_ptr<Platform>(platform);
            core::logger::logm("The platform \"" + name + "\" has been created.", core::logger::MSG);
            return platform;
        }       
        else {
            core::logger::logm("Tried to override the existing platform \"" + name + "\" : cancelled operation and returned nullptr.", core::logger::WARNING);
            return nullptr;
        }
    }

    Obstacle* World::createObstacle(const std::string& name, const geometry::Point& position)
    {
        if(!existsEntity(name)) {
            Obstacle *obstacle = new Obstacle(name, m_world, position);
            m_entities[name] = std::unique_ptr<Obstacle>(obstacle);
            core::logger::logm("The obstacle \"" + name + "\" has been created.", core::logger::MSG);
            return obstacle;
        }
        else {
            core::logger::logm("Tried to override the existing obstacle \"" + name + "\" : cancelled operation and returned nullptr.", core::logger::WARNING);
            return nullptr;
        }
    }
    Obstacle* World::createObstacle(const std::string& name, const geometry::Point& position, const geometry::AABB& rect, float friction)
    {
        if(!existsEntity(name)) {
            Obstacle *obstacle = new Obstacle(name, m_world, position, rect, friction);
            m_entities[name] = std::unique_ptr<Obstacle>(obstacle);
            core::logger::logm("The obstacle \"" + name + "\" has been created.", core::logger::MSG);
            return obstacle;
        }
        else {
            core::logger::logm("Tried to override the existing obstacle \"" + name + "\" : cancelled operation and returned nullptr.", core::logger::WARNING);
            return nullptr;
        }
    }

    b2RopeJoint* World::createRopeJoint(const std::string& name, Entity* entityA, Entity* entityB, float maxLength, const geometry::Point& localAnchorA, const geometry::Point& localAnchorB, bool collideConnected)
    {
        if(!existsJoint(name)) {
            b2RopeJointDef jointDef;
            jointDef.bodyA = entityA->getBody();
            jointDef.bodyB = entityB->getBody();
            jointDef.collideConnected = collideConnected;
            jointDef.localAnchorA.Set(localAnchorA.x, localAnchorA.y);
            jointDef.localAnchorB.Set(localAnchorB.x, localAnchorB.y);
            jointDef.maxLength = maxLength;
            b2RopeJoint* joint = (b2RopeJoint*)m_world->CreateJoint(&jointDef);

            m_joints[name] = std::unique_ptr<b2RopeJoint, NullDeleter>(joint, NullDeleter());
            core::logger::logm("The rope joint \"" + name + "\" has been created.", core::logger::MSG);
            return joint;
        }
        else {
            core::logger::logm("Tried to override the existing rope joint \"" + name + "\" : cancelled operation and returned nullptr.", core::logger::WARNING);
            return nullptr;
        }
    }

    void World::destroyEntity(const std::string& name)
    {
        if(existsEntity(name)) {
            m_world->DestroyBody(m_entities.at(name).get()->getBody());
            m_entities.erase(name);
            core::logger::logm("The entity \"" + name + "\" has been destroyed.", core::logger::MSG);
        }
        else
            core::logger::logm("Tried to destroy unexisting entity \"" + name + "\" : cancelled operation.", core::logger::WARNING);
    }

    void World::destroyJoint(const std::string& name)
    {
        if(existsJoint(name)) {
            m_world->DestroyJoint(m_joints.at(name).get());
            core::logger::logm("The joint \"" + name + "\" has been destroyed.", core::logger::MSG);
            m_joints.erase(name);
        }
        else
            core::logger::logm("Tried to destroy unexisting joint \"" + name + "\" : cancelled operation.", core::logger::WARNING);
    }

    void World::SayGoodbye(b2Joint* joint)
    {
        std::map<std::string, std::unique_ptr<b2Joint, NullDeleter>>::iterator it;
        for(it = m_joints.begin() ; it != m_joints.end() ; ++it) {
            if(it->second.get() == joint) {
                core::logger::logm("The joint \"" + it->first + "\" has been destroyed.", core::logger::MSG);
                m_joints.erase(it->first);
                break;
            }
        }
    }

    void World::SayGoodbye(b2Fixture* fixture) {}
}
