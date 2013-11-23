
#ifndef DEF_GEOMETRY_POINT
#define DEF_GEOMETRY_POINT

/** @brief Contains all classes describing shapes. */
namespace geometry
{
    /** @brief Describes a point in a 2D plane. */
	struct Point
	{
        Point();
		Point(float mx, float my);
		void set(float mx, float my);

		float x, y;
	};
}

#endif

