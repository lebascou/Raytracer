#ifndef CSE168_POINTLIGHT_H_INCLUDED
#define CSE168_POINTLIGHT_H_INCLUDED

#include <vector>
#include "Vector3.h"

class PointLight
{
public:
    PointLight(): m_sphere(0), m_samples(1) {}
    void setPosition(const Vector3& v)  {m_position = v;}
    void setColor(const Vector3& v)     {m_color = v;}
    void setWattage(float f)            {m_wattage = f;}
    void setBlur(float sphere, int samples) {m_sphere = sphere; m_samples = samples;}
    
    float wattage() const               {return m_wattage;}
    const Vector3 & color() const       {return m_color;}
    const Vector3& position() const     {return m_position;}
    float sphere() const                {return m_sphere;}
    int samples() const                 {return m_samples;}

    void preCalc() {} // use this if you need to

protected:
    Vector3 m_position;
    Vector3 m_color;
    float m_wattage;
    float m_sphere;
    int m_samples;
};

typedef std::vector<PointLight*> Lights;

#endif // CSE168_POINTLIGHT_H_INCLUDED
