#pragma once
#include "Object.h"

class BoundingBox
{
public:
	BoundingBox(const Vector3& a, const Vector3& b);
	BoundingBox() {}
	virtual ~BoundingBox();

	bool doIntersect(HitInfo &, const Ray &, float, float);

	void setA(const Vector3 & v) {_a = v;}
	const Vector3& getA() const { return _a; }
	void setB(const Vector3 & v) {_b = v;}
	const Vector3& getB() const { return _b; }

    float area() const;

protected:
    Vector3 _a;
	Vector3 _b;
};

