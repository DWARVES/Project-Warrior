
#ifndef DEF_PHYSICS_ENTITY
#define DEF_PHYSICS_ENTITY

#include <map>
#include <Box2D/Box2D.h>
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
            typedef enum
            {
                Character = 0x0001,
                Attack = 0x0002,
                Platform = 0x0004,
                Obstacle = 0x0008,

            } Type;

        public:
            Entity();
            Entity(b2World* world, const geometry::Point& position, const b2BodyType& bodyType, float gravityScale = 1, bool fixedRotation = true);

            b2Fixture* getFixture(const std::string& name) const; // Returns a pointer to the fixture named "name" in the entity fixtures map
            
            // Overloaded functions used to add a fixture to the entity's body depending on its geometric shape, returning a pointer the user can use to access it (can also be done later with getFixture())
            b2Fixture* createFixture(const std::string& name, const geometry::Line& line, float density, float friction);
            b2Fixture* createFixture(const std::string& name, const geometry::AABB& aabb, float density, float friction, const geometry::Point& position = geometry::Point(0, 0));
            b2Fixture* createFixture(const std::string& name, const geometry::Circle& circle, float density, float friction, const geometry::Point& position = geometry::Point(0, 0));
            b2Fixture* createFixture(const std::string& name, const geometry::Polygon& polygon, float density, float friction);

            // Overloaded functions used to apply a linear force to the entity's body ; the force is applied to its center when the point is not specified
            void applyForce(const b2Vec2& force);
            void applyForce(const b2Vec2& force, const geometry::Point& point);
            // Overloaded functions used to apply a linear impulse to the entity's body ; the force is applied to its center when the point is not specified
            void applyLinearImpulse(const b2Vec2& force);
            void applyLinearImpulse(const b2Vec2& force, const geometry::Point& point);

            void applyTorque(float force); // Apply an angular force (torque) to the entity's body
            void applyAngularImpulse(float force); // Apply an angular impulse to the entity's body

        protected:
            b2FixtureDef* createBaseFixtureDef(float density, float friction) const; // Creates a base b2FixtureDef based on density and friction, and return a pointer to it ; used in createFixture overloaded functions to avoid code repetitions

        protected:
            b2Body* m_body; // The b2Body of the entity, used in Box2D simulations
            std::map<std::string, b2Fixture*> m_fixtures; // A map containing all the fixtures of the body, allowing to access them with a specific name given by the user when created
    };
}

#endif
