#ifndef CSE168_BVH_H_INCLUDED
#define CSE168_BVH_H_INCLUDED

#include "Miro.h"
#include "Object.h"
#include "BoundingBox.h"
#include "IntersectObjects.h"
#include <boost/function.hpp>

class BVH
{
public:
    void build(Objects * objs);

    bool intersect(HitInfo& result, const Ray& ray,
                   float tMin = 0.0f, float tMax = MIRO_TMAX) const;

protected:
    struct BBoxNode
    {
    	BoundingBox box;
    	std::vector<IntersectObjects> *objs;
    	BBoxNode *a;
    	BBoxNode *b;

    	BBoxNode() : a(0), b(0), objs(0) {}

    	~BBoxNode() { delete a; delete b; delete objs; }
    };

    struct AxisData
    {
        float lowest;
        float highest;
        float interval;
        float pos;
        int axis;
        float cost;

        bool operator<(AxisData const &other) const
        {
            return cost < other.cost;
        }
    };

    struct ObjectWithBox
    {
        Object *obj;
        Vector3 min, max;
    };
    typedef std::vector<ObjectWithBox> ObjectsWithBoxes;

    void splitBox(AxisData const &data, BoundingBox &abox, BoundingBox &bbox, ObjectsWithBoxes::iterator begin, ObjectsWithBoxes::iterator end, float areaCinv, boost::function<void (int, ObjectsWithBoxes::iterator &, ObjectsWithBoxes::iterator &)> f);
    void buildLeaf(ObjectsWithBoxes::iterator begin, ObjectsWithBoxes::iterator end, BVH::BBoxNode *prev_node);

	void recBuildBBox(ObjectsWithBoxes::iterator begin, ObjectsWithBoxes::iterator end, BVH::BBoxNode *prev_node, int depth = 0);
	BoundingBox objectBox(ObjectsWithBoxes *objs);
	void printHierarchy(BBoxNode *node, int ind);

    Objects * m_objects;
    std::vector<IntersectObjects> m_categories_objects;
    typedef bool (*Intersect)(IntersectObjects const &objects, HitInfo& result,
                              const Ray& ray, float tMin, float tMax);
    std::vector<Intersect> m_intersect_fcts;

    BBoxNode *m_root;

public:
	bool rec_intersect(BBoxNode *node, HitInfo& result, const Ray& ray, 
		float tMin = 0.0f, float tMax = MIRO_TMAX) const;
};

#endif // CSE168_BVH_H_INCLUDED
