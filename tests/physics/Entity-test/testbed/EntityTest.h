#ifndef DEF_ENTITYTEST_H
#define DEF_ENTITYTEST_H

#include "physics/Entity.hpp"

using namespace geometry;

class EntityTest : public Test
{
    public:

        EntityTest()
        {
            physics::Entity square(m_world, Point(-300, 50), b2_dynamicBody, false);
            square.createFixture("body", AABB(100, 100), 1, 1);

            physics::Entity circle(m_world, Point(-100, 50), b2_dynamicBody, false);
            circle.createFixture("body", Circle(50), 1, 1);
            
            physics::Entity polygon(m_world, Point(100, 50), b2_dynamicBody, false);
            std::vector<Point> vertices;
            vertices.push_back(Point(-25, 0));
            vertices.push_back(Point(0, -50));
            vertices.push_back(Point(25, 0));
            vertices.push_back(Point(0, 50));
            polygon.createFixture("body", Polygon(vertices), 1, 1);

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
};

#endif
