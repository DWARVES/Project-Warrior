
#ifndef DEF_GEOMETRY_POLYGON
#define DEF_GEOMETRY_POLYGON

#include "point.hpp"
#include <vector>

namespace Geometry
{
	class Polygon
	{
		public:
			Polygon();
			Polygon(const std::vector<Point>& pts);
			void set(const std::vector<Point>& pts);

			std::vector<Point> points() const;

		private:
			std::vector<Point> m_points;
	};
}

#endif

