#ifndef CSE168_CAMERA_H_INCLUDED
#define CSE168_CAMERA_H_INCLUDED

#include "Vector3.h"
#include "Miro.h"
#include "Ray.h"

class Camera
{
public:
    Camera();
    virtual ~Camera();

    enum
    {
        RENDER_OPENGL   = 0,
        RENDER_RAYTRACE = 1
    };

    void click(Scene* pScene, Image* pImage);

    inline bool isOpenGL() const {return m_renderer == RENDER_OPENGL;}
    inline void setRenderer(int i) {m_renderer = i;}

    inline void setEye(float x, float y, float z);
    inline void setEye(const Vector3& eye);
    inline void setUp(float x, float y, float z);
    inline void setUp(const Vector3& up);
    inline void setViewDir(float x, float y, float z);
    inline void setViewDir(const Vector3& vd);
    inline void setLookAt(float x, float y, float z);
    inline void setLookAt(const Vector3& look);
    inline void setFOV(float fov) {m_fov = fov;}

    inline float fov() const                {return m_fov;}
    inline const Vector3 & viewDir() const  {return m_viewDir;}
    inline const Vector3 & lookAt() const   {return m_lookAt;}
    inline const Vector3 & up() const       {return m_up;}
    inline const Vector3 & eye() const      {return m_eye;}

    Ray eyeRay(float x, float y, int imageWidth, int imageHeight) const;
    
    void drawGL();

private:

    void calcLookAt();

    int m_renderer;

    // main screen params
    Vector3 m_eye;
    Vector3 m_up;
    Vector3 m_viewDir;
    Vector3 m_lookAt;
    float m_fov;
};

extern Camera * g_camera;

//--------------------------------------------------------

inline void Camera::setEye(float x, float y, float z)
{
    Vector3 v(x, y, z);
    setEye(v);
}

inline void Camera::setEye(const Vector3& eye)
{
    m_eye.set(eye);
}

inline void Camera::setUp(float x, float y, float z)
{
    Vector3 v(x, y, z);
    setUp(v);
}

inline void Camera::setUp(const Vector3& up)
{
    m_up.set(up);
    m_up.normalize();
}

inline void Camera::setViewDir(float x, float y, float z)
{
    Vector3 v(x, y, z);
    setViewDir(v);
}

inline void Camera::setViewDir(const Vector3& vd)
{
    m_viewDir.set(vd);
    m_viewDir.normalize();
}

inline void Camera::setLookAt(float x, float y, float z)
{
    Vector3 v(x, y, z);
    setLookAt(v);
}

inline void Camera::setLookAt(const Vector3& vd)
{
    Vector3 dir = vd - m_eye;
    setViewDir(dir);
}

#endif // CSE168_CAMERA_H_INCLUDED
