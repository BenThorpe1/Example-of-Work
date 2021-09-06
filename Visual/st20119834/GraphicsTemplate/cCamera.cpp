#include "cCamera.h"


cCamera::cCamera()
{
	projectionMatrix = glm::perspective(glm::radians(45.0f), (float)800.0f / (float)600.0f, 0.5f, 100.f);
	
	m_zoom	= -10.0f;
	m_pan	= 180.0f;
	m_tilt	= 0.0f;
}

cCamera::~cCamera()
{
}

void cCamera::update()
{
	viewMatrix = glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, m_zoom));
//	viewMatrix = glm::rotate(viewMatrix, glm::radians(180.0f), vec3(0.0f, 0.0f, 1.0f));
	viewMatrix = glm::rotate(viewMatrix, glm::radians(m_pan), vec3(0.0f, 1.0f, 0.0f));
	viewMatrix = glm::rotate(viewMatrix, glm::radians(m_tilt), vec3(1.0f, 0.0f, 0.0f));
}