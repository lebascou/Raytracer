#ifndef CSE168_OBJECT_H_INCLUDED
#define CSE168_OBJECT_H_INCLUDED

#include <vector>
#include "Miro.h"
#include "Material.h"
#include "IntersectObjects.h"

enum ObjType
{
    SPHERE,
    TRIANGLE,
    BLPATCH,
	BOUNDING_BOX,
	BOX,
    NB_OBJS
};

class ObjBundle;

class Object
{
public:
    Object() {}
    virtual ~Object() {}

    void setMaterial(const Material* m) {m_material = m;}

    virtual void renderGL() {}
    virtual void preCalc() {}

    // Needs a static function that intersects a bunch of objects of the same type

    virtual ObjType type() const = 0;

    virtual void *ptr() = 0;

    virtual Vector3 minVector() const = 0;
    virtual Vector3 maxVector() const = 0;

protected:
    const Material* m_material;
};

typedef std::vector<Object*> Objects;

#endif // CSE168_OBJECT_H_INCLUDED
