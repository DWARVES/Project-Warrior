
#ifndef DEF_GEOMETRY_CIRCLE
#define DEF_GEOMETRY_CIRCLE

#include "point.hpp"
#include "line.hpp"

namespace geometry
{
	struct Circle
	{
        Circle(float r);
        Circle(const Line& diameter);
        void set(float r);
        void set(const Line& diameter);

        float radius;
	};
}

#endif

