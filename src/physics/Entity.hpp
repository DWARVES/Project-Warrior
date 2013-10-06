
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
    class Entity
    {
        public:
            // Bit flags enum used by collision masks between entity types
            typedef enum 
            {
                Default = 0x0001, // Only used for categoryBits
                Character = 0x0002,
                Attack = 0x0004,
                Platform = 0x0008,
                Obstacle = 0x0010,
                All = 0xFFFF, // Only used for maskBits

                ThisType = 0xAAAA, // Value assigned in createBaseFixtureDef to m_type
                ThisCollideWith = 0xBBBB // Value assigned in createBaseFixtureDef to m_collideWith

            } Type;

        public:
            Entity();
            Entity(const std::string& name, b2World* world, const geometry::Point& position, const b2BodyType& bodyType, uint16 type = Type::Default, uint16 collideWith = Type::All, float gravityScale = 1, bool fixedRotation = true);

            b2Fixture* getFixture(const std::string& name) const; // Returns a pointer to the fixture named "name" in the entity fixtures map
            uint16 getType() const; // Returns the type of the Entity
            geometry::Point getPosition() const; // Returns the current position of the entity

            // Functions to get the entity linear velocity
            float getXLinearVelocity();
            float getYLinearVelocity();

            float getAngularVelocity(); // Get angular velocity

            // Functions to set the entity linear velocity
            void setXLinearVelocity(float velX);
            void setYLinearVelocity(float velY);
            void setLinearVelocity(float velX, float velY);

            void setAngularVelocity(float omega); // Set angular velocity

            void setFixedRotation(bool fixedRotation); // Set fixedRotation either true or false
            
            // Overloaded functions used to add a fixture to the entity's body depending on its geometric shape, returning a pointer the user can use to access it (can also be done later with getFixture())
            b2Fixture* createFixture(const std::string& name, const geometry::Line& line, float density = 1, float friction = 1, uint16 type = Type::ThisType, uint16 collideWith = Type::ThisCollideWith);
            b2Fixture* createFixture(const std::string& name, const geometry::AABB& aabb, float density = 1, float friction = 1, uint16 type = Type::ThisType, uint16 collideWith = Type::ThisCollideWith, const geometry::Point& position = geometry::Point(0, 0));
            b2Fixture* createFixture(const std::string& name, const geometry::Circle& circle, float density = 1, float friction = 1, uint16 type = Type::ThisType, uint16 collideWith = Type::ThisCollideWith, const geometry::Point& position = geometry::Point(0, 0));
            b2Fixture* createFixture(const std::string& name, const geometry::Polygon& polygon, float density = 1, float friction = 1, uint16 type = Type::ThisType, uint16 collideWith = Type::ThisCollideWith);

            // Overloaded functions used to apply a linear force to the entity's body ; the force is applied to its center when the point is not specified
            void applyForce(float forceX, float forceY);
            void applyForce(float forceX, float forceY, const geometry::Point& point);
            // Overloaded functions used to apply a linear impulse to the entity's body ; the force is applied to its center when the point is not specified
            void applyLinearImpulse(float forceX, float forceY);
            void applyLinearImpulse(float forceX, float forceY, const geometry::Point& point);

            void applyTorque(float force); // Apply an angular force (torque) to the entity's body
            void applyAngularImpulse(float force); // Apply an angular impulse to the entity's body

        protected:
            b2FixtureDef* createBaseFixtureDef(const std::string& name, float density, float friction, uint16 type, uint16 collideWith) const; // Creates a base b2FixtureDef based on density, friction and collision parameters, and return a pointer to it ; used in createFixture overloaded functions to avoid code repetitions

        protected:
            std::string m_name; // The entity name
            uint16 m_type, m_collideWith;

            b2Body* m_body; // The b2Body of the entity, used in Box2D simulations
            std::map<std::string, b2Fixture*> m_fixtures; // A map containing all the fixtures of the body, allowing to access them with a specific name given by the user when created
    };
}

#endif
