#include "TriangleMesh.h"
#include "Console.h"
#include <algorithm>
#include "Texture.h"
#include "Phong.h"

#ifdef WIN32
// disable useless warnings
#pragma warning(disable:4996)
#endif


void
TriangleMesh::createSingleTriangle()
{
    m_normals = new Vector3[3];
    m_vertices = new Vector3[3];
    m_texCoords = new VectorR2[3];

    m_texCoords[0].x = 0.0f;
    m_texCoords[0].y = 0.0f;
    m_texCoords[1].x = 1.0f;
    m_texCoords[1].y = 0.0f;
    m_texCoords[2].x = 0.0f;
    m_texCoords[2].y = 1.0f;

    m_normalIndices = new TupleI3[1];
    m_vertexIndices = new TupleI3[1];
    m_texCoordIndices = new TupleI3[1];

    m_vertexIndices[0].x = 0;
    m_vertexIndices[0].y = 1;
    m_vertexIndices[0].z = 2;

    m_normalIndices[0].x = 0;
    m_normalIndices[0].y = 1;
    m_normalIndices[0].z = 2;

    m_texCoordIndices[0].x = 0;
    m_texCoordIndices[0].y = 1;
    m_texCoordIndices[0].z = 2;

    m_numTris = 1;
}

//************************************************************************
// You probably don't want to modify the following functions
// They are for loading .obj files
//************************************************************************

bool
TriangleMesh::load(char* file, const Matrix4x4& ctm)
{
    FILE *fp = fopen(file, "rb");
    if (!fp)
    {
        error("Cannot open \"%s\" for reading\n",file);
        return false;
    }
    debug("Loading \"%s\"...\n", file);

    loadObj(fp, ctm);
    debug("Loaded \"%s\" with %d triangles\n",file,m_numTris);
    fclose(fp);

    return true;
}

void
getIndices(char *word, int *vindex, int *tindex, int *nindex)
{
    char *null = " ";
    char *ptr;
    char *tp;
    char *np;

    // by default, the texture and normal pointers are set to the null string
    tp = null;
    np = null;

    // replace slashes with null characters and cause tp and np to point
    // to character immediately following the first or second slash
    for (ptr = word; *ptr != '\0'; ptr++)
    {
        if (*ptr == '/')
        {
            if (tp == null)
                tp = ptr + 1;
            else
                np = ptr + 1;

            *ptr = '\0';
        }
    }

    *vindex = atoi (word);
    *tindex = atoi (tp);
    *nindex = atoi (np);
}

