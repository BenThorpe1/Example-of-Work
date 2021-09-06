#pragma once

#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp" 
#include "glm/gtc/type_ptr.hpp"
#include "core.h"


class cTransform
{
public:

	cTransform();
	~cTransform();

	void init(vec3 pos, vec3 rot_axis, float rot_angle, vec3 scale);

	vec3	m_rotAxis;
	vec3	m_pos;
	vec3	m_scale;
	float	m_rotAngle; // local rotation of this mesh
};