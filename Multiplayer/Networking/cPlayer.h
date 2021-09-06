#pragma once
#include "stdafx.h"

class cPlayer :public cMesh
{
public:
	cPlayer();
	~cPlayer();

	//Getters
	vec3 GetPos(){
		return m_transform.m_pos;
	}

	sOBB GetOBB(){
		return m_obj.m_obb; // object aligned bounding box
	}
	float GetBSphere(){
		return m_obj.m_bounding_sphere;
	}
	//Get the limits of the bounding box
	vec4 GetMaxOOB();
	vec4 GetMinOOB();

	void Update(float a_yPos);
	
};


