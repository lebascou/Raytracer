#pragma once
#include "Object.h"
#include "BoundingBox.h"

class Box : public Object, public BoundingBox
{
public:
	Box(const Vector3& a, const Vector3& b);
	virtual ~Box();

	void renderGL();
	static bool doIntersect(IntersectObjects const &objects, HitInfo &, const Ray &, float, float);

	virtual ObjType type() const { return BOX; }
	
	virtual void *ptr() { return this; }

	virtual Vector3 minVector() const;
	virtual Vector3 maxVector() const;
};

