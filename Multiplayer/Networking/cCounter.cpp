#include "stdafx.h"

const int MAX_VALUE = 10;
int numPlayers = 0;

cCounter::cCounter()
{
}

cCounter::~cCounter()
{
	if (m_value)
	{
		delete[] m_value;
	}
}

void cCounter::Init(int a_Num)
{
	m_value = new int[a_Num];
	for (int i = 0; i < a_Num; i++)
	{
		m_value[i] = 0;
	}
	numPlayers = a_Num;
}
void cCounter::Render(class cShaderInfo* pShaderInfo)
{
	//Render text Not implemented
}

bool cCounter::Increament(int a_index)
{
	m_value[a_index]++;

	if (m_value[a_index] >= MAX_VALUE)
	{
		return true;
	}
	return false;
}
