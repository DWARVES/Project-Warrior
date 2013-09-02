
#ifndef DEF_GEOMETRY_POLYGON
#define DEF_GEOMETRY_POLYGON

#include "point.hpp"
#include <vector>

namespace geometry
{
	struct Polygon
	{
        Polygon(const std::vector<Point>& pts);
        void set(const std::vector<Point>& pts);

        std::vector<Point> points;
	};
}

#endif

