#include "cObjImport.h"
#include <string.h>
#include <stdio.h>
#include "core.h"
#include "cCamera.h"
#include "tga.h"

#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp" 
#include "glm/gtc/type_ptr.hpp"

using namespace glm;

const unsigned int vertexPos_loc	= 0;   // Corresponds to "location = 0" in the vertex shader definition
const unsigned int texCoord_loc		= 1;
const unsigned int vertexNormal_loc = 2;

glm::vec3 LightPosition = glm::vec3(-5.0f, 50.0f, -30.0f);

extern cCamera Camera;

extern sMeshInfo meshInfo[2];

//////////////////////////////////////////////////////////////////////////////////////////
// loadTGA() - .tga file loader
//////////////////////////////////////////////////////////////////////////////////////////
int loadTGA(char* TexName, GLuint TexHandle, GLuint textureID)
{
	TGAImg Img;        // Image loader

	// Load our Texture
	if (Img.Load(TexName) != IMG_OK)
		return false;

	glBindTexture(GL_TEXTURE_2D, TexHandle);

	// Create the texture
	if (Img.GetBPP() == 24)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, 3, Img.GetWidth(), Img.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, Img.GetImg());
	}
	else
		if (Img.GetBPP() == 32)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, 4, Img.GetWidth(), Img.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Img.GetImg());
		}
		else
		{
			return 0;
		}

	// Specify filtering and edge actions
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return Img.GetBPP();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// cObjImport() - 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
cObjImport::cObjImport()
{
	m_fp = 0;

	m_e_count = 0;
	m_v_count  = 0;
	m_vt_count = 0;
	m_vn_count = 0;
	m_f_count  = 0;

	m_v  = NULL;
	m_vt = NULL;
	m_vn = NULL;
	m_fn = NULL;
	m_f  = NULL;

	m_vList = NULL;
	m_eList = NULL;

	m_rotate	= vec3(0,0,0);
	m_translate = vec3(0,0,0);
	m_scale		= 1.0f;

	modelMatrix = mat4(1.0f);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ~cObjImport() - 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
cObjImport::~cObjImport()
{
	if( m_v )
		delete [] m_v;
	if( m_vt )
		delete [] m_vt;
	if( m_vn )
		delete m_vn;
	if( m_f )
		delete m_f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// render() - 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void cObjImport::init(sMeshInfo info)
{
	m_objID = info.meshID;		// object ID;
	load("Meshes/", info.name); // .obj file name

	glGenTextures(m_material_count, m_tex); 

	for (int i = 0; i < m_material_count; i++)
		loadTGA(matList(i)->tex, m_tex[i], i);

	// bind the flag texture..
	for (int i = 0; i < m_material_count; i++)
		glBindTextureUnit(i, m_tex[i]);

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);


	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cVertex) * 3 * m_f_count, m_vList, GL_STATIC_DRAW); // triangles
	glVertexAttribPointer(vertexPos_loc, 3, GL_FLOAT, GL_FALSE, sizeof(cVertex), (void*)0);
	glEnableVertexAttribArray(vertexPos_loc);
	glVertexAttribPointer(vertexNormal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(cVertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(vertexNormal_loc);
	glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(cVertex), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(texCoord_loc);


	// lock down this data to the current VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_shdr = info.shaderID; // shader index
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// rotate() - update the local rotation matrix for this mesh
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void cObjImport::rotate(glm::vec3 rotation, float angle)
{
	m_angle += angle;
	modelMatrix = glm::rotate(modelMatrix, radians(angle), rotation);
}

//Moves an object
void cObjImport::move(glm::vec3 direction, float speed)
{
	m_translate += direction;
	modelMatrix = glm::translate(modelMatrix, direction * speed);
}

//Scales an object
void cObjImport::scale(glm::vec3 scale)
{
	
	modelMatrix = glm::scale(modelMatrix, scale);
}

//Add a texture to any object
void cObjImport::AddTexture(unsigned int texture)
{
	m_tex[m_material_count] = texture;
	glBindTextureUnit(m_material_count, m_tex[m_material_count]);
	
	m_material_count++;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// render() - an object using a set shader
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void cObjImport::renderShader(cShaderInfo* pShaderInfo, int i)
{
	cShader* pList = pShaderInfo->getList();
	int shdr = i;

	if (shdr >= pShaderInfo->shaderCount())
	{
		printf("Error: Can't access requested shader\n");
		shdr = 0;
	}

	// enable shader program..
	glUseProgram(pList[shdr].program());

	//	modelMatrix = glm::rotate(mat4(1.0f), radians(0.0f), vec3(0, 1, 0));
	normalMatrix = transpose(inverse(modelMatrix));

	glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(Camera.viewMatrix));
	glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(Camera.projectionMatrix));
	glUniformMatrix4fv(5, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix3fv(6, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glUniform3fv(7, 1, glm::value_ptr(LightPosition));

	// select which texture we want to use..
	//glBindTextureUnit(0, m_tex[0]);
	// bind the textures..
	for (int i = 0; i < m_material_count; i++)
	{
		glBindTextureUnit(i, m_tex[i]);
	}
	// Draw FACES
	int vertexCount = m_f_count * 3; // calculate the number of vertices for this object
	glBindVertexArray(m_VAO);// myVAO[objID[0]]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// render() - 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void cObjImport::render(cShaderInfo* pShaderInfo)
{
	cShader* pList = pShaderInfo->getList();
	int shdr = m_shdr;

	if (shdr >= pShaderInfo->shaderCount())
	{
		printf("Error: Can't access requested shader\n");
		shdr = 0;
	}

	// enable shader program..
	glUseProgram(pList[shdr].program());

	//	modelMatrix = glm::rotate(mat4(1.0f), radians(0.0f), vec3(0, 1, 0));
	normalMatrix = transpose(inverse(modelMatrix));

	glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(Camera.viewMatrix));
	glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(Camera.projectionMatrix));
	glUniformMatrix4fv(5, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix3fv(6, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glUniform3fv(7, 1, glm::value_ptr(LightPosition));

	// select which texture we want to use..
	//glBindTextureUnit(0, m_tex[0]);
	//	glBindTextureUnit(1, m_tex[1]); // reflection map
	//Bind all textures
	for (int i = 0; i < m_material_count; i++)
	{
		glBindTextureUnit(i, m_tex[i]);
	}
		// Draw FACES
	int vertexCount = m_f_count * 3; // calculate the number of vertices for this object
	glBindVertexArray(m_VAO);// myVAO[objID[0]]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

//Render a object using instance rendering
void cObjImport::renderInstance(cShaderInfo* pShaderInfo,GLuint instanceVBO,int numInstances, int shaderID)
{
	cShader* pList = pShaderInfo->getList();
	int shdr = shaderID;
	//check_for_opengl_errors();

	if (shdr >= pShaderInfo->shaderCount())
	{
		printf("Error: Can't access requested shader\n");
		shdr = 0;
	}
//	check_for_opengl_errors();

	// render the instance buffer
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, instanceVBO);
	//check_for_opengl_errors();

	// enable shader program..
	glUseProgram(pList[shdr].program());
	//check_for_opengl_errors();

	//	modelMatrix = glm::rotate(mat4(1.0f), radians(0.0f), vec3(0, 1, 0));
	normalMatrix = transpose(inverse(modelMatrix));

	glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(Camera.viewMatrix));
	glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(Camera.projectionMatrix));
	glUniformMatrix4fv(5, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix3fv(6, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glUniform3fv(7, 1, glm::value_ptr(LightPosition));
//	check_for_opengl_errors();

	//Bind all textures
	for (int i = 0; i < m_material_count; i++)
	{
		glBindTextureUnit(i, m_tex[i]);
	}
	//check_for_opengl_errors();

	// Draw FACES
	int vertexCount = m_f_count * 3; // calculate the number of vertices for this object
	glBindVertexArray(m_VAO);// myVAO[objID[0]]);
	//glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	//check_for_opengl_errors();

	//Draw all the instances
	glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, numInstances);
	//check_for_opengl_errors();

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// createFaceNormalList() - load into memory list of face normals
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void cObjImport::createFaceNormalList()
{
	// allocate memory to hold one face normal per face
	m_fn = new cVec3f[(int)m_f_count];

	// loop through each face and calculate the face normal
	for (int f = 0; f < m_f_count; f++)
	{
		m_fn[f] = (m_vn[m_f[f].vn_id[0]] + m_vn[m_f[f].vn_id[1]] + m_vn[m_f[f].vn_id[2]]);

		m_fn[f].normalize();
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// faceList() - load into memory list of faces
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void cObjImport::createFaceList()
{
	m_vList = new cVertex[((int)m_f_count) * 3];

	for (int f = 0; f < m_f_count; f++)
	{
		// vertices
		m_vList[f * 3 + 0].m_v	= m_v[m_f[f].v_id[0]];
		m_vList[f * 3 + 1].m_v	= m_v[m_f[f].v_id[1]];
		m_vList[f * 3 + 2].m_v	= m_v[m_f[f].v_id[2]];

		// vertex normals
		m_vList[f * 3 + 0].m_n = m_vn[m_f[f].vn_id[0]];
		m_vList[f * 3 + 1].m_n = m_vn[m_f[f].vn_id[1]];
		m_vList[f * 3 + 2].m_n = m_vn[m_f[f].vn_id[2]];

		// texture coordinates
		m_vList[f * 3 + 0].m_uv = m_vt[m_f[f].vt_id[0]];
		m_vList[f * 3 + 1].m_uv = m_vt[m_f[f].vt_id[1]];
		m_vList[f * 3 + 2].m_uv = m_vt[m_f[f].vt_id[2]];
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// createEdgeList() - load into memory list of edges
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void cObjImport::createEdgeList()
{
	m_e_count = m_f_count * 3;
	int num_vertices = m_e_count * 2;
	m_eList = new cVec3f[num_vertices]; // two vertices needed to per edge

	int e = 0;
	for (int f = 0; f < m_f_count; f++)
	{
		m_eList[e++] = m_v[m_f[f].v_id[0]];
		m_eList[e++] = m_v[m_f[f].v_id[1]];

		m_eList[e++] = m_v[m_f[f].v_id[1]];
		m_eList[e++] = m_v[m_f[f].v_id[2]];

		m_eList[e++] = m_v[m_f[f].v_id[2]];
		m_eList[e++] = m_v[m_f[f].v_id[0]];
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// importMTL() - 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void cObjImport::importMTL()
{
	char buffer[64];
	char mtl_path[256];

	int buffer_size = sizeof(buffer);

	strcpy_s ( mtl_path, m_mtl_filename);

	// load .mtl file
	 fopen_s( &m_mtl, mtl_path , "r"  );
	
	if( !m_mtl )
		return;


	m_material_count = 0;
	
	// search through mtl file and find out how materials we have
	do
	{
		fscanf_s( m_mtl, "%s", buffer, buffer_size);
	
		if( strcmp( buffer, "newmtl" ) == 0  )
			m_material_count++;		

	}while( !feof(m_mtl) );


	// allocate sufficient memory to load all materials
	m_mtlList = new newmtl[m_material_count];

	
	// return back to start of .mtl file ready for the big read
	fseek( m_mtl, 0, SEEK_SET );

	int count = -1;

	// search through mtl file and extract all materials for this object
	do
	{
		fscanf_s( m_mtl, "%s", buffer, buffer_size );
	
		if( strcmp( buffer, "newmtl" ) == 0 )
		{
			count++;
			// read in material name
			fscanf_s( m_mtl, "%s", buffer, buffer_size);
			strcpy_s( m_mtlList[count].name, buffer);
		}
		else
		{
			if( strcmp( buffer, "Kd" ) == 0 )
			{
				// read in diffuse values
				fscanf_s( m_mtl, "%f%f%f", &m_mtlList[count].diffuse.r, &m_mtlList[count].diffuse.g, &m_mtlList[count].diffuse.b );
			}
			else
				if (strcmp(buffer, "map_Kd") == 0)
				{
					char texturePath[128];
					strcpy_s(texturePath, "tga/");
					// read in the texture name
					fscanf_s(m_mtl, "%s", m_mtlList[count].tex, (unsigned int)sizeof(m_mtlList[count].tex));
					strcat_s(texturePath, m_mtlList[count].tex);
					strcpy_s(m_mtlList[count].tex, texturePath);
				}
		}

	}while( !feof(m_mtl) );


	// OK, so we've finished with the .mtl file
	fclose(m_mtl);
	m_mtl = NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// loadFaces() - extracts face data from .obj file
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void cObjImport::loadFaces()
{
	char buffer[32] = { NULL };
	int  f[3] = { NULL };
	char slash[2] = { NULL };
	int  face = 0;
	int  m_index = 0;
	char materialName[64] = { NULL };
	
	fscanf_s( m_fp, "%s", buffer, (unsigned int)sizeof(buffer));
	do
	{		
		if( strcmp(buffer, "usemtl") == 0 )
		{
			fscanf_s( m_fp, "%s", materialName, (unsigned int)sizeof(materialName));// material name

			// convert material name into index into m_mtlList
			for( int m=0; m<m_f_count; m++ )
			{
				if (strcmp( materialName, m_mtlList[m].name) == 0 )
				{
					m_index = m;
					break;
				}
				
			}
		}

		if( strcmp(buffer, "f") == 0 )
		{
			for( int i=0; i<3; i++ )
			{
				fscanf_s( m_fp, "%d%c%d%c%d", &f[0], &slash, (unsigned int)sizeof(slash), &f[1], &slash, (unsigned int)sizeof(slash), &f[2] );
				
				// store indices for each vertex on this face here..
				if( i==0 )
				{
					m_f[face].mtl_id = m_index;
				}

				m_f[face].v_id[i]  = f[0]-1;
				m_f[face].vt_id[i] = f[1]-1;
				m_f[face].vn_id[i] = f[2]-1;
			}

			face++;
		}

		fscanf_s( m_fp, "%s", buffer, (unsigned int)sizeof(buffer));

	}while( !feof(m_fp) );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// load() - 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void cObjImport::load(const char *obj_path, const char* obj_name)
{
	char filepath[64];

	strcpy_s( filepath, obj_path );
	strcat_s( filepath, obj_name );

	fopen_s(&m_fp, filepath, "r" );

	if( !m_fp )
		return;

	static char buffer[32];

	// find the material library used for this object..
	do
	{
		fscanf_s( m_fp, "%s", buffer, unsigned int(sizeof(char)*32) );
	}while( strcmp( buffer, "mtllib" ) != 0 );

	// this is the .mtl filename
	fscanf_s( m_fp, "%s", buffer, unsigned int(sizeof(char) * 32));

	// store .mtl filename for later use
	strcpy_s( m_mtl_filename, obj_path );
	strcat_s( m_mtl_filename, buffer );


	// read ahead and find out how much data we have
	do
	{
		fscanf_s( m_fp, "%s", buffer, unsigned int(sizeof(char) * 32));

		if( strcmp( buffer, "v" ) == 0  )m_v_count++;
		if( strcmp( buffer, "vt" ) == 0 )m_vt_count++;
		if( strcmp( buffer, "vn" ) == 0 )m_vn_count++;
		if( strcmp( buffer, "f" ) == 0 )m_f_count++;

	}while( !feof(m_fp) );


	// allocate memory required to hold the data
	m_v = new cVec3f[m_v_count];
	m_vt = new sVec2f[m_vt_count];
	m_vn = new cVec3f[m_vn_count];
	m_f  = new sFace[m_f_count];


	// return back to start of file ready for the big read
	fseek( m_fp, 0, SEEK_SET );

	// move cursor to the start of vertex list
	do
	{
		fscanf_s( m_fp, "%s", buffer, unsigned int(sizeof(char) * 32));
	}while( strcmp( buffer, "v" ) != 0 );

	fscanf_s( m_fp, "%f%f%f", &m_v[0].x, &m_v[0].y, &m_v[0].z );

	// vertices
	for( int i=1; i<m_v_count; i++ )
	{
		fscanf_s( m_fp, "%s%f%f%f", buffer, unsigned int(sizeof(char) * 32), &m_v[i].x, &m_v[i].y, &m_v[i].z );
	}

	// texture coordinates
	for( int i=0; i<m_vt_count; i++ )
	{
		fscanf_s( m_fp, "%s%f%f", buffer, unsigned int(sizeof(char) * 32), &m_vt[i].x, &m_vt[i].y );
		m_vt[i].y = 1.0f - m_vt[i].y;
	}

	// vertex normals
	for( int i=0; i<m_vn_count; i++ )
	{
		fscanf_s( m_fp, "%s%f%f%f", buffer, unsigned int(sizeof(char) * 32), &m_vn[i].x, &m_vn[i].y, &m_vn[i].z );
	}

	// load in all material data...
	importMTL();

	// load in face data
	loadFaces();

	createFaceNormalList();
	createFaceList();
	createEdgeList();

	fclose(m_fp);
	m_fp = NULL;
}

