#pragma once
#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp" 
#include "glm/gtc/type_ptr.hpp"

using namespace glm;

class cCamera
{
public:
	cCamera();
	~cCamera();

	void update();

	glm::mat4 projectionMatrix;		// projection matrix
	glm::mat4 viewMatrix;			// view matrix

	float m_zoom;
	float m_pan;
	float m_tilt;
};

