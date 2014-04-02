#include "CellularStoneTexture.h"
#include "Ray.h"
#include "Scene.h"
#include "Perlin.h"
#include "Worley.h"
#include <algorithm>

CellularStoneTexture::CellularStoneTexture(float tile_size, float sep_size, const Vector3& sep_color)
{
	m_tile_size = 1.0f / tile_size;
	m_sep_delta = sep_size;
	m_tiles_colors.push_back(Vector3(0.6, 0.35, 0.19));
	m_tiles_colors.push_back(Vector3(0.68, 0.41, 0.21));
	m_tiles_colors.push_back(Vector3(0.82, 0.49, 0.35));
	m_tiles_colors.push_back(Vector3(0.5, 0.33, 0.16));
	m_tiles_colors.push_back(Vector3(0.42, 0.23, 0.14));
	m_sep_color = sep_color;
}

CellularStoneTexture::~CellularStoneTexture()
{
}

Vector3 
CellularStoneTexture::getTextureColorAt(const Vector3& pos) const
{
	Vector3 color(m_sep_color);
	Vector3 npos(pos);

	float at[] = {npos.x * m_tile_size, npos.y * m_tile_size, npos.z * m_tile_size};
	float F[m_max_order];
	float delta[m_max_order][3];
	unsigned int ID[m_max_order];
	float noise_scale = 0.2f;
	float p_noise = 0;

	WorleyNoise::noise3D(at, m_max_order, F, delta, ID); 
	if(F[2] - F[1] < -m_sep_delta || F[2] - F[1] > m_sep_delta) 
	{
		color = m_tiles_colors[ID[1] % m_tiles_colors.size()] + m_tiles_colors[ID[0] % m_tiles_colors.size()];
		npos *= 6;
		noise_scale = 0.05;
		p_noise = (PerlinNoise::noise(npos.x, npos.y, npos.z));
	}
	else
	{
		npos *= 18;
		p_noise = (PerlinNoise::noise(npos.x, npos.y, npos.z)) + 0.6;	
	}
	color = (1.0f - noise_scale) * color + noise_scale * p_noise;
	return color;
}

Vector3
CellularStoneTexture::shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const
{
	return getTextureColorAt(hit.P);
}
