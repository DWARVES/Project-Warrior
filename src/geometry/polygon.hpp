
#ifndef DEF_GEOMETRY_POLYGON
#define DEF_GEOMETRY_POLYGON

#include "point.hpp"
#include <vector>

namespace geometry
{
    /** @brief Describes a polygon. */
	struct Polygon
	{
        Polygon();
        Polygon(const std::vector<Point>& pts);
        void set(const std::vector<Point>& pts);
        /** @brief Return a list of triangles that constitue this polygon. */
        std::vector<Polygon> convexify() const;

        /** @brief All the points of the polygon. Their order is important. */
        std::vector<Point> points;
	};
}

#endif

