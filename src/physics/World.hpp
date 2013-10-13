
#ifndef DEF_PHYSICS_WORLD
#define DEF_PHYSICS_WORLD

#include <map>
#include <memory>
#include "Box2D/Box2D.h"
#include "geometry/point.hpp"
#include "CollisionManager.hpp"
#include "Entity.hpp"
#include "Character.hpp"
#include "Attack.hpp"
#include "Platform.hpp"
#include "Obstacle.hpp"

namespace physics
{
    class World
    {
        public:
            World(const b2Vec2& gravity); // Creates a new World with gravity passed in parameter
            World(b2World* world); // Initializes m_world from world. Not so useful, except for testing purposes with Testbed
            ~World();

            Entity* getEntity(const std::string& name) const; // Returns a unique_ptr to the entity named "name" in the world entities map

            bool existsEntity(const std::string& name) const; // Check if the entity named "name" already exists

            Entity* createEntity(const std::string& name, const geometry::Point& position, const b2BodyType& bodyType, uint16 type = Entity::Type::Default, uint16 collideWith = Entity::Type::All, float gravityScale = 1, bool fixedRotation = true); // Adds an entity to the world, returning a pointer the user can use to access it (can be done later with getEntity())
            // These functions do the same that createEntity with children classes
            Character* createCharacter(const std::string& name, const geometry::Point& position, const geometry::AABB& rect, float weight);
            Attack* createAttack(const std::string& name, const geometry::Point& position, const b2BodyType& bodyType, uint16 collideWith = Attack::CollideType::Normal, float gravityScale = 1, bool fixedRotation = true);
            Platform* createPlatform(const std::string& name, const geometry::Point& position);
            Platform* createPlatform(const std::string& name, const geometry::Point& position, const geometry::AABB& rect, float friction = 1);
            Obstacle* createObstacle(const std::string& name, const geometry::Point& position);
            Obstacle* createObstacle(const std::string& name, const geometry::Point& position, const geometry::AABB& rect, float friction = 1);

        protected:
            b2World* m_world; // The world used in Box2D for simulation, containing all the bodies and fixtures (that we grouped in the Entity class)
            CollisionManager* m_colManager; // The collision manager, used for advanced collisions management not possible with masks
            std::map<std::string, std::unique_ptr<Entity>> m_entities; // A map containing all the entities of the world, allowing to access them with a specific name given by the user when created (unique_ptr allows polymorphism behavior)
    };
}

#endif
