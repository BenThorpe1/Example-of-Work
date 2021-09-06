#pragma once

#include <stdio.h>
#include <math.h>
#include "core.h"

#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp" 
#include "glm/gtc/type_ptr.hpp"

using namespace glm;

struct sFace
{
	int v_id[3];	// vertex indices
	int vt_id[3];	// texCoord indices
	int vn_id[3];	// normals indices
	int mtl_id;		// material index
};

class cVec3f
{
public:
	cVec3f operator +(const cVec3f v)
	{
		cVec3f out;
		out.x = this->x + v.x;
		out.y = this->y + v.y;
		out.z = this->z + v.z;

		return out;
	}

	void normalize()
	{
		float d = sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);

		this->x /= d;
		this->y /= d;
		this->z /= d;
	}

	cVec3f operator /(const cVec3f v)
	{
		cVec3f out;
		out.x = this->x / v.x;
		out.y = this->y / v.y;
		out.z = this->z / v.z;

		return out;
	}

	float x, y, z;
};


struct newmtl
{
	sRGB	diffuse;
	char	tex[64];
	char	name[64];
};

class cFace
{
public:
	cVec3f	m_v[3];
	cVec3f	m_fn;
};

class cVertex
{
public:
	cVec3f	m_v;  // vertex
	cVec3f	m_n;  // vertex normal
	sVec2f	m_uv; // texture coordinates
};

class cObjImport
{
public:
	cObjImport();
	~cObjImport();
		
	void init(sMeshInfo info);// const char* pFilename, const int objectID, const int shaderIndex);
	void rotate(glm::vec3 rotation, float theta);
	
	void move(glm::vec3 direction, float speed);	
	void scale(glm::vec3 scale);
	
	void AddTexture(unsigned int texture);
		
	void render(class cShaderInfo *pShaderInfo);
	void renderShader(cShaderInfo* pShaderInfo, int i);
	void renderInstance(cShaderInfo* pShaderInfo, GLuint instanceVBO, int numInstances,int shaderID);
	void load(const char *obj_path, const char* obj_name);
	void importMTL();
	void loadFaces();

	void createFaceNormalList();
	void createFaceList();
	void createEdgeList();

	inline int numFaces()
	{
		return m_f_count;
	}
	inline cVec3f *v(int index)
	{
		return &m_v[index];
	}

	inline newmtl	*matList(int index)
	{
		return &m_mtlList[index];
	}

	FILE		*m_fp;
	FILE		*m_mtl;

	/////////////////////////////////////////////////////////////////////////////////////
	// VBO:		Vertex Buffer Object holds an array of data. 
	//			The data contained within the VBO is typically vertex, normal, colour and 
	//			texture coordinate data
	/////////////////////////////////////////////////////////////////////////////////////
	unsigned int m_VBO;
	/////////////////////////////////////////////////////////////////////////////////////
	// VAO:		Vertex Array Object holds info about how the data stored within the VBO is  
	//			formatted. The VAO holds a pointer to the start of each vertex attribute - 
	//			i.e. vertex, normal, colour, texture coordinate, along with the number of 
	//			elements for each attribute, (a vertex has 3 elements, a texture 
	//			coordinate has 2 elements), the step size (stride) needed to move to the  
	//			beginning of the next item in the list
	/////////////////////////////////////////////////////////////////////////////////////
	unsigned int m_VAO;

	unsigned int m_tex[32];
	
	char m_mtl_filename[64];

	int m_objID;
	int m_v_count;
	int m_vt_count;
	int m_vn_count;
	int m_f_count;
	int m_e_count;
	int m_material_count;
	int m_shdr; // shader index - used to select a shader from shader list

	glm::mat4	modelMatrix;			// model matrix  
	glm::mat3	normalMatrix;			// normal matrix

	glm::vec3	m_rotate;
	glm::vec3	m_translate;
	float		m_scale;
	float		m_angle; // ocal rotation of this mesh

	cVec3f *m_v;	// vertex list
	sVec2f *m_vt;	// uv coord list
	cVec3f *m_vn;	// vertex normal list
	cVec3f* m_fn;	// face normals list
	sFace  *m_f;	// indices for each face vertex - i.e. m_f[0].x = m_v, m_f[0].y = m_vt, m_f[0].z = m_vn

	cVec3f	*m_eList;		// edge list
	cVertex	*m_vList;		// vertex list
	newmtl	*m_mtlList;		// material list
};

