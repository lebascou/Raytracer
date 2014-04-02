#include "Box.h"
#include "Ray.h"

Box::Box(const Vector3& a, const Vector3& b) : BoundingBox(a, b)
{
}


Box::~Box(void)
{
}

void Box::renderGL()
{
    Vector3 v[16];

	v[0] = Vector3(_a.x, _a.y, _a.z);
	v[1] = Vector3(_a.x + _b.x, _a.y, _a.z);
	v[2] = Vector3(_a.x + _b.x, _a.y, _a.z + _b.z);
	v[3] = Vector3(_a.x, _a.y, _a.z + _b.z);

	v[4] = Vector3(_a.x, _a.y, _a.z + _b.z);	
	v[5] = Vector3(_a.x, _a.y + _b.y, _a.z + _b.z);
	v[6] = Vector3(_a.x, _a.y + _b.y, _a.z);	
	v[7] = Vector3(_a.x, _a.y, _a.z);

	v[8] = Vector3(_a.x, _a.y + _b.y, _a.z);
	v[9] = Vector3(_a.x + _b.x, _a.y + _b.y, _a.z);
	v[10] = Vector3(_a.x + _b.x, _a.y + _b.y, _a.z + _b.z);
	v[11] = Vector3(_a.x, _a.y + _b.y, _a.z + _b.z);

	v[12] = Vector3(_a.x + _b.x, _a.y, _a.z + _b.z);	
	v[13] = Vector3(_a.x + _b.x, _a.y + _b.y, _a.z + _b.z);
	v[14] = Vector3(_a.x + _b.x, _a.y + _b.y, _a.z);	
	v[15] = Vector3(_a.x + _b.x, _a.y, _a.z);

    glBegin(GL_QUADS);
	for (int i = 0; i < 16; i++)
        glVertex3f(v[i].x, v[i].y, v[i].z);
    glEnd();
}

bool Box::doIntersect(IntersectObjects const &objects, HitInfo& result, const Ray& ray, float tMin, float tMax)
{
	bool hit = false;
	int idx = 0;

	for (int i = 0; i < objects.plain.size(); i++)
	{
		Box *cur = (Box*) objects.plain[i];
		HitInfo cur_result;

		if (((BoundingBox*)cur)->doIntersect(cur_result, ray, tMin, tMax) && cur_result.t > tMin && cur_result.t < tMax)
		{
			result = cur_result;
			result.material = cur->m_material;
			hit = true;
			idx = i;
		}		
	}
	if (hit == true)
	{
		result.P = ray.o + (ray.d * result.t);
		Vector3 p = (((Box*)objects.plain[idx])->getA() + ((Box*)objects.plain[idx])->getB()) / 2.f;
		p = result.P - p;

		if (std::abs(p.x) >= std::abs(p.y) && std::abs(p.x) >= std::abs(p.z)) 
		{
			p.y = 0;
			p.z = 0;
		}
		else if (std::abs(p.y) >= std::abs(p.x) && std::abs(p.y) >= std::abs(p.z))
		{
			p.x = 0;
			p.z = 0;
		}
		else
		{
			p.x = 0;
			p.y = 0;
		}
		p.normalize();
		result.N = p;
	}
	return hit;
}

Vector3 Box::maxVector() const
{
	return _a.max(_b);
}

Vector3 Box::minVector() const
{
	return _a.min(_b);
}
