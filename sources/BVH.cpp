#include "BVH.h"
#include "Ray.h"
#include "Console.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Box.h"
#include "BoundingBox.h"
#include <limits>
#include <iterator>
#include <boost/thread.hpp>

#define INF std::numeric_limits<float>::infinity()

#define CBOX 4.f
#define CTRI 1.f
#define SIZE_LEAF 8

static float costNTris(int n)
{
  return (n / 4 + (n % 4 != 0)) * CTRI;
}

static float cost(float areaA, int objA, float areaB, int objB, float areaCinv)
{
    return 2 * CBOX + (areaA * costNTris(objA) + areaB * costNTris(objB)) * areaCinv;
}

void BVH::splitBox(AxisData const &data, BoundingBox &abox, BoundingBox &bbox, ObjectsWithBoxes::iterator begin, ObjectsWithBoxes::iterator end, float areaCinv, boost::function<void (int, ObjectsWithBoxes::iterator &, ObjectsWithBoxes::iterator &)> f)
{
    Vector3 v(INF);
    abox.setA(v);
    bbox.setA(v);
    v.set(-INF);
    abox.setB(v);
    bbox.setB(v);
    int a = 0, b = 0;
    for (ObjectsWithBoxes::iterator it = begin; it != end;)
    {
        Vector3 const &min = it->min;
        Vector3 const &max = it->max;
        if (min[data.axis] > data.pos || (max[data.axis] > data.pos && max[data.axis] - data.pos > data.pos - min[data.axis]))
        {
            bbox.setA(bbox.getA().min(min));
            bbox.setB(bbox.getB().max(max));
            f(1, it, end);
            b++;
            continue;
        }
        else
        {
            abox.setA(abox.getA().min(min));
            abox.setB(abox.getB().max(max));
            f(0, it, end);
            a++;
            continue;
        }
    }
}

void BVH::buildLeaf(ObjectsWithBoxes::iterator begin, ObjectsWithBoxes::iterator end, BVH::BBoxNode *prev_node)
{
    prev_node->objs = new std::vector<IntersectObjects>();
    prev_node->objs->resize(NB_OBJS);
    for (ObjectsWithBoxes::iterator it = begin; it != end; ++it)
    {
        (*prev_node->objs)[it->obj->type()].plain.push_back(it->obj->ptr());
    }
    (*prev_node->objs)[TRIANGLE].sse_preprocessed = Triangle::preProcess((*prev_node->objs)[TRIANGLE].plain);
}

void BVH::recBuildBBox(ObjectsWithBoxes::iterator begin, ObjectsWithBoxes::iterator end, BVH::BBoxNode *prev_node, int depth)
{
    if (end - begin <= SIZE_LEAF)
    {
        buildLeaf(begin, end, prev_node);
        return;
    }
    BoundingBox abox;
    BoundingBox bbox;
    float areaCinv = 1.f / prev_node->box.area();
    AxisData minsplit;
    minsplit.axis = -1;
    minsplit.cost = INF;
    for (int axis = 0; axis < 3; ++axis)
    {
        AxisData data;
        data.lowest = prev_node->box.getA()[axis];
        data.highest = prev_node->box.getB()[axis];
        if (data.lowest > data.highest)
            std::swap(data.lowest, data.highest);
        data.interval = (data.highest - data.lowest) / 10;
        if (data.interval < EPSILON)
            continue;
        data.axis = axis;
        for (data.pos = data.lowest + data.interval; data.pos < data.highest; data.pos += data.interval)
        {
            int cnts[2] = {0, 0};
            splitBox(data, abox, bbox, begin, end, areaCinv, [&](int n, ObjectsWithBoxes::iterator &it, ObjectsWithBoxes::iterator &)
            {
                cnts[n]++;
                ++it;
            });
            data.cost = cost(abox.area(), cnts[0], bbox.area(), cnts[1], areaCinv);
            minsplit = std::min(minsplit, data);
        }
    }
    if (minsplit.axis == -1)
    {
        buildLeaf(begin, end, prev_node);
        return;
    }
    ObjectsWithBoxes::iterator middle = begin, last = begin + (end - begin - 1);
    splitBox(minsplit, abox, bbox, begin, end, areaCinv, [&](int n, ObjectsWithBoxes::iterator &it, ObjectsWithBoxes::iterator &curend)
    {
        if (n == 0)
        {
            ++it;
            ++middle;
        }
        else
        {
            std::swap(*it, *last);
            --last;
            curend = last + 1;
        }
    });
    prev_node->a = new BBoxNode;
    prev_node->a->box = abox;
    prev_node->b = new BBoxNode;
    prev_node->b->box = bbox;
    if (1 << depth <= nCpus())
    {
        boost::thread t([&]() {recBuildBBox(begin, middle, prev_node->a, depth + 1);});
        recBuildBBox(middle, end, prev_node->b, depth + 1);
        t.join();
    }
    else
    {
        recBuildBBox(begin, middle, prev_node->a, depth + 1);
        recBuildBBox(middle, end, prev_node->b, depth + 1);
    }
}

