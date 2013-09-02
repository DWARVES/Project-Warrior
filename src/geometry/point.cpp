
#include "point.hpp"

namespace Geometry
{
    Point::Point()
    {}

	Point::Point(float mx, float my)
	{
        set(mx, my);
    }

	void Point::set(float mx, float my)
	{
		x = mx;
		y = my;
	}
}

