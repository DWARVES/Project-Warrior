
#include "polygon.hpp"

namespace geometry
{
    Polygon::Polygon()
    {
        /* do nothing, std::vector default constructor is enough */
    }

	Polygon::Polygon(const std::vector<Point>& pts)
	{
        set(pts);
    }

	void Polygon::set(const std::vector<Point>& pts)
	{
		points = pts;
	}
}

