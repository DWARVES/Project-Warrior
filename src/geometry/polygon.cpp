
#include "polygon.hpp"

namespace Geometry
{
	Polygon::Polygon(const std::vector<Point>& pts)
	{
        set(pts);
    }

	void Polygon::set(const std::vector<Point>& pts)
	{
		m_points = pts;
	}

	std::vector<Point> Polygon::points() const
	{
		return m_points;
	}
}

