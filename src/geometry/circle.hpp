
#ifndef DEF_GEOMETRY_CIRCLE
#define DEF_GEOMETRY_CIRCLE

#include "point.hpp"
#include "line.hpp"

namespace geometry
{
    /** @brief Describes a circle of center (0;0). */
	struct Circle
	{
        Circle();
        Circle(float r);
        /** @brief Compute the radius as the double of the diameter length.
         * @param diameter The diameter. Only its length is used, its absolute position is ignored.
         */
        Circle(const Line& diameter);
        void set(float r);
        /** @brief Compute the radius as the double of the diameter length.
         * @param diameter The diameter. Only its length is used, its absolute position is ignored.
         */
        void set(const Line& diameter);

        float radius;
	};
}

#endif

