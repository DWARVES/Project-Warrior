#ifndef DEF_ENTITYTEST_H
#define DEF_ENTITYTEST_H

#include "physics/Entity.hpp"
#include "core/logger.hpp"
#include <iostream>

#define ROTATION_FIXED true
#define ROTATION_NOT_FIXED false

using namespace geometry;
using namespace physics;

class EntityTest : public Test
{
    public:

        Entity *square, *circle, *polygon;

        EntityTest()
        {
            core::logger::init();
            core::logger::addOutput(&std::cout);

            square = new Entity("Character", m_world, Point(-300, 50), b2_dynamicBody, Entity::Type::Character, ~Entity::Type::Platform, 0, ROTATION_NOT_FIXED);
            square->createFixture("body", AABB(100, 100), 1, 1);

            circle = new Entity("Spectre attack", m_world, Point(-100, 50), b2_dynamicBody, Entity::Type::Attack, ~(Entity::Type::Obstacle | Entity::Type::Platform), 1, ROTATION_NOT_FIXED); // SPECTRE ATTACK
            circle->createFixture("body", Circle(50), 1, 1);

            polygon = new Entity("Ghost attack", m_world, Point(100, 50), b2_dynamicBody, Entity::Type::Attack, ~(Entity::Type::Character | Entity::Type::Platform), 1, ROTATION_NOT_FIXED); // GHOST ATTACK
            std::vector<Point> vertices;
            vertices.push_back(Point(-25, 0));
            vertices.push_back(Point(0, -50));
            vertices.push_back(Point(25, 0));
            vertices.push_back(Point(0, 50));
            polygon->createFixture("body", Polygon(vertices), 1, 1);

            Entity ground("Ground", m_world, Point(0, 0), b2_staticBody, Entity::Type::Obstacle);
            ground.createFixture("body", Line(Point(0, 0), Point(2500, 0), Point(1250, 0)), 1, 1);
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
                    square->applyLinearImpulse(b2Vec2(0, 10));
                    break;
                case 'd':
                    polygon->applyAngularImpulse(10);
                    break;
                default:
                    //run default behaviour
                    Test::Keyboard(key);
            }
        }
};

#endif
