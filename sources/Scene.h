#ifndef CSE168_SCENE_H_INCLUDED
#define CSE168_SCENE_H_INCLUDED

#include "Miro.h"
#include "Object.h"
#include "PointLight.h"
#include "BVH.h"

class Camera;
class Image;

class Scene
{
public:
    Scene();

    void addObject(Object* pObj)        {m_objects.push_back(pObj);}
    const Objects* objects() const      {return &m_objects;}

    void addLight(PointLight* pObj)     {m_lights.push_back(pObj);}
    const Lights* lights() const        {return &m_lights;}

    void preCalc();
    void openGL(Camera *cam);

    inline void setBGColor(float x, float y, float z);
    inline void setBGColor(const Vector3& color);

    inline const Vector3 & bgColor() const  {return m_bgColor;}

    inline void setSamples(int n) { m_samples = n; }
    inline int samples() const { return m_samples; }

    inline void setAntiAliasing(int a, int b) { m_samples = a * b; }

    inline void setCutoffs(int n) { m_cutoffs = n; }
    inline int cutoffs() const { return m_cutoffs; }

	void setDepthOfField(float focus_length, float lens = 1.0f);

    void raytraceImage(Camera *cam, Image *img);

    bool trace(HitInfo& minHit, const Ray& ray,
               float tMin = 0.0f, float tMax = MIRO_TMAX) const;

protected:
    std::vector<Vector3 *> traceLine(Camera const *cam, Image const *img, int j) const;
    void sampleMap(Photon_map *map, int n_photons, float total_wattage);

    Vector3 m_bgColor;

    Objects m_objects;
    BVH m_bvh;
    Lights m_lights;
    int m_global_photons;
    int m_caustics_photons;

    int m_samples;
    int m_cutoffs;

	float m_focus_length;
	float m_lens;
};

extern Scene * g_scene;

inline void Scene::setBGColor(float x, float y, float z)
{
    Vector3 v(x, y, z);
    setBGColor(v);
}

inline void Scene::setBGColor(const Vector3& vd)
{
    m_bgColor.set(vd);
}

#endif // CSE168_SCENE_H_INCLUDED
