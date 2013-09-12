#ifndef DEF_ENTITYTEST_H
#define DEF_ENTITYTEST_H

#include "physics/Entity.hpp"

#define ROTATION_FIXED true
#define ROTATION_NOT_FIXED false

using namespace geometry;

class EntityTest : public Test
{
    public:

        physics::Entity *square, *circle, *polygon;

        EntityTest()
        {
            square = new physics::Entity(m_world, Point(-300, 50), b2_dynamicBody, 0, ROTATION_NOT_FIXED);
            square->createFixture("body", AABB(100, 100), 0.1f, 1);

            circle = new physics::Entity(m_world, Point(-100, 50), b2_dynamicBody, 1, ROTATION_NOT_FIXED);
            circle->createFixture("body", Circle(50), 1, 1);

            polygon = new physics::Entity(m_world, Point(100, 50), b2_dynamicBody, 1, ROTATION_NOT_FIXED);
            std::vector<Point> vertices;
            vertices.push_back(Point(-25, 0));
            vertices.push_back(Point(0, -50));
            vertices.push_back(Point(25, 0));
            vertices.push_back(Point(0, 50));
            polygon->createFixture("body", Polygon(vertices), 1, 1);

            physics::Entity ground(m_world, Point(0, 0), b2_staticBody);
            ground.createFixture("ground", Line(Point(0, 0), Point(2500, 0), Point(1250, 0)), 1, 1);
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
