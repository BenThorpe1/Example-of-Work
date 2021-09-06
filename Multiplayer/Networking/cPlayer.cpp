#include "stdafx.h"
//Empty
cPlayer::cPlayer()
{
}

// Empty
cPlayer::~cPlayer()
{

}

void cPlayer::Update(float a_yPos)
{
	//Update the players position to match inputed
	vec3 position = vec3(m_transform.m_pos.x, a_yPos, m_transform.m_pos.z);
	m_transform.m_pos = position;
		this->cMesh::Update(0.0);
}

//Return the limits of  bounding box
vec4 cPlayer::GetMaxOOB()
{
	return m_obj.modelMatrix * vec4(m_obj.m_obb.m_max, 1.0f);
}
vec4 cPlayer::GetMinOOB()
{
	return m_obj.modelMatrix * vec4(m_obj.m_obb.m_min,1.0f);
}