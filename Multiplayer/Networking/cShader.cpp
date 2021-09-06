#include "core.h"
#include "assert.h"
#include <windows.h>
#include <direct.h>
#include <iostream>
#include <string>

#define SHADER_COUNT	4


const char *vShader[] = {	"shaders\\diffuse.vert",		//
							"shaders\\counter.vert",		//
							"shaders\\splash.vert",			//
							"shaders\\string.vert",	
};

const char *fShader[] = {	"shaders\\diffuse.frag",		//
							"shaders\\counter.frag",		//
							"shaders\\splash.frag",			//
							"shaders\\string.frag",
};


//////////////////////////////////////////////////////////////////////////////////////////
// cShaderInfo() - constructor
//////////////////////////////////////////////////////////////////////////////////////////
cShaderInfo::cShaderInfo()
{
	if (sizeof(vShader) / 8 != SHADER_COUNT)
	{
		printf("ERROR! SHADER_COUNT does not match number of shaders in vShader\n\n");
		assert(0);
	}

	if (sizeof(fShader) / 8 != SHADER_COUNT)
	{
		printf("ERROR! SHADER_COUNT does not match number of shaders in fShader\n\n");
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
	m_cwd = new char[256];

	m_program = -1;
	m_v = -1;
	m_f = -1;
}

//////////////////////////////////////////////////////////////////////////////////////////
// clean() - delete shader object
//////////////////////////////////////////////////////////////////////////////////////////
void cShader::clean(const char *pVertexShader_path, const char *pFragmentShader_Path)
{
	if( m_v )
		glDeleteShader(m_v);
	if( m_f )
		glDeleteShader(m_f); 
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
void cShader::create(const char *pVertexShader_path, const char *pFragmentShader_Path)
{
	char *vs = NULL, *fs = NULL, *gs = NULL;
	GLint ret;

	m_v  = glCreateShader(GL_VERTEX_SHADER);
	m_f  = glCreateShader(GL_FRAGMENT_SHADER);

	char * cwd = _getcwd(m_cwd, _MAX_PATH);
	
	// store vertex path..
	strcpy_s( m_vpath, sizeof(char) * 256, m_cwd);
	strcat_s( m_vpath, sizeof(char) * 256, "\\" );
	strcat_s( m_vpath, sizeof(char) * 256, pVertexShader_path );

	// store fragment path..
	strcpy_s( m_fpath, sizeof(char) * 256, m_cwd);
	strcat_s( m_fpath, sizeof(char) * 256, "\\" );
	strcat_s( m_fpath, sizeof(char) * 256, pFragmentShader_Path );

	vs = textFileRead(m_vpath);
	fs = textFileRead(m_fpath);

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


	const char *vv = vs;
	const char *ff = fs;

	glShaderSource(m_v, 1, &vv, NULL );
	glShaderSource(m_f, 1, &ff, NULL );

	free(vs);
	free(fs);

	glCompileShader(m_v);
	CheckShader(m_v, GL_COMPILE_STATUS, &ret, "unable to compile the vertex shader!");

	glCompileShader(m_f);
	CheckShader(m_v, GL_COMPILE_STATUS, &ret, "unable to compile the fragment shader!");


	m_program = glCreateProgram();
	glAttachShader(m_program, m_v);
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
		list(i)->create( vShader[i], fShader[i] );
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
			list(i)->clean( vShader[i], fShader[i] );
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

