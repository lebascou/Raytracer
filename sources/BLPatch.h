#ifndef CSE168_BL_PATCH_H_INCLUDED
#define CSE168_BL_PATCH_H_INCLUDED

#include "Vector3.h"
#include "Object.h"

class BLPatch : public Object
{
public:
    BLPatch();
    virtual ~BLPatch();

    Vector3 & vertex(int i)             {return m_verts[i];}
    const Vector3 & vertex(int i) const {return m_verts[i];}

    virtual void renderGL();

    virtual ObjType type() const;

    virtual void *selfPtr() { return this; }
protected:
    Vector3 m_verts[4];
};

#endif // CSE168_BL_PATCH_H_INCLUDED
