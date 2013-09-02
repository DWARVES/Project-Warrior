
#include "unit_conversions.hpp"

float toMeters(float pixels)
{
	return pixels / DIST_SCALE;
}

float toPixels(float meters)
{
	return meters * DIST_SCALE;
}


float toRadians(float degrees)
{
	return degrees / ANGLE_SCALE;
}

float toDegrees(float radians)
{
	return radians * ANGLE_SCALE;
}
