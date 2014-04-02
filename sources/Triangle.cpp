#include "Triangle.h"
#include "TriangleMesh.h"
#include "Ray.h"

namespace
{
    struct FourVectors
    {
        __m128 x;
        __m128 y;
        __m128 z;

        FourVectors(const Vector3 &v): x(_mm_set_ps1(v.x)), y(_mm_set_ps1(v.y)), z(_mm_set_ps1(v.z)) {}
        FourVectors() {}

        FourVectors &operator-=(FourVectors const &v)
        {
            x = _mm_sub_ps(x, v.x);
            y = _mm_sub_ps(y, v.y);
            z = _mm_sub_ps(z, v.z);
            return *this;
        }
        FourVectors operator-(FourVectors const &v) const
        {
            FourVectors cpy(*this);
            cpy -= v;
            return cpy;
        }
        FourVectors cross(const FourVectors &v) const
        {
            FourVectors res;
            res.y = _mm_sub_ps(_mm_mul_ps(z, v.x), _mm_mul_ps(x, v.z));
            res.z = _mm_sub_ps(_mm_mul_ps(x, v.y), _mm_mul_ps(y, v.x));
            res.x = _mm_sub_ps(_mm_mul_ps(y, v.z), _mm_mul_ps(z, v.y));
            return res;
        }
        __m128 dot(const FourVectors &v) const
        {
            return _mm_add_ps(_mm_add_ps(_mm_mul_ps(x, v.x), _mm_mul_ps(y, v.y)), _mm_mul_ps(z, v.z));
        }
    };
    struct SSETriangle : SSEObject
    {
        FourVectors a;
        FourVectors ab;
        FourVectors ac;
    };
    struct Triangleref
    {
        Triangle *tri[4];
    };
    struct SSETriangles
    {
        SSETriangle *triangles;
        Triangleref *refs;
        int num_triangles;

        SSETriangles(SSETriangle *t, Triangleref *r, int n): triangles(t), refs(r), num_triangles(n) {}
    };
    struct TmpTriangle
    {
        Vector3 points[3];
    };
    void loadTriangle(FourVectors &tri, TmpTriangle *tmp, int idx)
    {
        tri.x = _mm_set_ps(tmp[3].points[idx].x, tmp[2].points[idx].x,
                            tmp[1].points[idx].x, tmp[0].points[idx].x);
        tri.y = _mm_set_ps(tmp[3].points[idx].y, tmp[2].points[idx].y,
                            tmp[1].points[idx].y, tmp[0].points[idx].y);
        tri.z = _mm_set_ps(tmp[3].points[idx].z, tmp[2].points[idx].z,
                            tmp[1].points[idx].z, tmp[0].points[idx].z);
    }
}

Triangle::Triangle(TriangleMesh * m, unsigned int i) :
    m_mesh(m), m_index(i)
{

}


Triangle::~Triangle()
{

}

void
Triangle::renderGL()
{
    TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
    const Vector3 & v0 = m_mesh->vertices()[ti3.x]; //vertex a of triangle
    const Vector3 & v1 = m_mesh->vertices()[ti3.y]; //vertex b of triangle
    const Vector3 & v2 = m_mesh->vertices()[ti3.z]; //vertex c of triangle

    glBegin(GL_TRIANGLES);
        glVertex3f(v0.x, v0.y, v0.z);
        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
    glEnd();
}

