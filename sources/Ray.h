#ifndef CSE168_RAY_H_INCLUDED
#define CSE168_RAY_H_INCLUDED

#include "Vector3.h"
#include <vector>

#define MAX_RAY_ITER 6

class Ray
{
public:
    Vector3 o,      //!< Origin of ray
            d;      //!< Direction of ray
    int iter;
    std::vector<float> *refractionStack;
    int refractionIndex;
    bool seen_specular; // Only for photon map generation

    Ray() : o(), d(Vector3(0.0f,0.0f,1.0f)), iter(0), seen_specular(false)
    {
        // empty
    }

    Ray(const Vector3& o, const Vector3& d, int i = 0) : o(o), d(d), iter(i)
    {
        // empty
    }
};


//! Contains information about a ray hit with a surface.
/*!
    HitInfos are used by object intersection routines. They are useful in
    order to return more than just the hit distance.
*/
class HitInfo
{
public:
    float t;                            //!< The hit distance
    Vector3 P;                          //!< The hit point
    Vector3 N;                          //!< Shading normal vector
    const Material* material;           //!< Material of the intersected object
    Vector3 tangent;
	float u;
	float v;

    //! Default constructor.
    explicit HitInfo(float t = 0.0f,
                     const Vector3& P = Vector3(),
                     const Vector3& N = Vector3(0.0f, 1.0f, 0.0f)) :
        t(t), P(P), N(N), material (0), u(0), v(0)
    {
        // empty
    }
};

#endif // CSE168_RAY_H_INCLUDED
