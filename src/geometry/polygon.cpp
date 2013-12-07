
#include "polygon.hpp"

namespace geometry
{
    Polygon::Polygon()
    {
        /* do nothing, std::vector default constructor is enough */
    }

	Polygon::Polygon(const std::vector<Point>& pts)
	{
        set(pts);
    }

	void Polygon::set(const std::vector<Point>& pts)
	{
		points = pts;
	}
        
    std::vector<Polygon> Polygon::convexify() const
    {
        std::vector<Polygon> convexes;
        std::vector<Point> temp = points;
        if(temp.size() < 3) {
            convexes.resize(1);
            convexes[0] = *this;
            return convexes;
        }

        convexes.reserve(temp.size() - 2);
        for(unsigned int i = 1; i < temp.size() - 1; ++i) {
            Polygon poly;
            poly.points.resize(3);
            poly.points[0] = temp[i-1];
            poly.points[1] = temp[i];
            poly.points[2] = temp[i+1];
            temp[i] = temp[i-1];
            convexes.push_back(poly);
        }

        return convexes;
    }
}

