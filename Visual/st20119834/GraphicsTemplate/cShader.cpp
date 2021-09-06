/////////////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////////////

#include "core.h"
#include "assert.h"
#include <windows.h>
#include <direct.h>
#include <iostream>
#include <string>

//Number of shaders
#define SHADER_COUNT	12
//List of all vertix shaders
const char* vShader[] = {	
				"shaders\\flat.vert",			 
				"shaders\\diffuse.vert",		
				"shaders\\ToonShader.vert",
				"shaders\\Strips.vert",
				"shaders\\LighFalloff.vert",
				"shaders\\Fog.vert",
				"shaders\\BlinnPhongIllumination.vert",
				"shaders\\GrayScale.vert",
				"shaders\\RedScale.vert",
				"shaders\\AntiAllis.vert",
				"shaders\\GeomatryShader.vert",
				"shaders\\Halftone.vert",
};

//List of all geomtric shaders
const char* gShader[] = {   
					"shaders\\flat.geom",			 
					"shaders\\diffuse.geom",		
					"shaders\\ToonShader.geom",
					"shaders\\Strips.geom",
					"shaders\\LighFalloff.geom",
					"shaders\\Fog.geom",
					"shaders\\BlinnPhongIllumination.geom",
					"shaders\\GrayScale.geom",
					"shaders\\RedScale.geom",
					"shaders\\AntiAllis.geom",
					"shaders\\GeomatryShader.geom",
					"shaders\\Halftone.geom",


};


//List of all fragment shaders
const char* fShader[] = { 
				"shaders\\flat.frag",			 
				"shaders\\diffuse.frag",		
				"shaders\\ToonShader.frag",
				"shaders\\Strips.frag",
				"shaders\\LighFalloff.frag",
				"shaders\\Fog.frag",
				"shaders\\BlinnPhongIllumination.frag",
				"shaders\\GrayScale.frag",		
				"shaders\\RedScale.frag",
				"shaders\\AntiAllis.frag",
				"shaders\\GeomatryShader.frag",
				"shaders\\Halftone.frag",




};

//////////////////////////////////////////////////////////////////////////////////////////
// cShaderInfo() - constructor
//////////////////////////////////////////////////////////////////////////////////////////
cShaderInfo::cShaderInfo()
{
	if (sizeof(vShader) / 8 != SHADER_COUNT)
	{
		printf("\ERROR! SHADER_COUNT does not match number of shaders in vShader\n\n");
		assert(0);
	}

	if (sizeof(fShader) / 8 != SHADER_COUNT)
	{
		printf("\ERROR! SHADER_COUNT does not match number of shaders in fShader\n\n");
		assert(0);
	}

	if (sizeof(gShader) / 8 != SHADER_COUNT)
	{
		printf("\ERROR! SHADER_COUNT does not match number of shaders in fShader\n\n");
		assert(0);
	}

	m_num_shaders = SHADER_COUNT;
	m_pList = NULL;
	m_flags = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
// cShader() - constructor
//////////////////////////////////////////////////////////////////////////////////////////
cShader::cShader()
{
	m_vpath = new char[256];
	m_fpath = new char[256];
	m_gpath = new char[256];
	m_cwd = new char[256];

	m_program = -1;
	m_v = -1;
	m_f = -1;
	m_g = -1;
}

//////////////////////////////////////////////////////////////////////////////////////////
// clean() - delete shader object
//////////////////////////////////////////////////////////////////////////////////////////
void cShader::clean(const char *pVertexShader_path, const char *pFragmentShader_Path, const char* pGragmentShader_Path)
{
	if( m_v )
		glDeleteShader(m_v);
	if( m_f )
		glDeleteShader(m_f); 
	if (m_g)
		glDeleteShader(m_g);
	if( m_program )
		glDeleteProgram(m_program);
}

//////////////////////////////////////////////////////////////////////////////////////////
// CheckShader() - creates if anything went wrong in shader compile/linking
//////////////////////////////////////////////////////////////////////////////////////////
void cShader::CheckShader(GLuint id, GLuint type, GLint* ret, const char* onfail)
{
	switch (type)
	{
	case(GL_COMPILE_STATUS):
	{
		glGetShaderiv(id, type, ret);
		if (*ret == false)
		{
			int infologLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infologLength);
			GLchar buffer[512];
			GLsizei charsWritten = 0;
			std::cout << onfail << std::endl;
			glGetShaderInfoLog(id, infologLength, &charsWritten, buffer);
			std::cout << buffer << std::endl;
		}
	}
	break;
	case(GL_LINK_STATUS):
	{
		glGetProgramiv(id, type, ret);
		if (*ret == false) {
			int infologLength = 0;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infologLength);
			GLchar buffer[512];
			GLsizei charsWritten = 0;
			std::cout << onfail << std::endl;
			glGetProgramInfoLog(id, infologLength, &charsWritten, buffer);
			std::cout << buffer << std::endl;
		}
	}
	break;
	default:
		break;
	};
}