void
TriangleMesh::loadMtl(std::string file_name)
{
	file_name.erase(std::remove(file_name.begin(), file_name.end(), '\n'), file_name.end());
	file_name.erase(std::remove(file_name.begin(), file_name.end(), '\r'), file_name.end());
	FILE *fp = fopen(file_name.c_str(), "rb");
    if (!fp)
	{
		printf("Cannot open \"%s\" for reading\n", file_name.c_str());
		return;
	}
	debug("Loading MTL file '%s'\n", file_name.c_str());


	int line_number = 0;
	char *current_token;
	char current_line[500];
	std::string current_mtl_name;
	char material_open = 0;
	Phong *current_phong_mtl = NULL;

	while(fgets(current_line, 500, fp))
	{
		current_token = strtok( current_line, " \t\n\r");
		line_number++;
		
		if( current_token == NULL || strcmp(current_token, "//") == 0 || strcmp(current_token, "#") == 0) // comments
			continue;
		else if(!strcmp(current_token, "newmtl")) // new material
		{
			material_open = 1;
			current_mtl_name = strtok(NULL, " \t");
			current_mtl_name.erase(std::remove(current_mtl_name.begin(), current_mtl_name.end(), '\n'), current_mtl_name.end());
			current_mtl_name.erase(std::remove(current_mtl_name.begin(), current_mtl_name.end(), '\r'), current_mtl_name.end());

			current_phong_mtl = new Phong(0.8f, 1.0f, 0.2f);

			m_mtls[current_mtl_name] = current_phong_mtl;
		}
		else if(!strcmp(current_token, "Ka") && material_open) // Ka r g b => The Ka statement specifies the ambient reflectivity using RGB values (range: [0, 1])
		{
			current_phong_mtl->setKa(Vector3(atof(strtok(NULL, " \t")), atof(strtok(NULL, " \t")), atof(strtok(NULL, " \t"))));
		}
		else if(!strcmp(current_token, "Kd") && material_open) // Kd r g b => The Kd statement specifies the diffuse reflectivity using RGB values (range: [0, 1])
		{
			current_phong_mtl->setKd(Vector3(atof(strtok(NULL, " \t")), atof(strtok(NULL, " \t")), atof(strtok(NULL, " \t"))));
		}
		else if(!strcmp(current_token, "Ks") && material_open) // Ks r g b => The Ks statement specifies the specular reflectivity using RGB values (range: [0, 1])
		{
			current_phong_mtl->setKs(Vector3(1.0f - atof(strtok(NULL, " \t")), 1.0f - atof(strtok(NULL, " \t")), 1.0f - atof(strtok(NULL, " \t"))));
		}
		else if(!strcmp(current_token, "Ns") && material_open) // Ns exponent => Specifies the specular exponent for the current material.  This defines the focus of the specular highlight (range: [0, 1000])
		{
			current_phong_mtl->setPhong(atof(strtok(NULL, " \t")));
		}
		else if(!strcmp(current_token, "d") && material_open) // d factor => Specifies the dissolve for the current material. A  factor of 1.0 is fully opaque (range: [0, 1])
		{
			current_phong_mtl->setTransparency(1.0f - atof(strtok(NULL, " \t")));
		}
		else if(!strcmp(current_token, "r") && material_open) // r factor => Specifies the reflection for the current material. A  factor of 1.0 reflects all the light (range: [0, 1])
		{
			current_phong_mtl->setKs(atof(strtok(NULL, " \t")));
		}
		else if(!strcmp(current_token, "Ni") && material_open) // Ni optical_density => Specifies the optical density for the surface.  This is also known as index of refraction (range: [0.001 to 10])
		{
			current_phong_mtl->setRefraction(atof(strtok(NULL, " \t")));
		}
		else if(!strcmp(current_token, "map_Kd") && material_open) // map_Kd filename => Specifies that a color texture file is linked to the diffuse reflectivity of the material. The map_Kd value is multiplied by the Kd value.
		{
			Texture* kd_texture = new Texture();
			kd_texture->setTexturePath(strtok(NULL, " \t"));
			current_phong_mtl->setKdTexture(shared_ptr<Material>(kd_texture));
		}
		else if((!strcmp(current_token, "map_Ka") || !strcmp(current_token, "map_kA")) && material_open) // map_Ka filename => Specifies that a color texture file is applied to the ambient reflectivity of the material. The map_Ka value is multiplied by the Ka value.
		{
			Texture* ka_texture = new Texture();
			ka_texture->setTexturePath(strtok(NULL, " \t"));
			current_phong_mtl->setKaTexture(shared_ptr<Material>(ka_texture));
		}
		else if ((!strcmp(current_token, "map_Ks") || !strcmp(current_token, "map_kS")) && material_open) // map_Ks filename => Specifies that a color texture file is linked to the specular reflectivity of the material. The map_Ks value is multiplied by the Ks value
		{
			Texture* ks_texture = new Texture();
			ks_texture->setTexturePath(strtok(NULL, " \t"));
			current_phong_mtl->setKsTexture(shared_ptr<Material>(ks_texture));
		}
		else if ((!strcmp(current_token, "bump") || !strcmp(current_token, "map_bump")) && material_open) // bump filename => Specifies that a bump texture file is linked to the material
		{
			Texture* bump_texture = new Texture();

			current_token = strtok(NULL, " \t");
			if (!strcmp(current_token, "-bm"))
			{
				current_phong_mtl->setBm(atof(strtok(NULL, " \t")));
				bump_texture->setTexturePath(strtok(NULL, " \t"));
			}
			else
				bump_texture->setTexturePath(current_token);
			current_phong_mtl->setBumpTexture(shared_ptr<Texture>(bump_texture));
		}
	}
	fclose(fp);
}

