
#ifndef DEF_GEOMETRY_LINE
#define DEF_GEOMETRY_LINE

#include "point.hpp"

namespace Geometry
{
	struct Line
	{
        Line();
        Line(const Point& mp1, const Point& mp2);
        Line(const Point& mp1, const Point& mp2, const Point& ori);
        void set(const Point& mp1, const Point& mp2);
        void set(const Point& mp1, const Point& mp2, const Point& ori);

        float length() const;

        Point p1, p2;
	};
}

#endif

