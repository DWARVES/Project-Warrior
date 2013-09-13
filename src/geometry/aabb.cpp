
#include "aabb.hpp"
#include <algorithm>

namespace geometry
{
    AABB::AABB()
        : width(0.0f), height(0.0f)
    {}

	AABB::AABB(float w, float h)
	{
        set(w, h);
    }

	AABB::AABB(Point p1, Point p2)
	{
		set(p1, p2);
	}

	void AABB::set(float w, float h)
	{
		width = w;
		height = h;
	}

	void AABB::set(Point p1, Point p2)
	{
		width = std::max(p1.x, p2.x) - std::min(p1.x, p2.x);
		height = std::max(p1.y, p2.y) - std::min(p1.y, p2.y);
	}
}

