#ifndef DEF_ENTITYTEST_H
#define DEF_ENTITYTEST_H

#include "physics/CollisionManager.hpp"
#include "physics/Entity.hpp"
#include "physics/Character.hpp"
#include "physics/Attack.hpp"
#include "physics/Platform.hpp"
#include "physics/Obstacle.hpp"
#include "core/logger.hpp"
#include <iostream>

#define ROTATION_FIXED true
#define ROTATION_NOT_FIXED false

using namespace geometry;
using namespace physics;

class EntityTest : public Test
{
    public:

        CollisionManager *colManager;

        Attack *circle, *polygon;
        Character *square1, *square2, *square3;
        Platform *platform;
        Obstacle *ground;

        EntityTest()
        {
            core::logger::init();
            core::logger::addOutput(&std::cout);

            m_world->SetGravity(b2Vec2(0, -40));
            colManager = new CollisionManager();
            m_world->SetContactListener(colManager);

            square1 = new Character("Square 1", m_world, Point(-300, 50), AABB(100, 100), Character::Weight::Heavy);
            square2 = new Character("Square 2", m_world, Point(-500, 50), AABB(100, 100), Character::Weight::Medium);
            square3 = new Character("Square 3", m_world, Point(-700, 50), AABB(100, 100), Character::Weight::Lightweight);

            circle = new Attack("Spectre attack", m_world, Point(-100, 500), b2_dynamicBody, Attack::CollideType::Spectre);
            circle->createFixture("body", Circle(50));

            polygon = new Attack("Ghost attack", m_world, Point(100, 50), b2_dynamicBody, Attack::CollideType::Ghost);
            std::vector<Point> vertices;
            vertices.push_back(Point(-25, 0));
            vertices.push_back(Point(0, -50));
            vertices.push_back(Point(25, 0));
            vertices.push_back(Point(0, 50));
            polygon->createFixture("body", Polygon(vertices));

            platform = new Platform("Platform of the death", m_world, Point(-100, 300), AABB(500, 20));

            ground = new Obstacle("Ground", m_world, Point(0, 0), AABB(2500, 1));

            // Testing different errors
            ground->createFixture("body", Line(Point(0, 0), Point(2500, 0), Point(1250, 0)), 1, 1);
            b2Fixture* f = ground->getFixture("pouet");
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
                    square1->jump(25);
                    square2->jump(25);
                    square3->jump(25);
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
