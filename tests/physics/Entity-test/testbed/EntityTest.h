#ifndef DEF_ENTITYTEST_H
#define DEF_ENTITYTEST_H

#include "physics/Entity.hpp"
#include "physics/Character.hpp"
#include "core/logger.hpp"
#include <iostream>

#define ROTATION_FIXED true
#define ROTATION_NOT_FIXED false

using namespace geometry;
using namespace physics;

class EntityTest : public Test
{
    public:

        Entity *circle, *polygon;
        Character *square1, *square2, *square3;

        EntityTest()
        {
            core::logger::init();
            core::logger::addOutput(&std::cout);

            m_world->SetGravity(b2Vec2(0, -40));

            square1 = new Character("Square 1", m_world, Point(-300, 50), AABB(100, 100), Character::Weight::Heavy);
            square2 = new Character("Square 2", m_world, Point(-500, 50), AABB(100, 100), Character::Weight::Medium);
            square3 = new Character("Square 3", m_world, Point(-700, 50), AABB(100, 100), Character::Weight::Lightweight);

            circle = new Entity("Spectre attack", m_world, Point(-100, 50), b2_dynamicBody, Entity::Type::Attack, ~(Entity::Type::Obstacle | Entity::Type::Platform), 1, ROTATION_NOT_FIXED);
            circle->createFixture("body", Circle(50), 1, 1);

            polygon = new Entity("Ghost attack", m_world, Point(100, 50), b2_dynamicBody, Entity::Type::Attack, ~(Entity::Type::Character | Entity::Type::Platform), 1, ROTATION_NOT_FIXED);
            std::vector<Point> vertices;
            vertices.push_back(Point(-25, 0));
            vertices.push_back(Point(0, -50));
            vertices.push_back(Point(25, 0));
            vertices.push_back(Point(0, 50));
            polygon->createFixture("body", Polygon(vertices), 1, 1);

            Entity ground("Ground", m_world, Point(0, 0), b2_staticBody, Entity::Type::Obstacle);
            ground.createFixture("body", Line(Point(0, 0), Point(2500, 0), Point(1250, 0)), 1, 1);

            // Testing different errors
            ground.createFixture("body", Line(Point(0, 0), Point(2500, 0), Point(1250, 0)), 1, 1);
            b2Fixture* f = ground.getFixture("pouet");
        }

        ~EntityTest()
        {
            core::logger::free();
        }

        void Step(Settings* settings)
        {
            Test::Step(settings); 
        }

        static Test* Create()
        {
            return new EntityTest;
        }

        void Keyboard(unsigned char key)
        {
            switch(key)
            {
                case 'a':
                    square1->applyLinearImpulse(0, 200);
                    square2->applyLinearImpulse(0, 200);
                    square3->applyLinearImpulse(0, 200);
                    break;
                case 'e':
                    polygon->applyAngularImpulse(10);
                    break;
                case 'd':
                    square1->setXLinearVelocity(10);
                    square2->setXLinearVelocity(10);
                    square3->setXLinearVelocity(10);
                    break;
                case 'q':
                    square1->setXLinearVelocity(-10);
                    square2->setXLinearVelocity(-10);
                    square3->setXLinearVelocity(-10);
                    break;
                default:
                    //run default behaviour
                    Test::Keyboard(key);
            }
        }
};

#endif
