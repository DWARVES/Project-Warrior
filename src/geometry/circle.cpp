
#include <cmath>
#include "circle.hpp"

namespace Geometry
{
	Circle::Circle()
	{
        set(0);
    }

	Circle::Circle(float r)
	{
		set(r);
	}

	Circle::Circle(const Line& diameter)
	{
		set(diameter);
	}

	void Circle::set(float r)
	{
		radius = r;
	}

	void Circle::set(const Line& diameter)
	{
		set(diameter.length());
	}

}

