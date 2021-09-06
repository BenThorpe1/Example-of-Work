#pragma once

#include "stdafx.h"

//Conters the score
class cCounter :public cMesh
{
public:
	cCounter();
	~cCounter();
	void Init(int a_Num);
	bool Increament(int a_index);
	void Render(class cShaderInfo* pShaderInfo);
private:
	int* m_value;
};


