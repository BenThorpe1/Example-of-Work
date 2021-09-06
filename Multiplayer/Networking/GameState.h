#pragma once
#include "stdafx.h"

class Scene;

class GameState
{
public:
	 GameState();
	~GameState();
	virtual void Init(Scene* a_scene,bool a_isSever);
	virtual void DeInit();
	virtual  void Update(float a_deltaTime);
	void RenderScene(class cShaderInfo* pShaderInfo);
 void RecieveKeyInput(int a_key);
 void RecieveData(Message* a_message);
 void Connection();
 void Disconnected();
void SetID(int a_i) { m_ID = a_i; }
protected:
	Scene* m_scene;
	bool m_isSever;
	int m_ID;
};