#ifndef CSE168_TRIANGLE_MESH_H_INCLUDED
#define CSE168_TRIANGLE_MESH_H_INCLUDED

#include <stdlib.h>
#include <map>
#include "Matrix4x4.h"
#include "Material.h"

class TriangleMesh
{
public:
    TriangleMesh();
    ~TriangleMesh();

    // load from an OBJ file
    bool load(char* file, const Matrix4x4& ctm = Matrix4x4());

    // for single triangles
    void createSingleTriangle();
    inline void setV1(const Vector3& v) {m_vertices[0] = v;}
    inline void setV2(const Vector3& v) {m_vertices[1] = v;}
    inline void setV3(const Vector3& v) {m_vertices[2] = v;}
    inline void setN1(const Vector3& n) {m_normals[0] = n;}
    inline void setN2(const Vector3& n) {m_normals[1] = n;}
    inline void setN3(const Vector3& n) {m_normals[2] = n;}

    struct TupleI3
    {
        unsigned int x, y, z;
    };

    struct VectorR2
    {
        float x, y;

        VectorR2() {}
        VectorR2(float x, float y) : x(x), y(y) {}

        VectorR2 operator+(VectorR2 const &v) const {return VectorR2(x + v.x, y + v.y);}
        VectorR2 operator-(VectorR2 const &v) const {return VectorR2(x - v.x, y - v.y);}
        VectorR2 operator*(float f) const {return VectorR2(x * f, y * f);}
    };

    Vector3* vertices()     {return m_vertices;}
    Vector3* normals()      {return m_normals;}
    VectorR2* uvs()         {return m_texCoords;}
    Vector3* tangents()     {return m_tangents;}
    TupleI3* tIndices()     {return m_vertexIndices;}
    TupleI3* vIndices()     {return m_vertexIndices;}
    TupleI3* nIndices()     {return m_normalIndices;}
    TupleI3* uvIndices()    {return m_texCoordIndices;}
    int numTris()           {return m_numTris;}
	Material* getMaterialForId(int triangle_id);

protected:
    void loadObj(FILE* fp, const Matrix4x4& ctm);
	void loadMtl(std::string file_name);

	std::map<std::string, Material*> m_mtls;
	std::map<std::string, std::vector<unsigned int> > m_mtl_ids;

    Vector3* m_normals;
    Vector3* m_vertices;
    VectorR2* m_texCoords;
    Vector3* m_tangents;

    TupleI3* m_normalIndices;
    TupleI3* m_vertexIndices;
    TupleI3* m_texCoordIndices;
    unsigned int m_numTris;
};


#endif // CSE168_TRIANGLE_MESH_H_INCLUDED
