
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
    /** @brief A deleter for unique_ptr that avoid deletion doing nothing ; useful to prevent from conflicts between default deleters and Box2D protected destructors */
    struct NullDeleter
    {
        void operator()(void* p) {}
    };

    /** @brief The main class, instanciated by the user and used to manage physical entities and relations between them */
    class World : public b2DestructionListener, b2ContactListener
    {
        public:
            /** @brief Creates a new World with x and y gravity */
            World(float x, float y);
            /** @brief Initializes m_world from world. Not so useful, except for testing purposes with Testbed */
            World(b2World* world); 
            ~World();

            /** @brief Returns a unique_ptr to the entity named "name" in the world entities map */
            Entity* getEntity(const std::string& name) const; 
            /** @brief Returns a unique_ptr to the joint named "name" in the world joints map */
            b2Joint* getJoint(const std::string& name) const; 

            /** @brief Get world's x gravity */
            float getXGravity() const; 
            /** @brief Get world's y gravity */
            float getYGravity() const; 

            /** @brief Set world's x gravity */
            void setXGravity(float x); 
            /** @brief Set world's y gravity */
            void setYGravity(float y); 
            /** @brief Set world's x and y gravity */
            void setGravity(float x, float y); 

            /** @brief Check if the Entity named "name" already exists */
            bool existsEntity(const std::string& name) const; 
            /** @brief Check if the Joint named "name" already exists */
            bool existsJoint(const std::string& name) const; 

            /** @brief Adds an Entity to the world, returning a pointer the user can use to access it (can be done later with getEntity()) */
            Entity* createEntity(const std::string& name, const geometry::Point& position, const b2BodyType& bodyType, uint16 type = Entity::Type::Default, uint16 collideWith = Entity::Type::All, float gravityScale = 1, bool fixedRotation = true);
            /** @brief Adds a Character to the world */
            Character* createCharacter(const std::string& name, const geometry::Point& position, const geometry::AABB& rect, float weight);
            /** @brief Adds an Attack to the world */
            Attack* createAttack(const std::string& name, const geometry::Point& position, const b2BodyType& bodyType, uint16 collideWith = Attack::CollideType::Normal, float gravityScale = 1, bool fixedRotation = true);
            /** @brief Adds a Platform to the world with default parameters */
            Platform* createPlatform(const std::string& name, const geometry::Point& position);
            /** @brief Adds a Platform to the world with specified rect and friction */
            Platform* createPlatform(const std::string& name, const geometry::Point& position, const geometry::AABB& rect, float friction = 1);
            /** @brief Adds an Obstacle to the world with default parameters */
            Obstacle* createObstacle(const std::string& name, const geometry::Point& position);
            /** @brief Adds an Obstacle to the world with specified rect and friction */
            Obstacle* createObstacle(const std::string& name, const geometry::Point& position, const geometry::AABB& rect, float friction = 1);

            /** @brief Adds a RopeJoint to the world, useful to create link CC */
            b2RopeJoint* createRopeJoint(const std::string& name, Entity* entityA, Entity* entityB, float maxLength, const geometry::Point& localAnchorA = geometry::Point(0, 0), const geometry::Point& localAnchorB = geometry::Point(0, 0), bool collideConnected = true); 

            /** @brief Removes an Entity from the world */
            void destroyEntity(const std::string& name); 
            /** @brief Removes a Joint from the world */
            void destroyJoint(const std::string& name); 

            /** @brief Calls the user implemented callback corresponding to the Entities colliding's types */
            void collisionCallback(Entity* entityA, Entity* entityB); 

        protected:
            /** @brief Callback from b2DestructionListener called when a joint is destroyed */
            virtual void SayGoodbye(b2Joint* joint);
            /** @brief Callback from b2DestructionListener called when a fixture is destroyed */
            virtual void SayGoodbye(b2Fixture* fixture);

            /** @brief Callback from b2ContactListener called when two entities start to collide */
            void BeginContact(b2Contact* contact);
            /** @brief Callback from b2ContactListener called when two entities stop colliding */
            void EndContact(b2Contact* contact);

            /** @brief Returns a pointer to the Entity owning the fixture passed in parameter */
            Entity* getEntityFromFixture(b2Fixture* fixture) const; 

        protected:
            /** @brief The world used in Box2D for simulation, containing all the bodies and fixtures (that we grouped in the Entity class) */
            b2World* m_world; 
            /** @brief A map containing all the entities of the world, allowing to access them with a specific name given by the user when created (unique_ptr allows polymorphism behavior) */
            std::map<std::string, std::unique_ptr<Entity>> m_entities; 
            /** @brief A map containing all the joints of the world, allowing to access them with a specific name given by the user when created (unique_ptr allows polymorphism behavior) */
            std::map<std::string, std::unique_ptr<b2Joint, NullDeleter>> m_joints; 
    };
}

#endif
