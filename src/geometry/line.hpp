
#ifndef DEF_GEOMETRY_LINE
#define DEF_GEOMETRY_LINE

#include "point.hpp"

namespace geometry
{
    /** @brief Describes a segment in a 2D plane. */
	struct Line
	{
        Line();
        /** @brief Constructs the segment going from mp1 to mp2 and report it to (0;0). */
        Line(const Point& mp1, const Point& mp2);
        /** @brief Constructs the segment going from mp1 to mp2 and report it to ori. */
        Line(const Point& mp1, const Point& mp2, const Point& ori);
        /** @brief Sets the segment going from mp1 to mp2 and report it to (0;0). */
        void set(const Point& mp1, const Point& mp2);
        /** @brief Sets the segment going from mp1 to mp2 and report it to ori. */
        void set(const Point& mp1, const Point& mp2, const Point& ori);

        /** @brief Indicates the length of the segment. */
        float length() const;

        Point p1, /**< @brief The first extremity of the segment. */
              p2; /**< @brief The second extremity of the segment. */
	};
}

#endif

