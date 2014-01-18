
#ifndef DEF_PHYSICS_PLATFORM
#define DEF_PHYSICS_PLATFORM

#include "Entity.hpp"

namespace physics
{
    /** @brief Entity based class facilitating creation of platforms */
    class Platform : public Entity
    {
        public:
            /** @brief Creates an empty platform (without fixture) */
            Platform(const std::string& name, b2World* world, const geometry::Point& position);
            /** @brief Creates a basic platform with AABB fixture */
            Platform(const std::string& name, b2World* world, const geometry::Point& position, const geometry::AABB& rect, float friction = 1);
    };
}

#endif
