
#include <algorithm>
#include <cmath>
#include "line.hpp"

namespace Geometry
{
	Line::Line(Point mp1, Point mp2)
	{
        set(mp1, mp2);
    }

	Line::Line(Point mp1, Point mp2, Point ori)
	{
        set(mp1, mp2, ori);
    }

	void Line::set(Point mp1, Point mp2)
	{
		p1 = Point(0, 0);
        p2 = Point(std::abs(mp2.x - mp1.x), std::abs(mp2.y - mp1.y));
	}

	void Line::set(Point mp1, Point mp2, Point ori)
	{
        Point min = Point(std::min(mp1.x, mp2.x), std::min(mp1.y, mp2.y));
        Point max = Point(std::max(mp1.x, mp2.x), std::max(mp1.y, mp2.y));
		p1 = Point(ori.x - max.x, ori.y - max.y);
        p2 = Point(ori.x - min.x, ori.y - min.y);
	}

    float Line::length() const
    {
        float width = std::abs(p2.x - p1.x);
        float height = std::abs(p2.y - p1.y);
        return (float)sqrt(pow(width, 2) + pow(height, 2));
    }
}

