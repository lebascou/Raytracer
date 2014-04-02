#include "Material.h"

Material::Material() : m_castShadow(true)
{
	m_color = Vector3(1, 1, 1);
}

Material::~Material()
{
}

Vector3
Material::shade(const Ray&, const HitInfo&, const Scene&) const
{
    return m_color;
}
