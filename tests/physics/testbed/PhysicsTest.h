#ifndef DEF_PHYSICSTEST_H
#define DEF_PHYSICSTEST_H

#include "physics/CollisionManager.hpp"
#include "physics/World.hpp"
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

class PhysicsTest : public Test
{
    public:

        CollisionManager *colManager;

        World *world;

        Attack *circle, *polygon;
        Character *square1, *square2, *square3;
        Platform *platform;
        Obstacle *ground;

        PhysicsTest()
        {
            m_world->SetGravity(b2Vec2(0, -40));
            colManager = new CollisionManager();
            m_world->SetContactListener(colManager);

            world = new World(m_world);

            square1 = world->createCharacter("Square 1", Point(-300, 50), AABB(100, 100), Character::Weight::Heavy);
            square2 = world->createCharacter("Square 2", Point(-500, 50), AABB(100, 100), Character::Weight::Medium);
            square3 = world->createCharacter("Square 3", Point(-700, 50), AABB(100, 100), Character::Weight::Lightweight);

            circle = world->createAttack("Spectre attack", Point(-100, 500), b2_dynamicBody, Attack::CollideType::Spectre);
            circle->createFixture("body", Circle(50));

            polygon = world->createAttack("Ghost attack", Point(100, 50), b2_dynamicBody, Attack::CollideType::Ghost);
            //polygon = world->getEntity("Ghost Attack").get();
            std::vector<Point> vertices;
            vertices.push_back(Point(-25, 0));
            vertices.push_back(Point(0, -50));
            vertices.push_back(Point(25, 0));
            vertices.push_back(Point(0, 50));
            polygon->createFixture("body", Polygon(vertices));

            platform = world->createPlatform("Platform of the death", Point(-100, 300), AABB(500, 20));

            ground = world->createObstacle("Ground", Point(0, 0), AABB(2500, 1));

            // Testing different errors
            ground->createFixture("body", Line(Point(0, 0), Point(2500, 0), Point(1250, 0)), 1, 1);
            b2Fixture* f = ground->getFixture("pouet");
        }

        ~PhysicsTest()
        {}

        void Step(Settings* settings)
        {
            Test::Step(settings); 
        }

        static Test* Create()
        {
            return new PhysicsTest;
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
