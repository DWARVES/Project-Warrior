
#ifndef DEF_GEOMETRY_LINE
#define DEF_GEOMETRY_LINE

#include "point.hpp"

namespace Geometry
{
	struct Line
	{
		Line(Point mp1, Point mp2);
		Line(Point mp1, Point mp2, Point ori);
		void set(Point mp1, Point mp2);
		void set(Point mp1, Point mp2, Point ori);

        float length() const;

		Point p1, p2;
	};
}

#endif

