#pragma once
#include "material.h"
#include "Ray.h"
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>

class Texture :
	public Material
{
public:
	Texture() { m_img_loaded = false; }; // A texture without a path is a basic material (one color)
	Texture(const std::string& texture_path);
	virtual ~Texture(void);

	virtual void preCalc() {}
    
    virtual Vector3 shade(const Ray& ray, const HitInfo& hit,
                          const Scene& scene) const;
	void setTexturePath(const std::string& texture_path);
	const std::string& getTexturePath() { return m_texture_path; }
    Vector3 getPixel(float u, float v, int x, int y) const;

protected:
	void loadTexture();

private:
	std::string m_texture_path;
	boost::gil::rgb8_image_t m_img;
	bool m_img_loaded;
};

