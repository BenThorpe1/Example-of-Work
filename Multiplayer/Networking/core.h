#pragma once

// These libraries are needed to link the program (Visual Studio specific)
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glfw3.lib")
#pragma comment(lib,"glew32s.lib")
#pragma comment(lib,"glew32.lib")

#define GLEW_STATIC
#include "GL/glew.h" 
#include "GLFW/glfw3.h"

#include <Windows.h>
#include <stdio.h>

#include "textfile.h"
#include "cShader.h"

#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp" 
#include "glm/gtc/type_ptr.hpp"

extern bool check_for_opengl_errors();
extern unsigned int initWidth;
extern unsigned int initHeight;

using namespace glm;

struct sOBB
{
	vec3 m_min, m_max;
};

struct sVec2f
{
	float x, y;
};

struct sVec3f
{
	float x, y, z;
};

struct sRGB
{
	float r, g, b;
};