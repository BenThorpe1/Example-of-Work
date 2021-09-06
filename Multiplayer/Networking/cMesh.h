#pragma once

#include "stdafx.h"
#include "cObjImport.h"

class cMesh
{
public:
	cMesh();
	~cMesh();
	void Init(const char* meshName, int meshID, int shaderID, vec3 pos, vec3 rotAxis, float rotAngle, vec3 scale);
	void Update(float spin_speed);
	void Render(class cShaderInfo* pShaderInfo);
	void RenderSprite(class cShaderInfo* pShaderInfo, const char* buffer, bool bAlphaBlend);
	cObjImport m_obj;
	cTransform m_transform;

	int		m_instance_count;
	bool	m_isInstanced;

private:
};