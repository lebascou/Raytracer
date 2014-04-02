#include "BoundingBox.h"
#include "Ray.h"
#include <algorithm>

BoundingBox::BoundingBox(const Vector3& a, const Vector3& b) : _a(a), _b(b)
{
}


BoundingBox::~BoundingBox(void)
{
}

bool BoundingBox::doIntersect(HitInfo &result, const Ray &ray, float tMin, float tMax)
{
	Vector3 t1 = (_a - ray.o) / ray.d;
	Vector3 t2 = (_b - ray.o) / ray.d;
	
	float t_min = std::max(std::min(t1.x, t2.x), std::max(std::min(t1.y, t2.y), std::min(t1.z, t2.z)));
	float t_max = std::min(std::max(t1.x, t2.x), std::min(std::max(t1.y, t2.y), std::max(t1.z, t2.z)));
	
	if (t_max < 0 || t_min > t_max)
		return false;
	result.t = t_min < 0 ? t_max : t_min;
	return true;
}

float BoundingBox::area() const
{
	Vector3 segments = (_a - _b).abs();
	return (segments.x * segments.y + segments.x * segments.z + segments.y * segments.z) * 2;
}
