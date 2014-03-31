
#ifndef DEF_PHYSICS_DEBUGDRAW
#define DEF_PHYSICS_DEBUGDRAW

#include "Box2D/Box2D.h"

namespace graphics
{
    class Graphics;
}

namespace physics
{
    /** @brief A class used to draw physics, for debug.
     * This class is reserved for physics::World use.
     */
    class DebugDraw : public b2Draw
    {
        public:
            DebugDraw() = default;
            void set(graphics::Graphics* gfx);
            ~DebugDraw() = default;

            /* Drawing funtions. */
            virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
            virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
            virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
            virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
            virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
            virtual void DrawTransform(const b2Transform&);

        private:
            /** @brief The graphics instance to draw to.
             * The virtual isn't changed, nor the axes are, so they must be configured before by the user.
             */
            graphics::Graphics* m_gfx;
    };
}

#endif

