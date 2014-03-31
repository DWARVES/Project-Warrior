
#include "debugDraw.hpp"
#include "graphics/graphics.hpp"

namespace physics
{
    void DebugDraw::set(graphics::Graphics* gfx)
    {
        m_gfx = gfx;
    }

    void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
    {
        DrawSolidPolygon(vertices, vertexCount, color);
    }

    void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
    {
        geometry::Polygon poly;
        poly.points.resize(vertexCount);
        for(int i = 0; i < vertexCount; ++i)
            poly.points[i] = geometry::Point(vertices[i].x, vertices[i].y);
        graphics::Color c;
        c.set(color.r, color.g, color.b, 0.5f);

        m_gfx->draw(poly, c);
    }

    void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
    {
        DrawSolidCircle(center, radius, b2Vec2(1.0f,0.0f), color);
    }

    void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
    {
        geometry::Circle circle;
        circle.radius = radius;
        graphics::Color c;
        c.set(color.r, color.g, color.b, 0.5f);

        geometry::Line ax;
        ax.p1 = geometry::Point(0.0f, 0.0f);
        float norm = std::sqrt(axis.x * axis.x + axis.y * axis.y);
        ax.p2.x = axis.x / norm * radius;
        ax.p2.y = axis.y / norm * radius;

        m_gfx->push();
        m_gfx->move(center.x, center.y);
        m_gfx->draw(circle, c);
        c.a = 255;
        m_gfx->draw(ax, c);
        m_gfx->pop();
    }

    void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
    {
        geometry::Line l;
        l.p1 = geometry::Point(p1.x, p1.y);
        l.p2 = geometry::Point(p2.x, p2.y);
        graphics::Color c;
        c.set(color.r, color.g, color.b);

        m_gfx->draw(l, c);
    }

    void DebugDraw::DrawTransform(const b2Transform&)
    {
        /* Transforms won't be drawn. */
    }

}

