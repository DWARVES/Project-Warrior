
#include "polygon.hpp"

namespace Geometry
{
	Polygon::Polygon(const std::vector<Point>& pts)
	{
        set(pts);
    }

	void Polygon::set(const std::vector<Point>& pts)
	{
		points = pts;
	}
}

