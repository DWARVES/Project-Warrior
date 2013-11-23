
#ifndef DEF_GEOMETRY_AABB
#define DEF_GEOMETRY_AABB

#include "point.hpp"

namespace geometry
{
    /** @brief Describes an aligned axis bounding box. */
    struct AABB
    {
        AABB();
        AABB(float w, float h);
        /** @brief Sets this to the smaller AABB including p1 and p2. */
        AABB(Point p1, Point p2);
        void set(float w, float h);
        /** @brief Sets this to the smaller AABB including p1 and p2. */
        void set(Point p1, Point p2);

        float width, height;
    };
}

#endif

