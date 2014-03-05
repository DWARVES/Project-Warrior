
#include "image.hpp"

namespace gui
{
    unsigned int Image::m_count;

    Image::Image(graphics::Graphics* gfx)
        : Widget(gfx), m_namespace(createNamespace()), m_ratio(false)
    {}

    Image::Image(graphics::Graphics* gfx, const std::string& path, bool real)
        : Image(gfx)
    {
        set(path, real);
    }
            
    Image::~Image()
    {
        m_gfx->deleteNamespace(m_namespace);
    }

    void Image::set(const std::string& path, bool real)
    {
        m_gfx->enterNamespace(m_namespace);
        if(!m_name.empty())
            m_gfx->free(m_name);

        m_name = "picture";
        if(!real)
            m_gfx->link(m_name, path);
        else
            m_gfx->loadTexture(m_name, path);
        updatePosRect();
    }

    bool Image::ratio(bool r)
    {
        m_ratio = r;
        updatePosRect();
        return m_ratio;
    }

    bool Image::ratio() const
    {
        return m_ratio;
    }

    float Image::width(float w)
    {
        float r = Widget::width(w);
        updatePosRect();
        return r;
    }

    float Image::height(float h)
    {
        float r = Widget::height(h);
        updatePosRect();
        return r;
    }

    float Image::width() const
    {
        return Widget::width();
    }

    float Image::height() const
    {
        return Widget::height();
    }

    void Image::draw()
    {
        if(m_name.empty())
            return;

        m_gfx->push();
        m_gfx->enterNamespace(m_namespace);
        m_gfx->move(m_pos.x, m_pos.y);
        m_gfx->draw(m_rect, m_name);
        m_gfx->pop();
    }

    void Image::updatePosRect()
    {
        if(m_name.empty())
            return;

        if(!m_ratio) {
            m_pos.x = m_pos.y = 0;
            m_rect.width  = width();
            m_rect.height = height();
            return;
        }

        m_gfx->enterNamespace(m_namespace);
        float w = (float)m_gfx->getTextureWidth(m_name);
        float h = (float)m_gfx->getTextureHeight(m_name);
        float r1 = w/h;
        float r2 = width() / height();

        if(r2 > r1) {
            m_rect.height = height();
            m_rect.width  = height() * r1;
            m_pos.y       = 0.0f;
            m_pos.x       = (width() - m_rect.width) / 2.0f;
        }
        else {
            m_rect.width  = width();
            m_rect.height = width() / r1;
            m_pos.x       = 0.0f;
            m_pos.y       = (height() - m_rect.height) / 2.0f;
        }
    }

    std::string Image::createNamespace()
    {
        std::ostringstream oss;
        oss << "/gui/image/" << m_count << "/";
        ++m_count;
        m_gfx->createNamespace(oss.str());
        return oss.str();
    }

}


