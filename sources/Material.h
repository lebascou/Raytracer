#ifndef CSE168_MATERIAL_H_INCLUDED
#define CSE168_MATERIAL_H_INCLUDED

#include "Miro.h"
#include "Vector3.h"
#include "PhotonMap.h"
#include <exception>

class Material
{
public:
    Material();
    virtual ~Material();

    virtual void preCalc() {}
    
    virtual Vector3 shade(const Ray& ray, const HitInfo& hit,
                          const Scene& scene) const;

    virtual void shadePhoton(const Ray &ray, const HitInfo &hit, const Scene &scene, Vector3 const &power, Photon_map *map) const {throw std::runtime_error("Not implemented");}

    virtual bool castShadow() const {return m_castShadow;}
    virtual void setCastShadow(bool b) {m_castShadow = b;}

    virtual bool hasBump() const { return false; }

protected:
	Vector3 m_color;
    bool m_castShadow;
};

#endif // CSE168_MATERIAL_H_INCLUDED
