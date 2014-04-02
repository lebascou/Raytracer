#include "Sphere.h"
#include "Ray.h"
#include "Console.h"

Sphere::Sphere()
{
}

Sphere::~Sphere()
{
}

void
Sphere::renderGL()
{
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(m_center.x, m_center.y, m_center.z);
    glutWireSphere(m_radius, 20, 20);
    glPopMatrix();
}

bool Sphere::doIntersect(IntersectObjects const &objs, HitInfo& result, const Ray& ray,
                         float tMin, float tMax)
{
    bool hit = false;
    for (int i = 0; i < objs.plain.size(); ++i)
    {
        Sphere *s = (Sphere *)objs.plain[i];
        const Vector3 toO = ray.o - s->m_center; 

        const float a = ray.d.length2(); 
        const float b = dot(2*ray.d, toO);
        const float c = toO.length2() - s->m_radius*s->m_radius;

        const float discrim = b*b-4.0f*a*c; 

        if (discrim < 0) 
        {
            continue;   // quadratic equation would yield imaginary numbers
        }

        const float sqrt_discrim = sqrt(discrim); 

        // solve the quadratic equation
        const float t[2] = {(-b-sqrt_discrim)/(2.0f*a), (-b+sqrt_discrim)/(2.0f*a)}; 

        // since we know that discrim >= 0, t[0] < t{1]
        // return the t closest to us that is within range
        if ((t[0] > tMin) && (t[0] < tMax))
        {
            result.t = t[0];
        }
        else if((t[1] > tMin) && (t[1] < tMax))
        {
            result.t = t[1];
        }
        else
        {
            // neither of the solutions are in the required range
            continue; 
        }

        hit = true;

        tMax = result.t;
        result.P = ray.o + result.t*ray.d; 
        result.N = (result.P-s->m_center); 
        result.N.normalize(); 
        result.material = s->m_material; 
    }
    return hit;
}

Vector3 Sphere::maxVector() const
{
    return m_center + Vector3(m_radius);
}

Vector3 Sphere::minVector() const
{
    return m_center - Vector3(m_radius);
}