//////////////////////////////////////////////////////////////////////////////////////////
// create() - creates a new shader object
//////////////////////////////////////////////////////////////////////////////////////////
void cShader::create(const char *pVertexShader_path, const char *pFragmentShader_Path, const char* pGragmentShader_Path)
{
	char *vs = NULL, *fs = NULL, *gs = NULL;
	GLint ret;

	m_v  = glCreateShader(GL_VERTEX_SHADER);
	m_f  = glCreateShader(GL_FRAGMENT_SHADER);
	m_g = glCreateShader(GL_GEOMETRY_SHADER);

	char * cwd = _getcwd(m_cwd, _MAX_PATH);
	
	// store vertex path..
	strcpy_s( m_vpath, sizeof(char) * 256, m_cwd);
	strcat_s( m_vpath, sizeof(char) * 256, "\\" );
	strcat_s( m_vpath, sizeof(char) * 256, pVertexShader_path );

	// store geomatryy path..
	strcpy_s(m_gpath, sizeof(char) * 256, m_cwd);
	strcat_s(m_gpath, sizeof(char) * 256, "\\");
	strcat_s(m_gpath, sizeof(char) * 256, pGragmentShader_Path);


	// store fragment path..
	strcpy_s( m_fpath, sizeof(char) * 256, m_cwd);
	strcat_s( m_fpath, sizeof(char) * 256, "\\" );
	strcat_s( m_fpath, sizeof(char) * 256, pFragmentShader_Path );

	vs = textFileRead(m_vpath);
	fs = textFileRead(m_fpath);
	gs = textFileRead(m_gpath);
	
	if( !vs )
	{
		printf("\n\nERROR: file: %s not found -  ", m_vpath);
		return;
	}
	if( !fs )
	{
		printf("\n\nERROR: file: %s not found -  ", m_fpath);
		return;
	}
	if (!gs)
	{
		printf("\n\nERROR: file: %s not found -  ", m_gpath);
		return;
	}

	const char *vv = vs;
	const char *ff = fs;
	const char* gg = gs;
	glShaderSource(m_v, 1, &vv, NULL );
	glShaderSource(m_f, 1, &ff, NULL );
	glShaderSource(m_g, 1, &gg, NULL);

	free(vs);
	free(fs);
	free(gs);

	glCompileShader(m_v);
	CheckShader(m_v, GL_COMPILE_STATUS, &ret, "unable to compile the vertex shader!");

	glCompileShader(m_f);
	CheckShader(m_f, GL_COMPILE_STATUS, &ret, "unable to compile the fragment shader!");

	glCompileShader(m_g);
	CheckShader(m_g, GL_COMPILE_STATUS, &ret, "unable to compile the geomtry shader!");


	m_program = glCreateProgram();
	glAttachShader(m_program, m_v);
	glAttachShader(m_program, m_g);
	glAttachShader(m_program, m_f); 

	glLinkProgram(m_program);
	CheckShader(m_program, GL_LINK_STATUS, &ret, "linkage failure!");

	check_for_opengl_errors();

}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
// create() - allocate block of memory to hold ALL shaders
//////////////////////////////////////////////////////////////////////////////////////////
void cShaderInfo::create()
{
	m_num_shaders = SHADER_COUNT;

	m_pList = new cShader[SHADER_COUNT];	

	for( int i=0; i<SHADER_COUNT; i++ )
	{
		list(i)->create( vShader[i], fShader[i],gShader[i] );
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// clean() - delete an existing shader object
//////////////////////////////////////////////////////////////////////////////////////////
void cShaderInfo::clean()
{
	m_num_shaders = SHADER_COUNT;

	if( m_pList )
	{
		for( int i=0; i<SHADER_COUNT; i++ )
		{
			list(i)->clean( vShader[i], fShader[i],gShader[i] );
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// list() - returns requested element from shader list
//////////////////////////////////////////////////////////////////////////////////////////
cShader *cShaderInfo::list(int id)
{
	if( id >= m_num_shaders )
	{
		// if you get here, then you are trying to access a shader element that exceeds the shader array size.
		// try increasing the number_of_shaders created in main() - ShaderInfo.create(number_of_shaders);
		printf("ERROR: ");
		printf("array limits exceeded\n");
	}

	return &m_pList[id];
}

