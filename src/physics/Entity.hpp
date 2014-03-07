
#ifndef DEF_PHYSICS_ENTITY
#define DEF_PHYSICS_ENTITY

#include <map>
#include "Box2D/Box2D.h"
#include "geometry/point.hpp"
#include "geometry/line.hpp"
#include "geometry/aabb.hpp"
#include "geometry/circle.hpp"
#include "geometry/polygon.hpp"

namespace physics
{
    /** @brief The class representing the physical entities in the World. An entity is composed of one or many fixtures corresponding to geometric shapes with specific physical properties. */
    class Entity
    {
        public:
            /** @brief Bit flags enum used by collision masks between entity types */
            typedef enum 
            {
                /** Default type */
                Default = 0x0001,
                /** A character */
                Character = 0x0002,
                /** An attack */
                Attack = 0x0004,
                /** A platform */
                Platform = 0x0008,
                /** An obstacle */
                Obstacle = 0x0010,
                /** Any type (used only with collideWith) */
                All = 0xFFFF,

                /** Value assigned in createBaseFixtureDef to m_type */
                ThisType = 0xAAAA, 
                /** Value assigned in createBaseFixtureDef to m_collideWith */
                ThisCollideWith = 0xBBBB

            } Type;

        public:
            /** @brief Default constructor */
            Entity();
            /** @brief Constructor defining some attributes and physical parameters ; should not be used directly to instanciate a b2Body in a b2World, but only through World::createEntity method */
            Entity(const std::string& name, b2World* world, const geometry::Point& position, const b2BodyType& bodyType, uint16 type = Type::Default, uint16 collideWith = Type::All, float gravityScale = 1, bool fixedRotation = true);

            /** @brief Return a pointer to the entity's b2Body ; should be used only by other physics engine classes */
            b2Body* getBody() const; 
            /** @brief Returns a pointer to the fixture named "name" in the entity fixtures map */
            b2Fixture* getFixture(const std::string& name) const;

            /** @brief Returns the type of the entity */
            uint16 getType() const;
            /** @brief Returns the current position of the entity */
            geometry::Point getPosition() const;

            /** @brief Returns the entity horizontal linear velocity */
            float getXLinearVelocity();
            /** @brief Returns the entity vertical linear velocity */
            float getYLinearVelocity();

            /** @brief Returns the entity angular velocity */
            float getAngularVelocity();

            /** @brief Sets the entity horizontal linear velocity to velX */
            void setXLinearVelocity(float velX);
            /** @brief Sets the entity vertical linear velocity to velY */
            void setYLinearVelocity(float velY);
            /** @brief Sets the entity linear velocity to (velX ; velY) */
            void setLinearVelocity(float velX, float velY);

            /** @brief Sets the entity angular velocity to omega */
            void setAngularVelocity(float omega);

            /** @brief Defines if the entity should rotate (fixedRotation = false) or not (fixedRotation = false) */
            void setFixedRotation(bool fixedRotation);

            /** @brief Creates a fixture based on a line-shaped hitbox, automatically adding it to the entity's fixtures map, and returning a pointer to the fixture */
            b2Fixture* createFixture(const std::string& name, const geometry::Line& line, float density = 1, float friction = 1, uint16 type = Type::ThisType, uint16 collideWith = Type::ThisCollideWith);
            /** @brief Creates a fixture based on an aabb-shaped hitbox, automatically adding it to the entity's fixtures map, and returning a pointer to the fixture */
            b2Fixture* createFixture(const std::string& name, const geometry::AABB& aabb, float density = 1, float friction = 1, uint16 type = Type::ThisType, uint16 collideWith = Type::ThisCollideWith, const geometry::Point& position = geometry::Point(0, 0));
            /** @brief Creates a fixture based on a circle-shaped hitbox, automatically adding it to the entity's fixtures map, and returning a pointer to the fixture */
            b2Fixture* createFixture(const std::string& name, const geometry::Circle& circle, float density = 1, float friction = 1, uint16 type = Type::ThisType, uint16 collideWith = Type::ThisCollideWith, const geometry::Point& position = geometry::Point(0, 0));
            /** @brief Creates a fixture based on an polygon-shaped hitbox, automatically adding it to the entity's fixtures map, and returning a pointer to the fixture */
            b2Fixture* createFixture(const std::string& name, const geometry::Polygon& polygon, float density = 1, float friction = 1, uint16 type = Type::ThisType, uint16 collideWith = Type::ThisCollideWith);

            /** @brief Destroy the fixture named "name" in the map */
            void destroyFixture(const std::string& name);

            /** @brief Applies a force of coordinates (forceX ; forceY) to the entity's center */
            void applyForce(float forceX, float forceY);
            /** @brief Applies a force of coordinates (forceX ; forceY) to the entity's point taken in parameter */
            void applyForce(float forceX, float forceY, const geometry::Point& point);
            /** @brief Applies a linear impulse of coordinates (forceX ; forceY) to the entity's center */
            void applyLinearImpulse(float forceX, float forceY);
            /** @brief Applies a linear impulse of coordinates (forceX ; forceY) to the entity's point taken in parameter */
            void applyLinearImpulse(float forceX, float forceY, const geometry::Point& point);

            /** @brief Applies an angular force to the entity */
            void applyTorque(float force);
            /** @brief Applies an angular impulse to the entity */
            void applyAngularImpulse(float force);

        protected:
            /** @brief Creates a base b2FixtureDef based on density, friction and collision parameters, and return a pointer to it ; used in createFixture overloaded functions to avoid code repetitions */
            b2FixtureDef* createBaseFixtureDef(const std::string& name, float density, float friction, uint16 type, uint16 collideWith) const;

        protected:
            /** @brief The entity name */
            std::string m_name; 
            /** @brief Defines the entity type */
            uint16 m_type;
            /** @brief Defines with which types the entity will collide */
            uint16 m_collideWith;

            /** @brief The b2Body of the entity, used in Box2D simulations */
            b2Body* m_body;
            /** @brief A map containing all the fixtures of the body, allowing to access them with a specific name given by the user when created */
            std::map<std::string, b2Fixture*> m_fixtures;
    };
}

#endif
