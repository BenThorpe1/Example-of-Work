#include "stdafx.h"
#include <cstdio>

const unsigned int	SHDR_1 = 3;

Menu::Menu()
{
	m_connected = 0;
}
Menu::~Menu()
{

}

void Menu::Init(Scene* a_scene, bool a_isSever)
{
	this->GameState::Init(a_scene, a_isSever);
	m_textStart.init("numerals.obj",3,SHDR_1,vec3(0.0f,0.0f,-0.9),vec3(1,0,0),180.0f,vec3(0.04f,0.2f,1.0f),32);
	glBindVertexArray(0);
	m_textCount.init("numerals.obj", 3, SHDR_1, vec3(0.0f, -0.5f, -0.9), vec3(1, 0, 0), 180.0f, vec3(0.04f, 0.2f, 1.0f), 32);
	glBindVertexArray(0);

	if (a_isSever)
	{
		const char temp[] = "Press E to Start";
		strcpy_s(m_textBuffer, strlen(temp) + 1, temp);
		const char temp2[] = "0 Players connected";
		strcpy_s(m_textBuffer2, strlen(temp2) + 1, temp2);
	}
	else
	{
		const char temp[] = "Waiting for sever to start";
		strcpy_s(m_textBuffer, strlen(temp) + 1, temp);
		const char temp2[] = " ";
		strcpy_s(m_textBuffer2, strlen(temp2) + 1, temp2);
	}
}

void Menu::DeInit()
{}

void Menu::Update(float a_deltaTime)
{
	m_textStart.Update(0.0);
	m_textCount.Update(0.0);
}

void Menu::RenderScene(class cShaderInfo* pShaderInfo)
{
	m_textStart.render(pShaderInfo, m_textBuffer, false);
	m_textCount.render(pShaderInfo, m_textBuffer2, false);
}


void Menu::RecieveKeyInput(int a_key)
{
	if (!m_isSever)
	{
		return;
	}


	switch (a_key)
	{
	case GLFW_KEY_E:
	{
		m_scene->StartGame(m_connected,0);
		break;
	}
	}

}

void Menu::RecieveData(Message* a_message)
{
	if (a_message->GetType() == Message::MessageType::START)
	{
		if (!m_isSever)
		{
			m_scene->StartGame(a_message->GetData().playerPos[0],a_message->GetData().playerPos[1]);
		}
		else
		{
			m_scene->StartGame(m_connected,0);
		}
	}
}



void Menu::Connection()
{
	m_connected++; 

	//Convert number to char[]
	char numTemp[MAX_TEXT_SIZE];
	sprintf_s(numTemp, "%i", m_connected);
	//Insert number text into buffer
	int i = strlen(numTemp)+1;

	const char temp2[] = "Players connected";
	int i2 = strlen(temp2);
	strcat_s(numTemp,i + i2, temp2);
	
	i = strlen(numTemp) + 1;

	strcpy_s(m_textBuffer2, i, numTemp);
}
void Menu::Disconnected()
{
	m_connected--;
}