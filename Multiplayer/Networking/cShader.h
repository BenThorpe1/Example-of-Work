//////////////////////////////////////////////////////////////////////////////////////////
// cShader.h
//////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class cShader
{
public:

	cShader();
	~cShader()
	{
		if (m_vpath)
			delete[] m_vpath;
		if (m_fpath)
			delete[] m_fpath;
		if (m_cwd)
			delete[] m_cwd;
	}

	inline void			cwd(char *path){ strcpy_s( m_cwd, sizeof(char)*32, path );}
	inline char			*cwd(){return m_cwd;}
	
	inline unsigned int	program(){return m_program;}

	void CheckShader(GLuint id, GLuint type, GLint* ret, const char* onfail);
	void clean(const char *pVertexShader_path, const char *pFragmentShader_Path);
	void create(const char *pVertexShader_path, const char *pFragmentShader_Path);

private:
	char* m_vpath;
	char* m_fpath;
	char* m_cwd;

	GLuint m_v;
	GLuint m_f;

	unsigned int m_program;
};


class cShaderInfo
{
public:

	cShaderInfo();
	~cShaderInfo(){}
	void create();//int count);
	void clean();

	inline cShader	*getList(){	return m_pList;}

	cShader *list(int id);

	void set_flags(unsigned int value)
	{
		m_flags = value;
	}

	unsigned int get_flags()
	{
		return m_flags;
	}

	inline void shaderCount(int count){ m_num_shaders = count;}
	inline int  shaderCount(){return m_num_shaders;}
	

private:
	cShader		*m_pList;

	int				m_num_shaders;
	unsigned int	m_flags;		// used to render individual maps
};