BoundingBox BVH::objectBox(ObjectsWithBoxes *objs)
{
    Vector3 min = Vector3(INF, INF, INF);
    Vector3 max = Vector3(-INF, -INF, -INF);
    for (ObjectsWithBoxes::const_iterator it = objs->begin(); it != objs->end(); ++it)
    {
        min = min.min(it->min);
        max = max.max(it->max);
    }
    return BoundingBox(min, max);
}

void BVH::printHierarchy(BBoxNode *node, int ind)
{
    printf("%f\n", node->box.area());
    if (node->a && node->b)
    {
        printHierarchy(node->a, ind + 1);
        printHierarchy(node->b, ind + 1);
    }
    else
        printf("%f\n", (*node->objs)[TRIANGLE].plain.size());
}

void
BVH::build(Objects * objs)
{
    // construct the bounding volume hierarchy

    ObjectsWithBoxes objs_boxes;
    std::transform(objs->begin(), objs->end(), std::back_inserter(objs_boxes), [](Object *o) -> ObjectWithBox
    {
        ObjectWithBox res;
        res.min = o->minVector();
        res.max = o->maxVector();
        res.obj = o;
        return res;
    });

    m_root = new BBoxNode;
    m_root->box = objectBox(&objs_boxes);
    recBuildBBox(objs_boxes.begin(), objs_boxes.end(), m_root);
    //printHierarchy(m_root, 0);

    m_objects = objs;

    // OBSOLETE: start
    //m_categories_objects.resize(NB_OBJS);
    //for (size_t i = 0; i < m_objects->size(); ++i)
    //{
    //    m_categories_objects[(*m_objects)[i]->type()].plain.push_back((*m_objects)[i]->ptr());
    //}
    //m_categories_objects[TRIANGLE].sse_preprocessed = Triangle::preProcess(m_categories_objects[TRIANGLE].plain);
    // OBSOLETE: stop

    m_intersect_fcts.resize(NB_OBJS);
    m_intersect_fcts[TRIANGLE] = &Triangle::doIntersect;
    m_intersect_fcts[SPHERE] = &Sphere::doIntersect;
    m_intersect_fcts[BOX] = &Box::doIntersect;
}

bool
BVH::rec_intersect(BBoxNode *node, HitInfo& minHit, const Ray& ray, float tMin, float tMax) const
{
    bool hit = false;
    HitInfo tempMinHit;
    minHit.t = MIRO_TMAX;

    if (node->a == NULL && node->b == NULL)
    {
        for (size_t i = 0; i < NB_OBJS; ++i)
        {
            if (m_intersect_fcts[i] && m_intersect_fcts[i]((*node->objs)[i], tempMinHit, ray, tMin, tMax) && tempMinHit.t < minHit.t)
            {
                minHit = tempMinHit;
                hit = true;
            }
        }

    }
    else
    {
        if (node->a && node->a->box.doIntersect(tempMinHit, ray, tMin, tMax))
        {
            hit = rec_intersect(node->a, minHit, ray, tMin, tMax);
        }
        if (node->b && node->b->box.doIntersect(tempMinHit, ray, tMin, tMax))
        {
            bool tmpHit = rec_intersect(node->b, tempMinHit, ray, tMin, tMax);
            if (tmpHit && (!hit || (hit && tempMinHit.t < minHit.t)))
            {
                minHit = tempMinHit;
                hit = true;
            }
        }
    }
    if (hit)
    {
        if (minHit.N.dot(ray.d) > 0)
        {
            minHit.N.negate();
        }
    }
    return hit;
}

bool
BVH::intersect(HitInfo& minHit, const Ray& ray, float tMin, float tMax) const
{
    // Here you would need to traverse the BVH to perform ray-intersection
    // acceleration. For now we just intersect every object.

    bool hit = false;
    HitInfo tempMinHit;
    minHit.t = MIRO_TMAX;

    return rec_intersect(m_root, minHit, ray, tMin, tMax);
}
