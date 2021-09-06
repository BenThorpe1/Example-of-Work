#pragma once
#include "stdafx.h"

#define MAX_TEXT_SIZE 32


class cText :public cMesh
{
public:
	cText() {}
	~cText() {}
	void init(const char* meshName, int mesh_id, int shader_id, vec3 pos, vec3 rot_axis, float rot_angle, vec3 scale, int instance_count);
	void render(cShaderInfo* pShaderInfo, const char* pBuffer, bool bUseAlpha);

	char	m_buffer[32];
	ivec2	m_value;

};