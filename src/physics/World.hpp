
#ifndef DEF_PHYSICS_WORLD
#define DEF_PHYSICS_WORLD

#include <map>
#include <memory>
#include "core/logger.hpp"
#include "Box2D/Box2D.h"
#include "geometry/point.hpp"
#include "Entity.hpp"
#include "Character.hpp"
#include "Attack.hpp"
#include "Platform.hpp"
#include "Obstacle.hpp"

namespace physics
{
    // A deleter for unique_ptr that avoid deletion doing nothing ; useful to prevent from conflicts between default deleters and Box2D protected destructors
    struct NullDeleter
    {
        void operator()(void* p) {}
    };

    class World : public b2DestructionListener, b2ContactListener
    {
        public:
            World(float x, float y); // Creates a new World with x and y gravity
            World(b2World* world); // Initializes m_world from world. Not so useful, except for testing purposes with Testbed
            ~World();

            Entity* getEntity(const std::string& name) const; // Returns a unique_ptr to the entity named "name" in the world entities map
            b2Joint* getJoint(const std::string& name) const; // Returns a unique_ptr to the joint named "name" in the world joints map

            float getXGravity() const; // Get world's x gravity
            float getYGravity() const; // Get world's y gravity

            void setXGravity(float x); // Set world's x gravity
            void setYGravity(float y); // Set world's y gravity
            void setGravity(float x, float y); // Set world's x and y gravity

            bool existsEntity(const std::string& name) const; // Check if the entity named "name" already exists
            bool existsJoint(const std::string& name) const; // Check if the joint named "name" already exists

            Entity* createEntity(const std::string& name, const geometry::Point& position, const b2BodyType& bodyType, uint16 type = Entity::Type::Default, uint16 collideWith = Entity::Type::All, float gravityScale = 1, bool fixedRotation = true); // Adds an entity to the world, returning a pointer the user can use to access it (can be done later with getEntity())
            // These functions do the same that createEntity with children classes
            Character* createCharacter(const std::string& name, const geometry::Point& position, const geometry::AABB& rect, float weight);
            Attack* createAttack(const std::string& name, const geometry::Point& position, const b2BodyType& bodyType, uint16 collideWith = Attack::CollideType::Normal, float gravityScale = 1, bool fixedRotation = true);
            Platform* createPlatform(const std::string& name, const geometry::Point& position);
            Platform* createPlatform(const std::string& name, const geometry::Point& position, const geometry::AABB& rect, float friction = 1);
            Obstacle* createObstacle(const std::string& name, const geometry::Point& position);
            Obstacle* createObstacle(const std::string& name, const geometry::Point& position, const geometry::AABB& rect, float friction = 1);

            // Functions used to create joints between entities
            b2RopeJoint* createRopeJoint(const std::string& name, Entity* entityA, Entity* entityB, float maxLength, const geometry::Point& localAnchorA = geometry::Point(0, 0), const geometry::Point& localAnchorB = geometry::Point(0, 0), bool collideConnected = true); // Rope joint, useful to create link CC

            // Remove an entity or a joint from the world
            void destroyEntity(const std::string& name);
            void destroyJoint(const std::string& name);
            
            void collisionCallback(Entity* entityA, Entity* entityB); // Calls the user implemented callback corresponding to the entities colliding's types

        protected:
            // Callbacks from b2DestructionListener called when a joint or fixture is destroyed
            virtual void SayGoodbye(b2Joint* joint);
            virtual void SayGoodbye(b2Fixture* fixture);
            
            // Callbacks from b2ContactListener for collision management
            void BeginContact(b2Contact* contact);
            void EndContact(b2Contact* contact);

            Entity* getEntityFromFixture(b2Fixture* fixture) const; // Returns a pointer to the Entity owning the fixture passed in parameter

        protected:
            b2World* m_world; // The world used in Box2D for simulation, containing all the bodies and fixtures (that we grouped in the Entity class)
            std::map<std::string, std::unique_ptr<Entity>> m_entities; // A map containing all the entities of the world, allowing to access them with a specific name given by the user when created (unique_ptr allows polymorphism behavior)
            std::map<std::string, std::unique_ptr<b2Joint, NullDeleter>> m_joints; // A map containing all the joints of the world, allowing to access them with a specific name given by the user when created (unique_ptr allows polymorphism behavior)
    };
}

#endif
