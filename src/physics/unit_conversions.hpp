
#ifndef DEF_PHYSICS_UNITCONVERSIONS
#define DEF_PHYSICS_UNITCONVERSIONS

#include "Box2D/Box2D.h"

namespace physics
{
    const float DIST_SCALE = 30.f;
    const float ANGLE_SCALE = 180 / b2_pi;

    float toMeters(float pixels);
    float toPixels(float meters);

    float toRadians(float degrees);
    float toDegrees(float radians);
}

#endif