void
TriangleMesh::loadObj(FILE* fp, const Matrix4x4& ctm)
{
    int nv=0, nt=0, nn=0, nf=0;
    char line[81];
    while (fgets(line, 80, fp) != 0)
    {
        if (line[0] == 'v')
        {
            if (line[1] == 'n')
                nn++;
            else if (line[1] == 't')
                nt++;
            else
                nv++;
        }
        else if (line[0] == 'f')
        {
            nf++;
        }
    }
    fseek(fp, 0, 0);

    m_normals = new Vector3[std::max(nv, std::max(nn, std::max(nf, nt))) * 2];
    m_vertices = new Vector3[nv * 2];

    if (nt)
    {   // got texture coordinates
        m_texCoords = new VectorR2[nt * 2];
        m_texCoordIndices = new TupleI3[nf * 2];
    }
    m_normalIndices = new TupleI3[std::max(nv, std::max(nn, std::max(nf, nt))) * 2]; // always make normals
    m_vertexIndices = new TupleI3[nf * 2]; // always have vertices

    m_numTris = 0;
    int nvertices = 0;
    int nnormals = 0;
    int ntextures = 0;

    Matrix4x4 nctm = ctm;
    nctm.invert();
    nctm.transpose();
    nctm.invert();

    while (fgets(line, 80, fp) != 0)
    {
        if (line[0] == 'v')
        {
            if (line[1] == 'n')
            {
                float x, y, z;
                sscanf(&line[2], "%f %f %f\n", &x, &y, &z);
                Vector3 n(x, y, z);
                m_normals[nnormals] = nctm*n;
                m_normals[nnormals].normalize();
                nnormals++;
            }
            else if (line[1] == 't')
            {
                float x, y;
                sscanf(&line[2], "%f %f\n", &x, &y);
                m_texCoords[ntextures].x = x;
                m_texCoords[ntextures].y = y;
                ntextures++;
            }
            else
            {
                float x, y, z;
                sscanf(&line[1], "%f %f %f\n", &x, &y, &z);
                Vector3 v(x, y, z);
                m_vertices[nvertices] = ctm*v;
                nvertices++;
            }
        }
        else if (line[0] == 'f')
        {
            char s1[32], s2[32], s3[32], s4[32];
            int v, t, n;
			s4[0] = 0;
            sscanf(&line[1], "%s %s %s %s\n", s1, s2, s3, s4);

            getIndices(s1, &v, &t, &n);
            m_vertexIndices[m_numTris].x = v-1;
            if (n)
                m_normalIndices[m_numTris].x = n-1;
            if (t)
                m_texCoordIndices[m_numTris].x = t-1;
            getIndices(s2, &v, &t, &n);
            m_vertexIndices[m_numTris].y = v-1;
            if (n)
                m_normalIndices[m_numTris].y = n-1;
            if (t)
                m_texCoordIndices[m_numTris].y = t-1;
            getIndices(s3, &v, &t, &n);
            m_vertexIndices[m_numTris].z = v-1;
            if (n)
                m_normalIndices[m_numTris].z = n-1;
            if (t)
                m_texCoordIndices[m_numTris].z = t-1;

            if (!n)
            {   // if no normal was supplied
                Vector3 e1 = m_vertices[m_vertexIndices[m_numTris].y] -
                             m_vertices[m_vertexIndices[m_numTris].x];
                Vector3 e2 = m_vertices[m_vertexIndices[m_numTris].z] -
                             m_vertices[m_vertexIndices[m_numTris].x];

                m_normals[nn] = cross(e1, e2);
                m_normalIndices[nn].x = nn;
                m_normalIndices[nn].y = nn;
                m_normalIndices[nn].z = nn;
                nn++;
            }

            m_numTris++;

			if (s4[0] != '\0')
			{
				m_vertexIndices[m_numTris].x = m_vertexIndices[m_numTris - 1].z;
				m_normalIndices[m_numTris].x = m_normalIndices[m_numTris - 1].z;
				m_texCoordIndices[m_numTris].x = m_texCoordIndices[m_numTris - 1].z;

				m_vertexIndices[m_numTris].y = m_vertexIndices[m_numTris - 1].x;
				m_normalIndices[m_numTris].y = m_normalIndices[m_numTris - 1].x;
				m_texCoordIndices[m_numTris].y = m_texCoordIndices[m_numTris - 1].x;

				getIndices(s4, &v, &t, &n);
				m_vertexIndices[m_numTris].z = v-1;
				if (n)
					m_normalIndices[m_numTris].z = n-1;
				if (t)
					m_texCoordIndices[m_numTris].z = t-1;

				if (!n)
				{   // if no normal was supplied
					Vector3 e1 = m_vertices[m_vertexIndices[m_numTris].y] -
								 m_vertices[m_vertexIndices[m_numTris].x];
					Vector3 e2 = m_vertices[m_vertexIndices[m_numTris].z] -
								 m_vertices[m_vertexIndices[m_numTris].x];
					m_normals[nn] = cross(e1, e2);
					m_normalIndices[nn].x = nn;
					m_normalIndices[nn].y = nn;
					m_normalIndices[nn].z = nn;
					nn++;
				}
				m_numTris++;
			}
        }
		else if (strncmp(line, "mtllib", 6) == 0)
		{
			loadMtl(&(line[7]));
		}
		else if (strncmp(line, "usemtl", 6) == 0)
		{
			char texture_name[500];
			sscanf(&line[6], "%s\n", texture_name);
			m_mtl_ids[texture_name].push_back(m_numTris);
		}
		//  else ignore line
    }

    if (!m_texCoords)
        return;
    Vector3 *tans = new Vector3[nv * 2];
    int *ni = new int[nv];
    m_tangents = new Vector3[nv];
    for (int i = 0; i < m_numTris; ++i)
    {
        Vector3 const &a = m_vertices[m_vertexIndices[i].x];
        Vector3 const &b = m_vertices[m_vertexIndices[i].y];
        Vector3 const &c = m_vertices[m_vertexIndices[i].z];
        Vector3 ab = b - a;
        Vector3 ac = c - a;
        VectorR2 const &ua = m_texCoords[m_texCoordIndices[i].x];
        VectorR2 const &ub = m_texCoords[m_texCoordIndices[i].y];
        VectorR2 const &uc = m_texCoords[m_texCoordIndices[i].z];
        VectorR2 uab = ub - ua;
        VectorR2 uac = uc - ua;

        float r = 1.f / (uab.x*uac.y - uac.x*uab.y);
        Vector3 tan_a(uac.y * ab.x - uab.y * ac.x, uac.y * ab.y - uab.y * ac.y, uac.y * ab.z - uab.y * ac.z);
        tan_a *= r;
        Vector3 tan_b(uab.x * ac.x - uac.x * ab.x, uab.x * ac.y - uac.x * ab.y, uab.x * ac.z - uac.x * ab.z);
        tan_b *= r;

        tans[m_vertexIndices[i].x] += tan_a;
        tans[m_vertexIndices[i].y] += tan_a;
        tans[m_vertexIndices[i].z] += tan_a;

        tans[m_vertexIndices[i].x + nv] += tan_a;
        tans[m_vertexIndices[i].y + nv] += tan_a;
        tans[m_vertexIndices[i].z + nv] += tan_a;

        ni[m_vertexIndices[i].x] = m_normalIndices[i].x;
        ni[m_vertexIndices[i].y] = m_normalIndices[i].y;
        ni[m_vertexIndices[i].z] = m_normalIndices[i].z;
    }
    for (int i = 0; i < nv; ++i)
    {
        Vector3 const &normal = m_normals[ni[i]];
        Vector3 const &tangent = tans[i];
        Vector3 const &tangent2 = tans[i + nv];
        if (normal.cross(tangent).dot(tans[i + nv]) < 0.f)
            m_tangents[i] = (tangent2 - normal * normal.dot(tangent2)).normalized();
        else
            m_tangents[i] = (tangent - normal * normal.dot(tangent)).normalized();
    }
    delete[] ni;
    delete[] tans;
}

Material*
TriangleMesh::getMaterialForId(int triangle_id)
{
	unsigned int max_id = 0;
	Material *ret = NULL;
	std::string name;
	for(std::map<std::string, std::vector<unsigned int> >::iterator iter = m_mtl_ids.begin(); iter != m_mtl_ids.end(); ++iter)
	{
		for (int i = 0; i < iter->second.size(); i++)
		{
			if (iter->second[i] <= triangle_id && iter->second[i] >= max_id)
			{
				max_id = iter->second[i];
				ret = m_mtls[iter->first];
				name = iter->first;
			}
		}
	}
	return ret;
}

