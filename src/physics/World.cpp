
#include "World.hpp"
#include <iostream>

namespace physics
{
    EntityDeleter::EntityDeleter(b2World* world)
        : m_world(world)
    {}
            
    EntityDeleter::EntityDeleter()
        : m_world(NULL)
    {}

    void EntityDeleter::operator()(Entity* e) const
    {
        if(m_world)
            m_world->DestroyBody(e->getBody());
        delete e;
    }

    World::World(float x, float y)
        : m_world(new b2World(b2Vec2(x,y))), m_entities(new EntityDeleter(m_world), true)
    {
        m_world->SetContactListener(this);
        m_world->SetDestructionListener(this);
    }

    World::World(b2World* world) : m_world(world)
    {
        m_world->SetContactListener(this);
        m_world->SetDestructionListener(this);
    }

    World::~World()
    {}

    Entity* World::getEntity(const std::string& name) const
    {
        if(!existsEntity(name)) {
            core::logger::logm("Tried to get unexisting entity \"" + name + "\" : returned nullptr.", core::logger::WARNING);
            return nullptr;
        }

        return m_entities.getEntityValue(name);
    }

    b2Joint* World::getJoint(const std::string& name) const
    {
        if(!existsJoint(name)) {
            core::logger::logm("Tried to get unexisting joint \"" + name + "\" : returned nullptr.", core::logger::WARNING);
            return nullptr;
        }

        return m_joints.at(name);
    }

    float World::getXGravity() const
    {
        return m_world->GetGravity().x;
    }

    float World::getYGravity() const
    {
        return m_world->GetGravity().y;
    }

    void World::setXGravity(float x)
    {
        m_world->SetGravity(b2Vec2(x, getYGravity()));
    }

    void World::setYGravity(float y)
    {
        m_world->SetGravity(b2Vec2(getXGravity(), y));
    }

    void World::setGravity(float x, float y)
    {
        m_world->SetGravity(b2Vec2(x, y));
    }

    bool World::existsEntity(const std::string& name) const
    {
        return m_entities.existsEntity(name);
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
            m_entities.createEntity(name, entity);
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
            m_entities.createEntity(name, character);
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
            m_entities.createEntity(name, attack);
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
            m_entities.createEntity(name, platform);
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
            m_entities.createEntity(name, platform);
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
            m_entities.createEntity(name, obstacle);
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
            m_entities.createEntity(name, obstacle);
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

            m_joints[name] = joint;
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
            m_entities.deleteEntity(name);
            core::logger::logm("The entity \"" + name + "\" has been destroyed.", core::logger::MSG);
        }
        else
            core::logger::logm("Tried to destroy unexisting entity \"" + name + "\" : cancelled operation.", core::logger::WARNING);
    }

    void World::destroyJoint(const std::string& name)
    {
        if(existsJoint(name)) {
            m_world->DestroyJoint(m_joints.at(name));
            core::logger::logm("The joint \"" + name + "\" has been destroyed.", core::logger::MSG);
            m_joints.erase(name);
        }
        else
            core::logger::logm("Tried to destroy unexisting joint \"" + name + "\" : cancelled operation.", core::logger::WARNING);
    }

    void World::collisionCallback(Entity* entityA, Entity* entityB) {}

    void World::SayGoodbye(b2Joint* joint)
    {
        std::map<std::string, b2Joint*>::iterator it;
        for(it = m_joints.begin() ; it != m_joints.end() ; ++it) {
            if(it->second == joint) {
                core::logger::logm("The joint \"" + it->first + "\" has been destroyed.", core::logger::MSG);
                m_joints.erase(it->first);
                break;
            }
        }
    }

    void World::SayGoodbye(b2Fixture* fixture) {}


    void World::BeginContact(b2Contact* contact)
    {
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        Entity* entityA = getEntityFromFixture(fixtureA);
        Entity* entityB = getEntityFromFixture(fixtureB);

        if(entityA == nullptr || entityB == nullptr)
        {
            core::logger::logm("A colliding fixture doesn't belong to an existing entity : stopped management of the collision.", core::logger::ERROR);
            return;
        }

        //collisionCallback(entityA, entityB);

        // Platform collision management

        // Check if one of the fixtures is the platform
        b2Body* platformBody = nullptr;
        b2Body* otherBody = nullptr;
        if(entityA->getType() == Entity::Type::Platform) {
            platformBody = fixtureA->GetBody();
            otherBody = fixtureB->GetBody();
        }
        if(entityB->getType() == Entity::Type::Platform) {
            platformBody = fixtureB->GetBody();
            otherBody = fixtureA->GetBody();
        }
        if(!platformBody)
            return;

        int numPoints = contact->GetManifold()->pointCount;
        b2WorldManifold worldManifold;
        contact->GetWorldManifold(&worldManifold);

        // Check if contact points are moving downward
        for(int i = 0; i < numPoints; i++) {
            b2Vec2 pointVel = otherBody->GetLinearVelocityFromWorldPoint(worldManifold.points[i]);
            if(pointVel.y < 0)
                return; // Point is moving down, leave contact solid and exit
        }

        // No points are moving downward, contact should not be solid
        contact->SetEnabled(false);
    }

    void World::EndContact(b2Contact* contact)
    {
        // Platform collision management

        // Reset the default state of the contact in case it comes back for more
        contact->SetEnabled(true);
    }

    Entity* World::getEntityFromFixture(b2Fixture* fixture) const
    {
        void* bodyUserData = fixture->GetBody()->GetUserData();
        if(bodyUserData) {
            return static_cast<Entity*>(bodyUserData);
        }
        else
            return nullptr;
    }

    void World::start()
    {
        m_ltime = SDL_GetTicks();
    }

    void World::step()
    {
        Uint32 time = SDL_GetTicks();
        m_world->Step(float(time - m_ltime) / 1000.0f, 10, 8);
        m_ltime = time;
    }

    bool World::createNamespace(const std::string& path)
    {
        return m_entities.createNamespace(path);
    }

    void World::deleteNamespace(const std::string& path)
    {
        m_entities.deleteNamespace(path);
    }

    bool World::existsNamespace(const std::string& path) const
    {
        return m_entities.existsNamespace(path);
    }

    void World::enterNamespace(const std::string& path)
    {
        m_entities.enterNamespace(path);
    }

    std::string World::actualNamespace() const
    {
        return m_entities.actualNamespace();
    }

}
