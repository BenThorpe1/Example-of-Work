#pragma once

#include "stdafx.h"

class Menu : public GameState
{
public:
	Menu();
	~Menu();
	void Init(Scene* a_scene,bool a_isSever)override;
	void DeInit()override;
	void Update(float a_deltaTime);
	void RenderScene(class cShaderInfo* pShaderInfo);
	void RecieveKeyInput(int a_key);
	
	void RecieveData(Message* a_message);
	void Connection();
	void Disconnected();
private:
	cText m_textStart;
	cText m_textCount;

	int m_connected;
	int m_accpeted; 
	char m_textBuffer[MAX_TEXT_SIZE];
	char m_textBuffer2[MAX_TEXT_SIZE];

};
