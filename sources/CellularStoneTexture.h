#ifndef CSE168_CELLULAR_STONE_TEXTURE_H_INCLUDED
#define CSE168_CELLULAR_STONE_TEXTURE_H_INCLUDED

#include "Material.h"
#include <vector>

class CellularStoneTexture : public Material
{
public:
	// tile_size: value between 0(small tiles) and 1(normal tiles)
	// sep_size: Separator between tiles (0.03 is a good value)
    CellularStoneTexture(float tile_size = 1.0f, float sep_size = 0.03f, const Vector3& sep_color = Vector3(0.9f));
    virtual ~CellularStoneTexture();

	virtual void preCalc() {}

    virtual Vector3 shade(const Ray& ray, const HitInfo& hit,
                          const Scene& scene) const;

protected:
	Vector3 getTextureColorAt(const Vector3& pos) const;

protected:
	std::vector<Vector3> m_tiles_colors;
	float m_tile_size;
	static const int m_max_order = 3;
	float m_sep_delta;
	Vector3 m_sep_color;
};

#endif // CSE168_CELLULAR_STONE_TEXTURE_H_INCLUDED