bool Triangle::doIntersect(IntersectObjects const &objects, HitInfo& result,
                           const Ray& ray, float tMin, float tMax)
{
    int m1 = -1;
    bool hit = false;
    Vector3 md = -ray.d;
    if (objects.sse_preprocessed)
    {
        SSETriangles const *triangles = (SSETriangles const*)objects.sse_preprocessed;
        FourVectors d4(md);
        FourVectors o4(ray.o);
        __m128 zero = _mm_set_ps1(-0.0001);
        __m128 one = _mm_set_ps1(1.0001f);
        __m128 minT = _mm_set_ps1(tMax);
        __m128 vtMin = _mm_set_ps1(tMin);
        __m128 minI = _mm_set_ps1(*(float*)&m1);

        for (int i = 0; i < triangles->num_triangles; ++i)
        {
            SSETriangle const &tri = triangles->triangles[i];
            FourVectors const &ab = tri.ab;
            FourVectors const &ac = tri.ac;
            FourVectors o_a = o4 - tri.a;
            FourVectors normal = ab.cross(ac);
            // Note: I used to use _mm_rcp_ps here and this caused a bug that took me hours to track because _mm_rcp_ps is approximate.
            __m128 detA = _mm_div_ps(_mm_set_ps1(1.f), d4.dot(normal));
            __m128 t = _mm_mul_ps(o_a.dot(normal), detA);
            __m128 alpha = _mm_mul_ps(d4.dot(o_a.cross(ac)), detA);
            __m128 beta = _mm_mul_ps(d4.dot(ab.cross(o_a)), detA);
            __m128 cnd = _mm_and_ps(_mm_cmpge_ps(t, vtMin), _mm_and_ps(_mm_cmple_ps(_mm_add_ps(beta, alpha), one), _mm_and_ps(_mm_cmpge_ps(alpha, zero), _mm_cmpge_ps(beta, zero))));
            // At this stage, all the cnd contains 0xffffffff for all indexes where the point is valid, and 0 elsewhere
            t = _mm_blendv_ps(minT, t, cnd);
            __m128 t2 = _mm_min_ps(t, _mm_shuffle_ps(t, t, _MM_SHUFFLE(1, 0, 3, 2)));
            __m128 curI = _mm_set_ps1(*(float*)&i);
            t2 = _mm_min_ps(t2, _mm_shuffle_ps(t2, t2, _MM_SHUFFLE(2, 3, 0, 1)));
            // At this stage, t2 contains only the minimum t so far, including the current triangle
            __m128 is_lowest = _mm_cmplt_ps(t2, minT); // Whether this time was when we found a lower t
            minT = t2;
            minI = _mm_blendv_ps(minI, curI, is_lowest);
        }
        int min_index = _mm_extract_int(minI, 0);
        if (min_index >= 0)
        {
            SSETriangle const &tri = triangles->triangles[min_index];
            FourVectors const &ab = tri.ab;
            FourVectors const &ac = tri.ac;
            FourVectors o_a = o4 - tri.a;
            FourVectors normal = ab.cross(ac);
            __m128 detA = _mm_div_ps(_mm_set_ps1(1.f), d4.dot(normal));
            __m128 t = _mm_mul_ps(o_a.dot(normal), detA);
            __m128 alpha = _mm_mul_ps(d4.dot(o_a.cross(ac)), detA);
            __m128 beta = _mm_mul_ps(d4.dot(ab.cross(o_a)), detA);
            __m128 cnd = _mm_and_ps(_mm_cmpge_ps(t, vtMin), _mm_and_ps(_mm_cmple_ps(_mm_add_ps(beta, alpha), one), _mm_and_ps(_mm_cmpge_ps(alpha, zero), _mm_cmpge_ps(beta, zero))));
            t = _mm_blendv_ps(_mm_set_ps1(tMax), t, cnd);
            int min = 0;
            for (int i = 1; i < 4; ++i)
                if (_mm_extract_ps1(t, i) < _mm_extract_ps1(t, min) && _mm_extract_ps1(t, i) > tMin)
                    min = i;
            result.t = _mm_extract_ps1(t, min);
            result.P = ray.o + result.t * ray.d;
            Triangle *tr = triangles->refs[min_index].tri[min];
            Vector3 *ns = tr->m_mesh->normals();
            TriangleMesh::TupleI3 *ni = tr->m_mesh->nIndices();
            Vector3 const &na = ns[ni[tr->m_index].x];
            Vector3 const &nb = ns[ni[tr->m_index].y];
            Vector3 const &nc = ns[ni[tr->m_index].z];
            float alphaf = _mm_extract_ps1(alpha, min), betaf = _mm_extract_ps1(beta, min);
            result.N = (1.0f - alphaf - betaf) * na + alphaf * nb + betaf * nc;
            //result.N = Vector3(_mm_extract_ps1(normal.x, min), _mm_extract_ps1(normal.y, min), _mm_extract_ps1(normal.z, min));
			result.N.normalize();
            TriangleMesh::VectorR2 *uvs = tr->m_mesh->uvs();
            if (uvs)
            {
                TriangleMesh::TupleI3 *uvi = tr->m_mesh->uvIndices();
                TriangleMesh::VectorR2 const &uva = uvs[uvi[tr->m_index].x];
                TriangleMesh::VectorR2 const &uvb = uvs[uvi[tr->m_index].y];
                TriangleMesh::VectorR2 const &uvc = uvs[uvi[tr->m_index].z];
                TriangleMesh::VectorR2 uv = uva * (1 - alphaf - betaf) + uvb * alphaf + uvc * betaf;
                result.u = uv.x;
                result.v = uv.y;
                if (tr->m_material->hasBump())
                {
                    TriangleMesh::TupleI3 *ti = tr->m_mesh->tIndices();
                    Vector3 *ts = tr->m_mesh->tangents();
                    Vector3 const &ta = ts[ti[tr->m_index].x];
                    Vector3 const &tb = ts[ti[tr->m_index].y];
                    Vector3 const &tc = ts[ti[tr->m_index].z];
                    result.tangent = (1 - alphaf - betaf) * ta + alphaf * tb + betaf * tc;
                }
            }
            result.material = tr->m_material;
            tMax = result.t;
            hit = true;
        }
    }
    for (int i = 0; i < objects.plain.size(); ++i)
    {
        Triangle *tri = (Triangle *)objects.plain[i];
        Vector3 *vs = tri->m_mesh->vertices();
        Vector3 *ns = tri->m_mesh->normals();
        TriangleMesh::TupleI3 *vi = tri->m_mesh->vIndices();
        TriangleMesh::TupleI3 *ni = tri->m_mesh->nIndices();

        Vector3 ab = vs[vi[tri->m_index].y] - vs[vi[tri->m_index].x];
        Vector3 ac = vs[vi[tri->m_index].z] - vs[vi[tri->m_index].x];
        Vector3 normal = ab.cross(ac);
        Vector3 o_a = ray.o - vs[vi[tri->m_index].x];
        float detA = 1 / md.dot(normal);
        float t = o_a.dot(normal) * detA;
        float alpha = md.dot(o_a.cross(ac)) * detA;
        float beta = md.dot(ab.cross(o_a)) * detA;
        if ((alpha >= -0.0001 && beta >= -0.0001 && alpha + beta <= 1.0001 && t > tMin && t < tMax))
        {
            result.t = t;
            result.P = ray.o + result.t*ray.d;
            Vector3 na = ns[ni[tri->m_index].x];
            Vector3 nb = ns[ni[tri->m_index].y];
            Vector3 nc = ns[ni[tri->m_index].z];
            result.N = (1 - alpha - beta) * na + alpha * nb + beta * nc;
            TriangleMesh::VectorR2 *uvs = tri->m_mesh->uvs();
            if (uvs)
            {
                TriangleMesh::TupleI3 *uvi = tri->m_mesh->uvIndices();
                TriangleMesh::VectorR2 uva = uvs[uvi[tri->m_index].x];
                TriangleMesh::VectorR2 uvb = uvs[uvi[tri->m_index].y];
                TriangleMesh::VectorR2 uvc = uvs[uvi[tri->m_index].z];
                TriangleMesh::VectorR2 uv = uva * (1 - alpha - beta) + uvb * alpha + uvc * beta;
                result.u = uv.x;
                result.v = uv.y;
                if (tri->m_material->hasBump())
                {
                    TriangleMesh::TupleI3 *ti = tri->m_mesh->tIndices();
                    Vector3 *ts = tri->m_mesh->tangents();
                    Vector3 const &ta = ts[ti[tri->m_index].x];
                    Vector3 const &tb = ts[ti[tri->m_index].y];
                    Vector3 const &tc = ts[ti[tri->m_index].z];
                    result.tangent = (1 - alpha - beta) * ta + alpha * tb + beta * tc;
                }
            }
            // result.N = normal;
            result.N.normalize();
            result.material = tri->m_material;
            tMax = result.t;
            hit = true;
        }
    }
    return hit;
}

