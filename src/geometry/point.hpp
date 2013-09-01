
#ifndef DEF_GEOMETRY_POINT
#define DEF_GEOMETRY_POINT

namespace Geometry
{
	struct Point
	{
		Point();
		Point(float mx, float my);
		void set(float mx, float my);

		float x, y;
	};
}

#endif

