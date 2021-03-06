
#include "World.hpp"
#include "graphics/graphics.hpp"
#include <iostream>

namespace physics
{
    World::World()
        : m_world(NULL), m_dd(false), m_ddraw(NULL)
    {
        m_world = new b2World(b2Vec2(0.0f,-10.0f));
        m_world->SetContactListener(this);
        m_world->SetDestructionListener(this);
    }

    World::World(float x, float y)
        : m_world(NULL), m_dd(false), m_ddraw(NULL)
    {
        m_world = new b2World(b2Vec2(x,y));
        m_world->SetContactListener(this);
        m_world->SetDestructionListener(this);
    }

    World::World(b2World* world) : World()
    {
        m_world = world;
        m_world->SetContactListener(this);
        m_world->SetDestructionListener(this);
    }

    World::~World()
    {
        if(m_ddraw != NULL)
            delete m_ddraw;
    }

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

        return m_joints.getEntityValue(name);
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
        return m_joints.existsEntity(name);
    }

    Entity* World::createEntity(const std::string& name, const geometry::Point& position, const b2BodyType& bodyType, uint16 type, uint16 collideWith, float gravityScale, bool fixedRotation)
    {
        if(!existsEntity(name)) {
            Entity *entity = new Entity(name, this, position, bodyType, type, collideWith, gravityScale, fixedRotation);
            m_entities.createEntity(name, entity);
            core::logger::logm("The entity \"" + name + "\" has been created.", core::logger::DEBUG);
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
            Character *character = new Character(name, this, position, rect, weight);
            m_entities.createEntity(name, character);
            core::logger::logm("The character \"" + name + "\" has been created.", core::logger::DEBUG);
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
            Attack *attack = new Attack(name, this, position, bodyType, collideWith, gravityScale, fixedRotation);
            m_entities.createEntity(name, attack);
            core::logger::logm("The attack \"" + name + "\" has been created.", core::logger::DEBUG);
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
            Platform *platform = new Platform(name, this, position);
            m_entities.createEntity(name, platform);
            core::logger::logm("The platform \"" + name + "\" has been created.", core::logger::DEBUG);
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
            Platform *platform = new Platform(name, this, position, rect, friction);
            m_entities.createEntity(name, platform);
            core::logger::logm("The platform \"" + name + "\" has been created.", core::logger::DEBUG);
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
            Obstacle *obstacle = new Obstacle(name, this, position);
            m_entities.createEntity(name, obstacle);
            core::logger::logm("The obstacle \"" + name + "\" has been created.", core::logger::DEBUG);
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
            Obstacle *obstacle = new Obstacle(name, this, position, rect, friction);
            m_entities.createEntity(name, obstacle);
            core::logger::logm("The obstacle \"" + name + "\" has been created.", core::logger::DEBUG);
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

            m_joints.createEntity(name, joint);
            core::logger::logm("The rope joint \"" + name + "\" has been created.", core::logger::DEBUG);
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
            m_world->DestroyBody(m_entities.getEntityValue(name)->getBody());
            m_entities.deleteEntity(name);
            core::logger::logm("The entity \"" + name + "\" has been destroyed.", core::logger::DEBUG);
        }
        else
            core::logger::logm("Tried to destroy unexisting entity \"" + name + "\" : cancelled operation.", core::logger::WARNING);
    }

    void World::destroyJoint(const std::string& name)
    {
        if(existsJoint(name)) {
            m_world->DestroyJoint(m_joints.getEntityValue(name));
            m_joints.deleteEntity(name);
            core::logger::logm("The joint \"" + name + "\" has been destroyed.", core::logger::DEBUG);
        }
        else
            core::logger::logm("Tried to destroy unexisting joint \"" + name + "\" : cancelled operation.", core::logger::WARNING);
    }

    void World::setCallback(std::string nameA, std::string nameB, Callback callback, void* data)
    {
        if(!m_entities.existsEntity(nameA) || !m_entities.existsEntity(nameB))
            return;

        Entity* entA = m_entities.getEntityValue(nameA);
        Entity* entB = m_entities.getEntityValue(nameB);
        m_callbacks[entA][entB].first  = callback;
        m_callbacks[entA][entB].second = data;
        m_callbacks[entB][entA].first  = callback;
        m_callbacks[entB][entA].second = data;
    }

    void World::removeCallback(std::string nameA, std::string nameB)
    {
        if(!m_entities.existsEntity(nameA) || !m_entities.existsEntity(nameB))
            return;
        Entity* entA = m_entities.getEntityValue(nameA);
        Entity* entB = m_entities.getEntityValue(nameB);

        if(m_callbacks.find(entA) != m_callbacks.end()
                && m_callbacks[entA].find(entB) != m_callbacks[entA].end()) {
            m_callbacks[entA].erase(entB);
            if(m_callbacks[entA].begin() == m_callbacks[entA].end())
                m_callbacks.erase(entA);
        }

        if(m_callbacks.find(entB) != m_callbacks.end()
                && m_callbacks[entB].find(entA) != m_callbacks[entB].end()) {
            m_callbacks[entB].erase(entA);
            if(m_callbacks[entB].begin() == m_callbacks[entB].end())
                m_callbacks.erase(entB);
        }
    }

    void World::setCallback(std::string name, Callback callback, void* data)
    {
        if(!m_entities.existsEntity(name))
            return;
        Entity* ent = m_entities.getEntityValue(name);
        m_glcallbacks[ent].first  = callback;
        m_glcallbacks[ent].second = data;
    }

    void World::removeCallback(std::string name)
    {
        if(!m_entities.existsEntity(name))
            return;
        Entity* ent = m_entities.getEntityValue(name);
        if(m_glcallbacks.find(ent) != m_glcallbacks.end())
            m_glcallbacks.erase(ent);
    }

    void World::setCallback(Entity* ent, b2Fixture* fixt, Callback callback, void* data, bool sensors)
    {
        m_ftcallbacks[ent][fixt].cb      = callback;
        m_ftcallbacks[ent][fixt].data    = data;
        m_ftcallbacks[ent][fixt].sensors = sensors;;
    }

    void World::removeCallback(Entity* ent, b2Fixture* fixt)
    {
        if(m_ftcallbacks.find(ent) != m_ftcallbacks.end()
                && m_ftcallbacks[ent].find(fixt) != m_ftcallbacks[ent].end()) {
            m_ftcallbacks[ent].erase(fixt);
            if(m_ftcallbacks[ent].begin() == m_ftcallbacks[ent].end())
                m_ftcallbacks.erase(ent);
        }
    }

    void World::collisionCallback(Entity* entityA, b2Fixture* fA, Entity* entityB, b2Fixture* fB, bool st)
    {
        if(m_callbacks.find(entityA) != m_callbacks.end()
                && m_callbacks[entityA].find(entityB) != m_callbacks[entityA].end())
            (*m_callbacks[entityA][entityB].first)(entityA, entityB, st, m_callbacks[entityA][entityB].second);

        if(m_glcallbacks.find(entityA) != m_glcallbacks.end())
            (*m_glcallbacks[entityA].first)(entityA, entityB, st, m_glcallbacks[entityA].second);
        if(m_glcallbacks.find(entityB) != m_glcallbacks.end())
            (*m_glcallbacks[entityB].first)(entityB, entityA, st, m_glcallbacks[entityB].second);

        if(m_ftcallbacks.find(entityA) != m_ftcallbacks.end()
                && m_ftcallbacks[entityA].find(fA) != m_ftcallbacks[entityA].end()
                && (m_ftcallbacks[entityA][fA].sensors || !fB->IsSensor()))
            (*m_ftcallbacks[entityA][fA].cb)(entityA, entityB, st, m_ftcallbacks[entityA][fA].data);
        if(m_ftcallbacks.find(entityB) != m_ftcallbacks.end()
                && m_ftcallbacks[entityB].find(fB) != m_ftcallbacks[entityB].end()
                && (m_ftcallbacks[entityB][fB].sensors || !fA->IsSensor()))
            (*m_ftcallbacks[entityB][fB].cb)(entityB, entityA, st, m_ftcallbacks[entityB][fB].data);
    }

    void World::SayGoodbye(b2Joint* joint)
    {
        std::vector<std::string> jointsNamesList = m_joints.listEntities();
        std::vector<std::string>::iterator it;
        for(it = jointsNamesList.begin() ; it != jointsNamesList.end() ; ++it) {
            if(m_joints.getEntityValue(*it) == joint) {
                m_joints.deleteEntity(*it);
                core::logger::logm("The joint \"" + *it + "\" has been destroyed.", core::logger::DEBUG);
                break;
            }
        }
    }

    void World::SayGoodbye(b2Fixture* fixture)
    {
        Entity* entity = getEntityFromFixture(fixture);
        std::vector<std::string> fixturesNamesList = entity->m_fixtures.listEntities();
        std::vector<std::string>::iterator it;
        for(it = fixturesNamesList.begin() ; it != fixturesNamesList.end() ; ++it) {
            if(entity->m_fixtures.getEntityValue(*it) == fixture) {
                entity->m_fixtures.deleteEntity(*it);
                core::logger::logm("The fixture \"" + *it + "\" in entity \"" + entity->m_name + "\" has been destroyed.", core::logger::DEBUG);
                break;
            }
        }
    }

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

        collisionCallback(entityA, fixtureA, entityB, fixtureB, true);

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
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();
        Entity* entityA = getEntityFromFixture(fixtureA);
        Entity* entityB = getEntityFromFixture(fixtureB);
        if(entityA == nullptr || entityB == nullptr)
            return; /* No need to log error : it has already been done when the contact started. */
        collisionCallback(entityA, fixtureA, entityB, fixtureB, false);

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

    b2World* World::getWorld() const
    {
        return m_world;
    }

    void World::enableDebugDraw(bool en)
    {
        if(en == m_dd)
            return;
        if(!en) {
            if(m_ddraw) {
                delete m_ddraw;
                m_ddraw = NULL;
            }
            m_world->SetDebugDraw(NULL);
            m_dd = false;
        }
        else {
            if(!m_ddraw)
                m_ddraw = new DebugDraw;
            m_world->SetDebugDraw(m_ddraw);
            m_ddraw->SetFlags(b2Draw::e_shapeBit);
            m_dd = true;
        }
    }

    bool World::debugDraw() const
    {
        return m_dd;
    }

    void World::debugDraw(graphics::Graphics* gfx)
    {
        if(!m_dd)
            return;
        m_ddraw->set(gfx);
        m_world->DrawDebugData();
    }

}