Vector3 Triangle::minVector() const
{
    Vector3 *vs = m_mesh->vertices();
    TriangleMesh::TupleI3 *vi = m_mesh->vIndices();
    Vector3 const &a = vs[vi[m_index].x];
    Vector3 const &b = vs[vi[m_index].y];
    Vector3 const &c = vs[vi[m_index].z];
    return a.min(b).min(c);
}

Vector3 Triangle::maxVector() const
{
    Vector3 *vs = m_mesh->vertices();
    TriangleMesh::TupleI3 *vi = m_mesh->vIndices();
    Vector3 const &a = vs[vi[m_index].x];
    Vector3 const &b = vs[vi[m_index].y];
    Vector3 const &c = vs[vi[m_index].z];
    return a.max(b).max(c);
}

void * Triangle::preProcess( std::vector<void *> &objects )
{
    // Remove triangles until all the remaining ones have a multiple of 4
    // real triangles. There is a more optimal way to write this but this is
    // simple, fast, and works well in practice.
    if (!sse_info.hasSSE())
        return 0;
    size_t sse_size = objects.size() / 4 + (objects.size() % 4 != 0);
    SSETriangle *tris = new SSETriangle[sse_size];
    Triangleref *refs = new Triangleref[sse_size];
    TmpTriangle triangles[4];
    int cur_in_four = 0;
    int cur = 0;
    for (int i = 0; i < objects.size() + (objects.size() % 4 ? 4 - objects.size() % 4 : 0); ++i)
    {
        if (i < objects.size())
        {
            Triangle *tri = (Triangle *)objects[i];
            Vector3 *vs = tri->m_mesh->vertices();
            TriangleMesh::TupleI3 *vi = tri->m_mesh->vIndices();

            triangles[cur_in_four].points[0] = vs[vi[tri->m_index].x];
            triangles[cur_in_four].points[1] = vs[vi[tri->m_index].y] - vs[vi[tri->m_index].x];
            triangles[cur_in_four].points[2] = vs[vi[tri->m_index].z] - vs[vi[tri->m_index].x];
            refs[cur].tri[cur_in_four] = tri;
        }
        else
        {
            triangles[cur_in_four].points[0] = Vector3(-INF);
            triangles[cur_in_four].points[1] = Vector3(-INF);
            triangles[cur_in_four].points[2] = Vector3(-INF);
            refs[cur].tri[cur_in_four] = 0;
        }

        cur_in_four++;
        if (cur_in_four >= 4)
        {
            loadTriangle(tris[cur].a, triangles, 0);
            loadTriangle(tris[cur].ab, triangles, 1);
            loadTriangle(tris[cur].ac, triangles, 2);
            cur_in_four = 0;
            ++cur;
        }
    }

    objects.clear();
    return new SSETriangles(tris, refs, sse_size);
}
