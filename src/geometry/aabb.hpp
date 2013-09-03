
#ifndef DEF_GEOMETRY_AABB
#define DEF_GEOMETRY_AABB

#include "point.hpp"

namespace geometry
{
    struct AABB
    {
        AABB(float w, float h);
        AABB(Point p1, Point p2);
        void set(float w, float h);
        void set(Point p1, Point p2);

        float width, height;
    };
}

#endif

