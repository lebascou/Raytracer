#include "Texture.h"
#include <boost/algorithm/clamp.hpp>
using namespace boost::gil;
using boost::algorithm::clamp;

Texture::Texture(const std::string& texture_path)
{
	m_img_loaded = false;
	setTexturePath(texture_path);
	loadTexture();
}


Texture::~Texture(void)
{
}

void 
Texture::loadTexture()
{
	if (m_texture_path.empty())
	{
		m_img_loaded = false;
		return;
	}
	try
	{
	    boost::gil::jpeg_read_image(m_texture_path, m_img);
		printf("[Texture] GIL: ('%s') texture map loaded\n", m_texture_path.c_str());
		m_img_loaded = true;
	}
	catch (std::exception &e)
	{
		printf("[Texture] GIL: ('%s') %s\n", m_texture_path.c_str(), e.what());
		m_img_loaded = false;
	}
}

void
Texture::setTexturePath(const std::string& texture_path)
{
	m_texture_path = texture_path;
	m_texture_path.erase(std::remove(m_texture_path.begin(), m_texture_path.end(), '\n'), m_texture_path.end());
	m_texture_path.erase(std::remove(m_texture_path.begin(), m_texture_path.end(), '\r'), m_texture_path.end());
	loadTexture();
}

Vector3
Texture::shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const
{
	if (m_img_loaded)
	{
		boost::gil::rgb8_pixel_t pixel = m_img._view((int) (fmodf(std::abs(hit.u), 1) * m_img.width()), (int) (fmodf(std::abs(hit.v), 1) * m_img.height()));
		return Vector3(pixel[0] / 255.f, pixel[1] / 255.f, pixel[2] / 255.f);
	}
	else
		return m_color;
}

Vector3
Texture::getPixel(float u, float v, int x, int y) const
{
    if (m_img_loaded)
    {
        int posx = fmodf(std::abs(u), 1) * m_img.width() + x;
        posx = (posx + m_img.width()) % m_img.width();
        int posy = fmodf(std::abs(v), 1) * m_img.height() + y;
        posy = (posy + m_img.height()) %m_img.height();
        boost::gil::rgb8_pixel_t pixel = m_img._view(posx, posy);
        return Vector3(pixel[0] / 255.f, pixel[1] / 255.f, pixel[2] / 255.f);
    }
    else
    {
        return m_color;
    }
}
