#ifndef CSE168_LAMBERT_H_INCLUDED
#define CSE168_LAMBERT_H_INCLUDED

#include "Material.h"
#include "Texture.h"
#include "Ray.h"
#include "Scene.h"
#include "Perlin.h"

class Phong : public Material
{
public:
    Phong(const Vector3 & diffuse, const Vector3 &specular, const Vector3 &ambient);
	Phong(shared_ptr<Material> kd, shared_ptr<Material> ks, const Vector3 &ambient);
	Phong(shared_ptr<Material> kd, const Vector3 &ambient = Vector3(0.f));
    virtual ~Phong();

    void setPhong(float f) {m_phong = f;}
    void setRefraction(float indice) {m_refr = indice;}
    void setIndirectLighting(bool value) {m_indirect = value;}

    virtual void preCalc() {}
    
    virtual Vector3 shade(const Ray& ray, const HitInfo& hit,
                          const Scene& scene) const;
    virtual void shadePhoton(const Ray &ray, const HitInfo &hit, const Scene &scene, Vector3 const &power, Photon_map *map) const;

	void setTransparency(float tp) { m_tp = tp; }
    
	void setKa(const Vector3& ka) { m_ka = ka; }
	void setKs(const Vector3& ks) { m_ks = ks; }
	const Vector3& getKs() { return m_ks; }
	void setKd(const Vector3& kd) { m_kd = kd; }
	void setBm(float bm) { m_bm = bm; }
	void setGlossy(bool is_glossy) { m_is_glossy = true; }
	bool isGlossy() { return m_is_glossy; }
	void setBumpNoisy(float height, int turbulence = 1) { m_is_bump_noisy = true; m_bump_noise_height = height; m_bump_noise_turbulence = turbulence; }
	bool isBumpNoisy() { return m_is_bump_noisy; }
	void setIsBumpNoisy(bool is_noisy) { m_is_bump_noisy = is_noisy; }

    virtual bool hasBump() const {return m_texture_bump;}

	void setKaTexture(shared_ptr<Material> ka_texture) { m_texture_ka = ka_texture; }
	void setKsTexture(shared_ptr<Material> ks_texture) { m_texture_ks = ks_texture; }
	void setKdTexture(shared_ptr<Material> kd_texture) { m_texture_kd = kd_texture; }
	void setBumpTexture(shared_ptr<Texture> bump_texture) { m_texture_bump = bump_texture; }

protected:
	HitInfo bumpHit(HitInfo const &rhit) const;

protected:
	shared_ptr<Material> m_texture_ka;
    shared_ptr<Material> m_texture_ks;
	shared_ptr<Material> m_texture_kd;
	shared_ptr<Texture> m_texture_bump;

    Vector3 m_ka;
	Vector3 m_ks; // old: m_sp
	Vector3 m_kd; // old: m_dp
	float m_bm;

    float m_tp;
    float m_phong;
    float m_refr;

    bool m_indirect;
	bool m_is_glossy;

	bool m_is_bump_noisy;
	float m_bump_noise_height;
	int m_bump_noise_turbulence;
};

#endif // CSE168_LAMBERT_H_INCLUDED
